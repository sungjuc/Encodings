#include <iostream>
#include <cstdint>
#include <array>
#include "../common/common.h"

#ifndef ENCODINGS_GROUPVARIN_H
#define ENCODINGS_GROUPVARIN_H
namespace encodings {
namespace groupvarint {
const uint8_t kGroupSize = 4;
const uint64_t kMask[kGroupSize] = {0xffff, 0xffffff, 0xffffffff, 0xffffffffffULL};
const uint8_t kHeaderSize = 1;
const uint8_t kTagSize = 8;
const uint8_t kMinBytes = 2;

template<typename T = std::array<uint64_t, kGroupSize> >
struct GroupVarintCodec : public common::Codec<T> {
public:
	uint64_t Encode(uint8_t *begin, const T &value) {
		uint8_t *ptr = begin;
		uint8_t num_bytes[kGroupSize];

		for (uint8_t i = 0; i < kGroupSize; i++) {
			num_bytes[i] = NumBytes(value[i]);
		}
		*ptr++ = (num_bytes[0] << 6) | (num_bytes[1] << 4) | (num_bytes[2] << 2) | num_bytes[3];

		for (uint8_t i = 0; i < kGroupSize; i++) {
			std::memcpy(ptr, &value[i], num_bytes[i] + kMinBytes);
			ptr += num_bytes[i] + kMinBytes;
		}

		return ptr - begin;
	}

	uint64_t Decode(const uint8_t *begin, T *value) {
		const uint8_t *ptr = begin;
		uint8_t num_bytes;
		uint8_t tag = *ptr++;

		for (uint8_t i = 0; i < kGroupSize; i++) {
			num_bytes = (tag >> ((kGroupSize - (i + 1)) * 2)) & 3;
			uint64_t dval = *reinterpret_cast<const uint64_t *>(ptr) & kMask[num_bytes];
			value->at(i) = dval;
			ptr += num_bytes + kMinBytes;
		}

		return ptr - begin;
	}

private:
	/**
	 * Computes the number of bytes to encode.
	 */
	const uint8_t NumBytes(uint64_t x) {
		uint8_t num_bytes = 8 - (__builtin_clzll(x | 1) / 8);
		if (num_bytes > 1) --num_bytes;
		return --num_bytes;
	}
};

struct EdgeGroupVarintCodec {
	GroupVarintCodec<std::array<uint64_t, kGroupSize> > codec;
	uint64_t Encode(uint8_t *begin, const common::EdgeWithId &edge) {
		std::array<common::Id, kGroupSize> data;
		for (uint8_t i = 0; i < common::kNLinkages; ++i) {
			data[i] = edge.link.linkages[i];
		}

		data[common::kNLinkages] = edge.id;
		uint64_t offset = 0;

		offset += codec.Encode(begin + offset, data);

		return offset;
	}

	uint64_t Decode(uint8_t *begin, common::EdgeWithId *edge) {
		uint8_t *ptr = begin;
		uint8_t num_bytes;
		uint8_t tag = *ptr++;

		for (uint8_t i = 0; i < kGroupSize - 1; i++) {
			num_bytes = (tag >> ((kGroupSize - (i + 1)) * 2)) & 3;
			edge->link.linkages[i] = *reinterpret_cast<uint64_t *>(ptr) & kMask[num_bytes];
			ptr += num_bytes + kMinBytes;
		}

		num_bytes = tag & 3;
		edge->id = *reinterpret_cast<uint64_t *>(ptr) & kMask[num_bytes];
		ptr += num_bytes + kMinBytes;

		return ptr - begin;
	}
};

struct EdgeGroupVarintDeltaCompressionCodec {
	EdgeGroupVarintCodec codec;

	uint64_t Encode(uint8_t *begin, const common::EdgeWithId &edge) {
		uint8_t *ptr = begin;
		common::EdgeWithId new_edge;
		uint8_t compression_tag = 0;

		common::Id subject = edge.link.GetLinkage(common::kSubject);
		common::Id predicate = edge.link.GetLinkage(common::kPredicate);
		common::Id object = edge.link.GetLinkage(common::kObject);

		new_edge.link.linkages = edge.link.linkages;
		if (predicate > subject) {
			compression_tag |= 0b00010000;
			new_edge.link.linkages[common::kPredicate] = predicate - subject;
		}

		if (object > predicate) {
			compression_tag |= 0b00001000;
			new_edge.link.linkages[common::kObject] = object - predicate;
		} else if (object > subject) {
			compression_tag |= 0b00000100;
			new_edge.link.linkages[common::kObject] = object - subject;
		}

		if (compression_tag == 0) {
			compression_tag |= 0b00000001;
			new_edge.id = edge.id - subject;
		} else if (subject < object || subject < predicate) {
			if (object > predicate) {
				compression_tag |= 0b00000011;
				new_edge.id = edge.id - object;
			} else {
				compression_tag |= 0b00000010;
				new_edge.id = edge.id - predicate;
			}
		}

		*ptr++ = compression_tag;
		ptr += codec.Encode(ptr, new_edge);

		return ptr - begin;
	}

	uint64_t Decode(uint8_t *begin, common::EdgeWithId *edge) {
		uint8_t *ptr = begin;
		uint64_t num_bytes;
		uint8_t compression_tag = *ptr++;

		ptr += codec.Decode(ptr, edge);


		if (compression_tag & 0b00110000) {
			edge->link.linkages[common::kPredicate] += edge->link.GetLinkage(common::kSubject);
		}

		if (compression_tag & 0b00001000) {
			edge->link.linkages[common::kObject] += edge->link.GetLinkage(common::kPredicate);
		}

		if (compression_tag & 0b00000100) {
			edge->link.linkages[common::kObject] += edge->link.GetLinkage(common::kSubject);
		}

		if (compression_tag & 0b00000001) {
			edge->id += edge->link.GetLinkage(common::kSubject);
		} else 	if (compression_tag & 0b00000011) {
			edge->id += edge->link.GetLinkage(common::kObject);
		}

		if (compression_tag & 0b00000010) {
			edge->id += edge->link.GetLinkage(common::kPredicate);
		}



		return ptr - begin;
	}
};
}  // end namespace groupvarint
}  // end namespace encodings

#endif  // ENCODINGS_GROUPVARIN_H
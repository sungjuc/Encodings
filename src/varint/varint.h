#include <stdint.h>
#include "../common/common.h"
#include "../common/types.h"

#ifndef ENCODINGS_VARINT_H_
#define ENCODINGS_VARINT_H_

namespace encodings {
namespace varint {

template<typename T = uint64_t>
struct VarintCodec : public common::Codec<T> {
	uint64_t Encode(uint8_t *begin, const T &value) {
		T val = value;
		uint8_t *ptr = begin;
		while (val > 0x7F) {
			*ptr++ = static_cast<uint8_t>(val & 0x7F) | uint8_t(0x80);
			val >>= 7;
		}
		*ptr++ = static_cast<uint8_t>(val);
		return ptr - begin;
	}

	uint64_t Decode(const uint8_t *begin, T *value) {
		const uint8_t *ptr = begin;
		*value = *ptr & 0x7F;
		for (uint_fast8_t i = 1; *ptr & 0x80; ++i) *value |= T(*(++ptr) & 0x7F) << (i * 7);
		return ++ptr - begin;
	}
};

struct EdgeVarintCodec {
	VarintCodec<common::Id> codec;

	uint64_t Encode(uint8_t *begin, const common::EdgeWithId &edge) {
		uint64_t offset = 0;
		for (common::Id linkage : edge.link.linkages) {
			offset += codec.Encode(begin + offset, linkage);
		}
		offset += codec.Encode(begin + offset, edge.id);
		return offset;
	}

	uint64_t Decode(uint8_t *begin, common::EdgeWithId *edge) {
		uint64_t offset = 0;
		for (uint8_t i = 0; i <= common::kObject; ++i) {
			offset += codec.Decode(begin + offset, &(edge->link.linkages[i]));
		}

		offset += codec.Decode(begin + offset, &(edge->id));
		return offset;
	}
};

}  // end namespace varint
}  // end namespace encodings

#endif  //  ENCODINGS_VARINT_H_

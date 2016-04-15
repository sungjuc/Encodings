#include <iostream>
#include <cstdint>
#include <array>
#include "../common/common.h"

#ifndef VARINT_GROUPVARINT_H_
#define VARINT_GROUPVARINT_H_

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
			uint64_t value_to_encode = value[i];
			std::memcpy(ptr, &value_to_encode, num_bytes[i] + kMinBytes);
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
			uint8_t val[8];
			std::memcpy(&val, ptr, sizeof(uint64_t));
			uint64_t dval = *(reinterpret_cast<uint64_t *>(&val)) & kMask[num_bytes];
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
}  // end namespace groupvarint

#endif  // VARINT_GROUPVARINT_H_
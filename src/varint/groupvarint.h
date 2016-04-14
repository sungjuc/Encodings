#include "../common/common.h"

#ifndef VARINT_GROUPVARINT_H_
#define VARINT_GROUPVARINT_H_

namespace groupvarint {

const enum

template<typename T = std::array<P, 4>
struct GroupVarintCodec : public common::Codec<T> {
	uint64_t Encode(uint8_t *begin, const T &value) {
		uint8_t* header = begin;
		uint8_t* body = header + 1;
		for (uint8_t i = 0; i < value.size(); i++) {
			P data = value[0];
			while (data > 0xFF) {

			}
		}
		return ptr - begin;
	}

	uint64_t Decode(const uint8_t *begin, value_type *value) {
		const uint8_t *ptr = begin;
		*value = *ptr & 0x7F;
		for (uint_fast8_t i = 1; *ptr & 0x80; ++i) *value |= value_type(*(++ptr) & 0x7F) << (i * 7);
		return ++ptr - begin;
	}
};
}  // end namespace groupvarint

#endif  // VARINT_GROUPVARINT_H_
#include <stdint.h>

#ifndef ENCODINGS_COMMON_H
#define ENCODINGS_COMMON_H
namespace common {
template<typename T>
struct Codec {
	virtual uint64_t Encode(uint8_t *begin, const T &value) = 0;

	virtual uint64_t Decode(const uint8_t *begin, T *value) = 0;

	virtual ~Codec() { }
};
}  // end namespace common

#endif  //  ENCODINGS_COMMON_H

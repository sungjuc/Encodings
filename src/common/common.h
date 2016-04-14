#include <stdint.h>

#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_
namespace common {
template <typename T>
struct Codec {
  virtual uint64_t Encode(uint8_t* begin, const T& value) = 0;
  virtual uint64_t Decode(const uint8_t* begin, T* value) = 0;
  virtual ~Codec() {}
};
}  // end namespace common

#endif  //  COMMON_COMMON_H_

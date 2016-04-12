#ifndef SUNGJUC_ENCODINGS_COMMON_H_
#define SUNGJUC_ENCODINGS_COMMON_H_

namespace sungjuc {
namespace encodings {
namespace common {
<T>
struct Codec {
  virtual uint64_t Encode(uint8_t* begin, const T& value) = 0;
  virtual uint64_t Decode(const uint8_t* begin, T* value) = 0;
}
}  // end namespace common
}  // end namespace encodings
}  // end namespace sungjuc

#endif  //  SUNGJUC_ENCODINGS_COMMON_H_

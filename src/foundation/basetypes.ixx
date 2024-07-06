module;
#include <cstdint>

export module foundation.basetypes;

namespace mbg {
  //primitive typedefs
  export {
    using SizeType  = size_t;
    using Double    = double;
    using Float     = float;
    using Integer   = int32_t;
    using UnsignedInt = uint32_t;
    using ShortInt  = uint16_t;
    using Character = int8_t;
    using Boolean   = bool;
  }
}
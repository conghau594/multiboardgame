module;
export module foundation.Utility;

import foundation.basetypes;

namespace mbg {
  export class Utility final {
  public:
    virtual ~Utility() = 0;

    static Integer getBitmask(Integer number) { return 1 << number; }

  };
}
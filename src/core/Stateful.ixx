export module core.Stateful;

import foundation.basetypes;

namespace mbg {
  /// Interface for all states.
  export class Stateful {
  public:
    virtual Boolean onEntering() = 0; ///< Does something before entering this state. Returns false if the entering fails.
    virtual Boolean onExiting()  = 0;  ///< Does something before exiting this state. Returns false if the exiting fails.
  };
}
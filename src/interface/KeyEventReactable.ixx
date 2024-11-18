export module core.KeyEventReactable;

namespace mbg {
  export class KeyEventReactable {
  public:
    virtual void onKeyPress() = 0;
    virtual void onKeyRelease() = 0;
  };
}
export module core.MouseEventReactable;

namespace mbg {
  export class MouseEventReactable {
  public:
    virtual void onMouseMove() = 0;
    virtual void onMouseButtonPress() = 0;
    virtual void onMouseButtonRelease() = 0;
  };
}
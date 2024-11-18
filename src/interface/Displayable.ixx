export module core.Displayable;

namespace mbg {
  export class Displayable {
  public:
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void display() = 0;
    virtual void clear() = 0;
  };
}
export module core.Updatable;

namespace mbg {
  ///Interface that the purpose is to force a class to define its own `update()` and `render()` methods.
  export class Updatable {
  public:
    virtual void update() = 0;
    virtual void render() = 0;
  };
}
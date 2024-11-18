export module foundation.Object;

namespace mbg {
  ///The parent of all classes.
  export class Object {
  public:
    virtual ~Object() = 0;
  };

  Object::~Object() {}
}
export module core.SceneModel;

import foundation.basetypes;
import foundation.Matrix;
import foundation.SharedPointer;
import foundation.Object;

namespace mbg {
  ///[abstract] Bussiness model of application (right-border interface)
  export class SceneModel : public Object {
  public:
    using Pointer = SharedPointer<SceneModel>;
    virtual ~SceneModel() = 0;

    //inline void add(Entity* entity, SizeType i); ///< Adds an entity to the group number `i`.
    //[x]TODO.2 Do you need remove(), clear(), add()? --> NO

  protected:
    SceneModel() = default;
  };
  SceneModel::~SceneModel() { }
}
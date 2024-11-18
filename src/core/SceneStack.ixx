export module core.SceneStack;

import foundation.basetypes;
import foundation.List;
import foundation.Object;
import foundation.SharedPointer;
import core.Updatable;
import core.SceneController;

namespace mbg {
  ///A container containing and managing scene controllers
  export class SceneStack : public Object, public Updatable {
  public://types
    using Pointer = SharedPointer<SceneStack>;

  public://member functions
    virtual ~SceneStack() { };
    
    SceneController::Pointer getFront() { return scene_controllers_.front(); } ///< Gets the top object.
    bool empty() { return scene_controllers_.empty(); }   ///< Checks if the stack is empty.
    SizeType size() { return scene_controllers_.size(); } ///< Gets size of the stack.
    void clear() { scene_controllers_.clear(); }  ///< Clears object if needed.

    void push(SceneController::Pointer sceneController) { ///< Adds an object for managing.
      if (sceneController->onEntering()) {
        scene_controllers_.push_front(sceneController);
      }
    } 

    void pop() {///< Removes the top object.
      if (not empty()) {
        if (getFront()->onExiting()) {
          scene_controllers_.pop_front();
        }
      } 
    }

    void render() override { scene_controllers_.back()->render(); } ///< Renders the last scene but you can override to render whatever scenes you want from the stack.
    void update() override { scene_controllers_.back()->update(); } ///< Updates app logic of the last scene but you can override to update whatever scenes you want from the stack.

  public://static functions
    inline static Pointer createPointer() { return Pointer(new SceneStack()); }

  protected://contructors
    SceneStack() = default;

  private://attributes
    List<SceneController::Pointer> scene_controllers_;
  };
}
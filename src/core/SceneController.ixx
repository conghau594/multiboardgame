export module core.SceneController;

import foundation.Object;
import foundation.SharedPointer;
import core.Stateful;

import core.Updatable;
//import core.SceneState;
import core.SceneView;
import core.SceneModel;
import core.EventHandler;

namespace mbg {
  export class SceneStack;

  ///A controller of scene according to MVC pattern (interface).
  export class SceneController : public Object, public Updatable, public Stateful {
  public://types
    using Pointer = SharedPointer<SceneController>;

  public://member functions
    virtual ~SceneController() { }

    //inline void add(EntityController* entityCtrler, SizeType i); ///< Adds an entity controller to the group number `i`.
    //[x]TODO.1 Do you need remove(), clear(), add()? --> NO
    //void update() { scene_state_->update(); }
    //void render() { scene_state_->render(); } ///< Delegates the scene view to render the scene.
    
    SceneModel::Pointer getModel()  const { 
      return scene_model_; 
    }
    SceneView::Pointer  getView()   const { return scene_view_; }
    EventHandler::Pointer getEventHandler() const { return event_handler_; }
    
    //TODO.039 See class EntityView to write function getSceneStack()

    void setModel(SceneModel::Pointer sceneModel)  {  scene_model_ = sceneModel; }
    void setView(SceneView::Pointer sceneView)   { scene_view_ = sceneView; }
    void setEventHandler(EventHandler::Pointer eventHandler) { event_handler_ = eventHandler; }

  protected://contructors
    SceneController(
      SceneModel::Pointer model, 
      SceneView::Pointer view, 
      EventHandler::Pointer eventHandler,
      SharedPointer<SceneStack> sceneStack
    ) : scene_model_(model), scene_view_(view), event_handler_(eventHandler), scene_stack_(sceneStack)
    {}

    void setSceneStack(SharedPointer<SceneStack> sceneStack) { scene_stack_ = sceneStack; }

  private://member data
    SceneModel::Pointer scene_model_;
    SceneView::Pointer scene_view_;

    EventHandler::Pointer event_handler_;
    WeakPointer<SceneStack> scene_stack_;
  };
}

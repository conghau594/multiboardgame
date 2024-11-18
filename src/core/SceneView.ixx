export module core.SceneView;

import foundation.SharedPointer;
import foundation.Object;
import core.Window;

namespace mbg {
  ///A view (or sreen) corresponding with a use case.
  /***************************************************************************
   * Each scene view contains some entity views for displaying and for user to
   * interact with. This conforms to MVC pattern.
   * \sa SceneController, SceneModel
   ***************************************************************************/
  export class SceneView : public Object {
  public://types
    using Pointer = SharedPointer<SceneView>;
    virtual ~SceneView() = 0;

  //public://member functions
    //virtual void render(Window::Pointer window) = 0;  ///< Renders screen by drawing all entity views belonging to it.
  
  protected:
    SceneView() = default;
  };

  SceneView::~SceneView() { }

}
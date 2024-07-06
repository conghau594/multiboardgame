module;

export module sdl_display.SDLSceneController;

import core.SceneController;
import core.SceneStack;
import core.AvatarManager;
import sdl_display.SDLWindow;
import sdl_display.SDLEventHandler;

namespace mbg {
  export class SDLSceneController : public SceneController { //[x]TODO.019 this inheritance must be changed to `Game2DController`
  public://type
    using Pointer = SharedPointer<SceneController>;

  public:
    AvatarManager::Pointer getAvatarManager() const { return avatar_mgr_; }
    SDLWindow::Pointer     getWindow() const { return window_; }

  public://static functions
    template<typename... Args> static inline Pointer createPointer(Args&&... args) {
      return Pointer(new SDLSceneController(std::forward<Args>(args)...));
    }

  protected://contructors
    SDLSceneController(
      SceneModel::Pointer model, 
      SceneView::Pointer view,
      SDLWindow::Pointer window,
      SDLEventHandler::Pointer eventHandler, 
      AvatarManager::Pointer avatarMgr
    ) : SceneController(model, view, eventHandler, nullptr), 
        window_(window), 
        avatar_mgr_(avatarMgr)
    { }

  private:
    SDLWindow::Pointer window_;
    AvatarManager::Pointer avatar_mgr_;
  };
}
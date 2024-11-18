module;

export module sdl_display.SDLGame2DController;

import core.SceneStack;
import core.AvatarManager;

import game_controller.Game2DController;
import game_controller.BoardGameState;
import sdl_display.SDLWindow;
import sdl_display.SDLEventHandler;

namespace mbg {
  export class SDLGame2DController : public Game2DController { //[x]TODO.019 this inheritance must be changed to `Game2DController`
  public://type
    using Pointer = SharedPointer<SDLGame2DController>;

  public:
    AvatarManager::Pointer getAvatarManager() const { return avatar_mgr_; }
    SDLWindow::Pointer     getWindow() const { return window_; }

  public://static functions
    template<typename... Args> static inline Pointer createPointer(Args&&... args) {
      return Pointer(new SDLGame2DController(std::forward<Args>(args)...));
    }

  //protected://contructors
    SDLGame2DController(
      Game2DModel::Pointer model, 
      Game2DView::Pointer view, 
      BoardGameState::Pointer firstGameState,
      SDLWindow::Pointer window,
      SDLEventHandler::Pointer eventHandler, 
      AvatarManager::Pointer avatarMgr
    ) : Game2DController(model, view, eventHandler, nullptr, firstGameState),
        window_(window), 
        avatar_mgr_(avatarMgr)
    { }

  private:
    SDLWindow::Pointer window_;
    AvatarManager::Pointer avatar_mgr_;
  };
}
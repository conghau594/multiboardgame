export module app.SDLAppEventHandler;

import foundation.SharedPointer;
import sdl_display.SDLEventHandler;
import chess.SDLChessGameEventManager;

namespace mbg {
  export class SDLAppEventHandler : public SDLEventHandler {
  public:
    enum Type {
      MOUSE = SDLEventHandler::MOUSE,
      KEYBOARD = SDLEventHandler::KEYBOARD,
      WINDOW = SDLEventHandler::WINDOW,
      CHESS_GAME
    }; ///< MOUSE or KEYBOARD or...

    using Pointer = SharedPointer<SDLAppEventHandler>;


    SDLEvent getEvent() override { ///< This function can be overrided to change from SDL_WaitEvent to SDL_PollEvent or SDL_PumpEvent or anything simillar...
      SDLEvent event;
      SDL_WaitEvent(&event);
      return event;
    } ///< 

    void handleUserEvent(SDLEvent* event) override;

    SDLChessGameEventManager::Pointer getChessGameEventManager() {
      return std::static_pointer_cast<SDLChessGameEventManager>(EventHandler::operator[](CHESS_GAME));
    }

  public://static functions
    template<typename... Args> static inline Pointer createPointer(Args... args) {
      return Pointer(new SDLAppEventHandler(std::forward<Args>(args)...));
    }

  protected://contructor
    SDLAppEventHandler()
    {
      add(SDLChessGameEventManager::createPointer());
    }


  private:

  };


}
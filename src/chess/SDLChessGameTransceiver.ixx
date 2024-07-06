module;
import <SDL2/SDL_events.h>;

#ifdef _DEBUG
import <iostream>;
#endif
#define TEST

export module chess.SDLChessGameTransceiver;

import foundation.basetypes;
import foundation.Object;
import foundation.SharedPointer;
import foundation.String;

import core.Transceiver;
#ifdef TEST
import sdl_display.SDLWindow;
#endif

//import chess.SDLChessGameEventManager;

namespace mbg {
  export class SDLChessGameTransceiver : public Transceiver {
  public://type
    using Pointer = SharedPointer<SDLChessGameTransceiver>;

  public:
    void pushEvent(UnsignedInt typeIndex, Integer info1, Integer info2) {
      SDL_Event event;
      //SDL_memset(&event, 0, sizeof(event)); /* or SDL_zero(event) */
      event.type = typeIndex;
      event.user.windowID = window_->getWindowID();
      //event.user.code = my_event_code;
      event.user.data1 = (void*)info1;
      event.user.data2 = (void*)info2;

      SDL_PushEvent(&event);

#ifdef _DEBUG
      std::cout << "\n\n...void pushEvent(UnsignedInt typeIndex, UnsignedInt windowID, Integer info)...";
      std::cout << "\n    event.window.windowID = " << event.window.windowID;
      std::cout << "\n    event.user.windowID   = " << event.user.windowID;
#endif
    }

    void sendEvent(UnsignedInt typeIndex, Integer info1, Integer info2 = 0) {
      enemy_transceiver_->pushEvent(typeIndex, info1, info2);
    }

  public:
    template<typename... Args> static inline Pointer createPointer(Args... args) {
      return Pointer(new SDLChessGameTransceiver(std::forward<Args>(args)...));
    }

//  protected:
//    SDLChessGameTransceiver() = default;

#ifdef TEST
  public:
    SDLChessGameTransceiver::Pointer getEnemyTransceiver() const { return enemy_transceiver_; }
    void setEnemyTransceiver(SDLChessGameTransceiver::Pointer enemyTransceiver) { enemy_transceiver_ = enemyTransceiver; }

    SDLWindow::Pointer getWindow() const { return window_; }
    void setWindow(SDLWindow::Pointer wnd) { window_ = wnd; }

  protected:
    SDLChessGameTransceiver(SDLWindow::Pointer wnd, SDLChessGameTransceiver::Pointer enemyTransceiver = nullptr)
      : window_(wnd), enemy_transceiver_(enemyTransceiver)
    { }

  private:
    SDLWindow::Pointer window_;
    SDLChessGameTransceiver::Pointer enemy_transceiver_;
#endif
  };
}
export module app.SDLGameEventHandler;

import sdl_display.SDLEventHandler;

namespace mbg {
  export class SDLGameEventHandler : public SDLEventHandler {
  public:
    void handleUserEvent() override;

  private:

  };
}
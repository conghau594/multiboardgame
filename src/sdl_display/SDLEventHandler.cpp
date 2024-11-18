module sdl_display.SDLEventHandler;


namespace mbg {
  void SDLEventHandler::update(SDLEvent* event) {
    (*(EventHandler::operator[](MOUSE)))[MouseEventManager::EVENT_TYPE] = event->type;

#ifdef _DEBUG1
    static SizeType i = 0;
    std::cout << "\n" << ++i;
#endif
    switch (event->type) {
    case MOUSE_MOVED:
      getMouseEventManager()->setCursorPos(event->motion.x, event->motion.y);
      ++EventHandler::event_count_;
#ifdef _DEBUG1
      std::cout << "\n\n\t...void SDLEventHandler::update(SDLEvent* event)...";
      std::cout << "\n  mouse moved";
      std::cout << "\n\t x = " << event->motion.x;
      std::cout << "\n\t y = " << event->motion.y;
#endif // _DEBUG
      break;

    case MOUSE_BUTTON_DOWN:
      (*getMouseEventManager())[MouseEventManager::BUTTONS_BITMASK] |= Utility::getBitmask(event->button.button);
      ++EventHandler::event_count_;

#ifdef _DEBUG
      std::cout << "\n\n...void SDLEventHandler::update(SDLEvent * event)...";
      std::cout << "\n\tmouse is pressed: " << (*getMouseEventManager())[MouseEventManager::BUTTONS_BITMASK] << "\n";
#endif
      break;

    case MOUSE_BUTTON_UP:
      (*getMouseEventManager())[MouseEventManager::BUTTONS_BITMASK] &= ~Utility::getBitmask(event->button.button);
      ++EventHandler::event_count_;
#ifdef _DEBUG
      std::cout << "\n\n...void SDLEventHandler::update(SDLEvent* event)...";
      std::cout << "\n\tmouse is released: " << (*getMouseEventManager())[MouseEventManager::BUTTONS_BITMASK] << "\n";
#endif
      break;

    case SDL_WINDOWEVENT:
      switch (event->window.event) {
      case SDL_WINDOWEVENT_ENTER:


#ifdef _DEBUG1
        std::cout << "\n\n...void SDLEventHandler::update(SDLEvent* event)...";
        std::cout << "\n  window enter, mouse pos";
        std::cout << "\n\t x = " << (*getMouseEventManager()).getCursorX();
        std::cout << "\n\t y = " << (*getMouseEventManager()).getCursorY();

#endif
        break;

      case SDL_WINDOWEVENT_LEAVE:
#ifdef _DEBUG1
        std::cout << "\n\n...void SDLEventHandler::update(SDLEvent* event)...";
        std::cout << "\n  window leave, mouse pos";
        std::cout << "\n\t x = " << (*getMouseEventManager())[MouseEventManager::CURSOR_X];
        std::cout << "\n\t y = " << (*getMouseEventManager())[MouseEventManager::CURSOR_Y];
#endif
        break;

      case SDL_WINDOWEVENT_CLOSE:
        viewframe_.lock()->exit();
        break;

      default:
        break;
      }
      break;

    case MOUSE_WHEEL:

#ifdef _DEBUG1
      std::cout << "\n\n...void SDLEventHandler::update(SDLEvent* event)...";
      std::cout << "\n\tmouse is wheeling\n";
#endif
      break;

    case SDL_USER_EVENT:
      handleUserEvent(event);
      break;

    case QUIT:
      viewframe_.lock()->exit();
      break;


    default:
      break;
    }
  }
}
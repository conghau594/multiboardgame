module;

import <SDL2/SDL_events.h>;

export module sdl_display.SDLEventHandler;

import foundation.basetypes;
import foundation.Vector;
import foundation.Utility;
import foundation.SharedPointer;
import core.EventHandler;
import core.MouseEventManager;
import core.KeyboardEventManager;
import core.WindowEventManager;
import core.ViewFrame;


namespace mbg {
  export using SDLEvent = SDL_Event;

  export class SDLEventHandler : public EventHandler {
  public://type
    enum Type {
      MOUSE = 0,
      KEYBOARD,
      WINDOW
    }; ///< MOUSE or KEYBOARD or...
    enum Event; ///<equivalent to SDL_EventType
    using Pointer = SharedPointer<SDLEventHandler>;

  public://member functions
    virtual SDLEvent getEvent() = 0;
    
    virtual void update(SDLEvent* event);

    void handleEvent() override {
      SDLEvent event = getEvent();
      update(&event);
    }

    virtual void handleUserEvent(SDLEvent* event) = 0;

    void setViewFrame(ViewFrame::Pointer viewframe) { viewframe_ = viewframe; }

    MouseEventManager::Pointer getMouseEventManager() override {
      return std::static_pointer_cast<MouseEventManager>(EventHandler::operator[](MOUSE));
    }    

    KeyboardEventManager::Pointer getKeyboardEventManager() override {
      return std::static_pointer_cast<KeyboardEventManager>(EventHandler::operator[](KEYBOARD));
    }

    WindowEventManager::Pointer getWindowEventManager() override {
      return std::static_pointer_cast<WindowEventManager>(EventHandler::operator[](WINDOW));
    }

  protected://contructor
    SDLEventHandler() 
    {
      add(MouseEventManager::createPointer());
      add(KeyboardEventManager::createPointer());
      add(WindowEventManager::createPointer());
    }    

  private:
    WeakPointer<ViewFrame> viewframe_;
  };


  enum SDLEventHandler::Event {
    FIRST_EVENT = SDL_FIRSTEVENT, ///< = 0,     /**< Unused (do not remove) */

    /* Application events */
    QUIT = SDL_QUIT, ///< = 0x100, /**< User-requested quit */

    /* These application events have special meaning on iOS, see README-ios.md for details */
    APP_TERMINATING = SDL_APP_TERMINATING,    /**< The application is being terminated by the OS
                                                   Called on iOS in applicationWillTerminate()
                                                   Called on Android in onDestroy()
                                               */
    APP_LOW_MEMORY = SDL_APP_LOWMEMORY,       /**< The application is low on memory, free memory if possible.
                                                   Called on iOS in applicationDidReceiveMemoryWarning()
                                                   Called on Android in onLowMemory()
                                              */
    APP_WILL_ENTER_BACKGROUND = SDL_APP_WILLENTERBACKGROUND, /**< The application is about to enter the background
                                     Called on iOS in applicationWillResignActive()
                                     Called on Android in onPause()
                                */
    APP_DID_ENTER_BACKGROUND = SDL_APP_DIDENTERBACKGROUND, /**< The application did enter the background and may not get CPU for some time
                                     Called on iOS in applicationDidEnterBackground()
                                     Called on Android in onPause()
                                */
    APP_WILL_ENTER_FOREGROUND = SDL_APP_WILLENTERFOREGROUND, /**< The application is about to enter the foreground
                                     Called on iOS in applicationWillEnterForeground()
                                     Called on Android in onResume()
                                */
    APP_DID_ENTER_FOREGROUND = SDL_APP_DIDENTERFOREGROUND, /**< The application is now interactive
                                     Called on iOS in applicationDidBecomeActive()
                                     Called on Android in onResume()
                                */
                                /* Display events */
    DISPLAY_EVENT = SDL_DISPLAYEVENT, ///< = 0x150,  /**< Display state change */

    /* Window events */
    WINDOW_EVENT = SDL_WINDOWEVENT,  ///< = 0x200, /**< Window state change */
    SYSWM_EVENT = SDL_SYSWMEVENT,             /**< System specific event */

    /* Keyboard events */
    KEYDOWN = SDL_KEYDOWN,  ///< = 0x300, /**< Key pressed */
    KEYUP = SDL_KEYUP,                  /**< Key released */
    TEXT_EDITTING = SDL_TEXTEDITING,            /**< Keyboard text editing (composition) */
    TEXT_INPUT = SDL_TEXTINPUT,              /**< Keyboard text input */
    KEYMAP_CHANGED = SDL_KEYMAPCHANGED,          /**< Keymap changed due to a system event such as an
                                     input language or keyboard layout change.
                                */
    TEXT_EDITTING_EXT = SDL_TEXTEDITING_EXT,       /**< Extended keyboard text editing (composition) */

    /* Mouse events */
    MOUSE_MOVED = SDL_MOUSEMOTION,  ///< = 0x400, /**< Mouse moved */
    MOUSE_BUTTON_DOWN = SDL_MOUSEBUTTONDOWN,        /**< Mouse button pressed */
    MOUSE_BUTTON_UP = SDL_MOUSEBUTTONUP,          /**< Mouse button released */
    MOUSE_WHEEL = SDL_MOUSEWHEEL,             /**< Mouse wheel motion */

    /* Clipboard events */
    CLIPBOARD_UPDATE = SDL_CLIPBOARDUPDATE, ///< = 0x900, /**< The clipboard or primary selection changed */

    /** Events SDL_USEREVENT through SDL_LASTEVENT are for your use,
     *  and should be allocated with SDL_RegisterEvents()
     */
    SDL_USER_EVENT = SDL_USEREVENT, ///< = 0x8000,

    /**
     *  This last event is only for bounding internal arrays
     */
    SDL_LAST_EVENT = SDL_LASTEVENT, ///< = 0xFFFF

    LOCALE_CHANGE = SDL_LOCALECHANGED,  /**< The user's locale preferences have changed. */

    /* Joystick events */
    JOY_AXIS_MOTION = SDL_JOYAXISMOTION, ///< = 0x600, /**< Joystick axis motion */
    JOY_BALL_MOTION = SDL_JOYBALLMOTION,          /**< Joystick trackball motion */
    JOY_HAT_MOTION = SDL_JOYHATMOTION,           /**< Joystick hat position change */
    JOY_BUTTON_DOWN = SDL_JOYBUTTONDOWN,          /**< Joystick button pressed */
    JOY_BUTTON_UP = SDL_JOYBUTTONUP,            /**< Joystick button released */
    JOY_DEVICE_ADDED = SDL_JOYDEVICEADDED,         /**< A new joystick has been inserted into the system */
    JOY_DEVICE_REMOVED = SDL_JOYDEVICEREMOVED,       /**< An opened joystick has been removed */
    JOY_BATTERY_UPDATED = SDL_JOYBATTERYUPDATED,      /**< Joystick battery level change */

    /* Game controller events */
    CONTROLLER_AXIS_MOTION = SDL_CONTROLLERAXISMOTION, ///< = 0x650, /**< Game controller axis motion */
    CONTROLLER_BUTTON_DOWN = SDL_CONTROLLERBUTTONDOWN,          /**< Game controller button pressed */
    CONTROLLER_BUTTON_UP = SDL_CONTROLLERBUTTONUP,            /**< Game controller button released */
    CONTROLLER_DEVICE_ADDED = SDL_CONTROLLERDEVICEADDED,         /**< A new Game controller has been inserted into the system */
    CONTROLLER_DEVICE_REMOVED = SDL_CONTROLLERDEVICEREMOVED,       /**< An opened Game controller has been removed */
    CONTROLLER_DEVICE_REMAPPED = SDL_CONTROLLERDEVICEREMAPPED,      /**< The controller mapping was updated */
    CONTROLLER_TOUCHPAD_DOWN = SDL_CONTROLLERTOUCHPADDOWN,        /**< Game controller touchpad was touched */
    CONTROLLER_TOUCHPAD_MOTION = SDL_CONTROLLERTOUCHPADMOTION,      /**< Game controller touchpad finger was moved */
    CONTROLLER_TOUCHPAD_UP = SDL_CONTROLLERTOUCHPADUP,          /**< Game controller touchpad finger was lifted */
    CONTROLLER_SENSOR_UPDATE = SDL_CONTROLLERSENSORUPDATE,        /**< Game controller sensor was updated */
    CONTROLLER_UPDATE_COMPLETE_RESERVED_FOR_SDL3 = SDL_CONTROLLERUPDATECOMPLETE_RESERVED_FOR_SDL3,
    CONTROLLER_STEAM_HANDLE_UPDATED = SDL_CONTROLLERSTEAMHANDLEUPDATED,  /**< Game controller Steam handle has changed */

    /* Touch events */
    FINGER_DOWN = SDL_FINGERDOWN, ///< = 0x700,
    FINGER_UP = SDL_FINGERUP,
    FINGER_MOTION = SDL_FINGERMOTION,

    /* Gesture events */
    DOLLAR_GESTURE = SDL_DOLLARGESTURE, ///< = 0x800,
    DOLLAR_RECORD = SDL_DOLLARRECORD,
    MULTIGESTURE = SDL_MULTIGESTURE,


    /* Drag and drop events */
    DROP_FILE = SDL_DROPFILE, ///< = 0x1000, /**< The system requests a file open */
    DROP_TEXT = SDL_DROPTEXT,                 /**< text/plain drag-and-drop event */
    DROP_BEGIN = SDL_DROPBEGIN,                /**< A new set of drops is beginning (NULL filename) */
    DROP_COMPLETE = SDL_DROPCOMPLETE,             /**< Current set of drops is now complete (NULL filename) */

    /* Audio hotplug events */
    AUDIO_DEVICE_ADDED = SDL_AUDIODEVICEADDED, ///< = 0x1100, /**< A new audio device is available */
    AUDIO_DEVICE_REMOVED = SDL_AUDIODEVICEREMOVED,        /**< An audio device has been removed. */

    /* Sensor events */
    SDL_SENSORUPDATESDL_SENSORUPDATE = 0x1200,     /**< A sensor was updated */

    /* Render events */
    RENDER_TARGETS_RESET = SDL_RENDER_TARGETS_RESET, ///< = 0x2000, /**< The render targets have been reset and their contents need to be updated */
    RENDER_DEVICE_RESET = SDL_RENDER_DEVICE_RESET, /**< The device has been reset and all textures need to be recreated */

    /* Internal events */
    SDL_POLL_SENTINEL = SDL_POLLSENTINEL, ///< = 0x7F00, /**< Signals the end of an event poll cycle */

  };
}
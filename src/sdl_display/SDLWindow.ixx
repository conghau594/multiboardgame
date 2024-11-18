module;
import <SDL2/SDL_render.h>;

export module sdl_display.SDLWindow;

import foundation.basetypes;
import foundation.String;
import foundation.SharedPointer;
//import foundation.PointerThisShareable;
//import core.RectangleParams;
import core.Window;

namespace mbg {
  ///Interface that the purpose is to force a class to define its own `update()` method.
  export class SDLWindow final : public Window {
  public://types
    ///< Equivalent to `SDL_WindowFlags`
    enum Initilization;
    using Pointer = SharedPointer<SDLWindow>;

  public://member functions
    UnsignedInt getWindowID() const {
      return SDL_GetWindowID(sdl_window_);
    }

    inline ~SDLWindow() {
      if (sdl_window_ != nullptr) {
        SDL_DestroyWindow(sdl_window_);
        sdl_window_ = nullptr;

        if (sdl_renderer_ != nullptr) {
          SDL_DestroyRenderer(sdl_renderer_);
          sdl_renderer_ = nullptr;
        }
      }
    }
    SDLWindow(const SDLWindow&) = delete;
    SDLWindow& operator=(const SDLWindow&) = delete;

    inline bool initialize() override;
    void show() override { SDL_ShowWindow(sdl_window_); }
    void hide() override { SDL_HideWindow(sdl_window_); }
    void display() override { SDL_RenderPresent(sdl_renderer_); }
    void clear() override { SDL_RenderClear(sdl_renderer_); }
    SDL_Renderer* getRenderer() const { return sdl_renderer_; }

  public://static
    template<typename... Args> static inline Pointer createPointer(Args&&... args) {
      return Pointer(new SDLWindow(std::forward<Args>(args)...));
    }

  private://contructor
    SDLWindow() = default;
    SDLWindow(String name, Integer width, Integer height)
      : Window(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height) { }
    
    SDLWindow(String name, Integer xpos, Integer ypos, Integer width, Integer height)
      : Window(name, xpos, ypos, width, height) { }

  private:
    SDL_Window* sdl_window_;
    SDL_Renderer* sdl_renderer_;
  };

  /***************************************************************************/
  enum SDLWindow::Initilization {
    WINDOW_FULLSCREEN         = SDL_WINDOW_FULLSCREEN,         ///< 0x00000001,         /**< fullscreen window */
    WINDOW_OPENGL             = SDL_WINDOW_OPENGL,             ///< 0x00000002,             /**< window usable with OpenGL context */
    WINDOW_SHOWN              = SDL_WINDOW_SHOWN,              ///< 0x00000004,              /**< window is visible */
    WINDOW_HIDDEN             = SDL_WINDOW_HIDDEN,             ///< 0x00000008,             /**< window is not visible */
    WINDOW_BORDERLESS         = SDL_WINDOW_BORDERLESS,         ///< 0x00000010,         /**< no window decoration */
    WINDOW_RESIZABLE          = SDL_WINDOW_RESIZABLE,          ///< 0x00000020,          /**< window can be resized */
    WINDOW_MINIMIZED          = SDL_WINDOW_MINIMIZED,          ///< 0x00000040,          /**< window is minimized */
    WINDOW_MAXIMIZED          = SDL_WINDOW_MAXIMIZED,          ///< 0x00000080,          /**< window is maximized */
    WINDOW_MOUSE_GRABBED      = SDL_WINDOW_MOUSE_GRABBED,      ///< 0x00000100,      /**< window has grabbed mouse input */
    WINDOW_INPUT_FOCUS        = SDL_WINDOW_INPUT_FOCUS,        ///< 0x00000200,        /**< window has input focus */
    WINDOW_MOUSE_FOCUS        = SDL_WINDOW_MOUSE_FOCUS,        ///< 0x00000400,        /**< window has mouse focus */
    WINDOW_FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP, ///< (SDL_WINDOW_FULLSCREEN | 0x00001000),
    WINDOW_FOREIGN            = SDL_WINDOW_FOREIGN,            ///< 0x00000800,            /**< window not created by SDL */
    WINDOW_ALLOW_HIGH_DPI     = SDL_WINDOW_ALLOW_HIGHDPI,      ///< 0x00002000,      /**< window should be created in high-DPI mode if supported.
                                                               ///< On macOS NSHighResolutionCapable must be set true in the
                                                               ///< application's Info.plist for this to have any effect. */
    WINDOW_MOUSE_CAPTURE      = SDL_WINDOW_MOUSE_CAPTURE,      ///< 0x00004000,   /**< window has mouse captured (unrelated to MOUSE_GRABBED) */
    WINDOW_ALWAYS_ON_TOP      = SDL_WINDOW_ALWAYS_ON_TOP,      ///< 0x00008000,   /**< window should always be above others */
    WINDOW_SKIP_TASKBAR       = SDL_WINDOW_SKIP_TASKBAR,       ///< 0x00010000,   /**< window should not be added to the taskbar */
    WINDOW_UTILITY            = SDL_WINDOW_UTILITY,            ///< 0x00020000,   /**< window should be treated as a utility window */
    WINDOW_TOOLTIP            = SDL_WINDOW_TOOLTIP,            ///< 0x00040000,   /**< window should be treated as a tooltip */
    WINDOW_POPUP_MENU         = SDL_WINDOW_POPUP_MENU,         ///< 0x00080000,   /**< window should be treated as a popup menu */
    WINDOW_KEYBOARD_GRABBED   = SDL_WINDOW_KEYBOARD_GRABBED,   ///< 0x00100000,   /**< window has grabbed keyboard input */
    WINDOW_VULKAN             = SDL_WINDOW_VULKAN,             ///< 0x10000000,   /**< window usable for Vulkan surface */
    WINDOW_METAL              = SDL_WINDOW_METAL,              ///< 0x20000000,   /**< window usable for Metal view */

    WINDOW_INPUT_GRABBED      = SDL_WINDOW_INPUT_GRABBED       ///< SDL_WINDOW_MOUSE_GRABBED /**< equivalent to SDL_WINDOW_MOUSE_GRABBED for compatibility */
    };
  
  /***************************************************************************/
  inline Boolean SDLWindow::initialize() {
    //RectangleParams::Pointer params = getParameters();
    sdl_window_ = SDL_CreateWindow(
      getName().c_str(), 
      (int)getX(), 
      (int)getY(), 
      (int)getWidth(),
      (int)getHeight(), 
      SDL_WindowFlags(WINDOW_HIDDEN));
    
    if (sdl_window_ == nullptr) //TODO.009 Add log
      return false;

    sdl_renderer_ = SDL_CreateRenderer(sdl_window_, -1, 0);
    if (sdl_renderer_ == nullptr)//TODO.010 Add log
      return false;

    SDL_SetRenderDrawColor(sdl_renderer_, 255, 255, 255, 255); ///background color = white
    return true;
  }
}

module;
import <SDL2/SDL_render.h>;
import <SDL2/SDL_image.h>;

export module sdl_display.SDLAvatar;

import foundation.basetypes;
import foundation.String;
import foundation.SharedPointer;
import core.Window;
import core.Avatar;
import sdl_display.SDLWindow;

namespace mbg {
  export using SDLTexture = SDL_Texture;

  export  class SDLAvatar final : public Avatar {
  public://types
    using Pointer = SharedPointer<SDLAvatar>;

  public://member functions
    ~SDLAvatar() {
      if (texture_ != nullptr)
        SDL_DestroyTexture(texture_);
    }
    SDLAvatar(const SDLAvatar&) = delete;
    SDLAvatar& operator=(const SDLAvatar&) = delete;

    SDLTexture* getTexture() const { return texture_; }
    Integer getWidth() const { return width_; }
    Integer getHeight() const { return height_; }

    inline Boolean load(String picturePath, Window::Pointer window);

  public://static functions
    template<typename... Args> static inline Pointer createPointer(Args&&... args) {
      return Pointer(new SDLAvatar(std::forward<Args>(args)...));
    }

  protected://contructors
    SDLAvatar(SizeType index) : Avatar(index)
    { }
    SDLAvatar(SizeType index, String picturePath, Window::Pointer window) : Avatar(index) {
      if (load(picturePath, window) == false) {
        //TODO.017 add log:
      }
      else {
        //TODO.018 add log:
      }
    }

  private://attributes
    SDL_Texture* texture_;
    Integer width_;
    Integer height_;
  };

  /***************************************************************************/
  inline Boolean SDLAvatar::load(String picturePath, Window::Pointer window) {
    SDL_Surface* surface = IMG_Load(picturePath.c_str());
    if (surface == nullptr)
      return false; //TODO.015 add log:

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(
      static_cast<SDLWindow*>(window.get())->getRenderer(),
      surface
    );
    SDL_FreeSurface(surface);
    if (newTexture == nullptr)
      return false; //TODO.016 add log:

    if (texture_ != nullptr) {
      SDL_DestroyTexture(texture_);
    }
    texture_ = newTexture;

    SDL_QueryTexture(texture_, nullptr, nullptr, (int*)&width_, (int*)&height_);
    return true;
  }
}
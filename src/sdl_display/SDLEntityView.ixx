module;
import <SDL2/SDL_render.h>;

export module sdl_display.SDLEntityView;

import foundation.basetypes;
import core.EntityModel;
import core.EntityView;
import sdl_display.SDLWindow;
import sdl_display.SDLRectangle;
import sdl_display.SDLPoint;
import sdl_display.SDLAvatar;
import sdl_display.SDLEntityParams;

namespace mbg {
  export class SDLEntityView : public EntityView {
  public://types
    using Pointer = SharedPointer<SDLEntityView>;

  public://functions
    inline void draw(Window::Pointer window) override {
      if (avatar() != nullptr) {
        SDL_RenderCopyEx(
          static_cast<SDLWindow*>(window.get())->getRenderer(),
          static_cast<SDLAvatar*>(avatar().get())->getTexture(),
          &(parameters_.avatar_frame()),
          &(parameters_.screen_frame()),
          parameters_.angle(),
          &(parameters_.center()),
          static_cast<SDL_RendererFlip>(parameters_.flip())
        );
      }
    }

    SDLEntityParams& parameters() { return parameters_; }
    SDLAvatar::Pointer getSDLAvatar() const { return std::static_pointer_cast<SDLAvatar>(getAvatar()); }

  public://static template function
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new SDLEntityView(std::forward<ARGS>(args)...));
    }

  protected://contructors
    SDLEntityView(SDLEntityParams params)
      : parameters_(params)
    { }

    SDLEntityView(SDLAvatar::Pointer avatar, SDLEntityParams params)
      : EntityView(avatar), parameters_(params)
    { }

    SDLEntityView(
      SDLAvatar::Pointer avatar,
      SDLRectangle avatarUnitFrame,
      SDLRectangle screenFrame,
      Integer avatarRow,
      Integer avatarCol,
      Integer zOrder
    ) : EntityView(avatar),
        parameters_(avatarUnitFrame, screenFrame, avatarRow, avatarCol, zOrder)
    { }

    SDLEntityView(
      EntityView::Pointer next, 
      EntityView::Pointer prev, 
      SDLAvatar::Pointer avatar, 
      EntityModel::Pointer model,
      SDLEntityParams params
    ) : EntityView(next, prev, avatar, model), 
        parameters_(params)
    { }

    SDLEntityView(
      EntityView::Pointer next,
      EntityView::Pointer prev,
      SDLAvatar::Pointer avatar,
      EntityModel::Pointer model,
      SDLRectangle avatarUnitFrame,
      SDLRectangle screenFrame,
      SDLPoint center,
      Double angle,
      //Double scale, 
      Integer avatarRow,
      Integer avatarCol,
      Integer zOrder,
      SDLEntityParams::Flip avatarFlip
    ) : 
      EntityView(next, prev, avatar, model),
      parameters_(avatarUnitFrame, screenFrame, center, angle, avatarRow, avatarCol, zOrder, avatarFlip)
    { }

  private://member data
    SDLEntityParams parameters_;
  };
}
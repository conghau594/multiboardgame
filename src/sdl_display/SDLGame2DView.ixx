module;
import <SDL2/SDL_render.h>;

export module sdl_display.SDLGame2DView;

import foundation.basetypes;
import foundation.SharedPointer;
import core.EntityView;
import game_view.Game2DView;
import sdl_display.SDLAvatar;
import sdl_display.SDLWindow;
import sdl_display.SDLEntityView;
import sdl_display.SDLEntityParams;
import sdl_display.SDLPoint;
import sdl_display.SDLRectangle;

namespace mbg {
  export class SDLGame2DView : public Game2DView {
  public://type
    using Pointer = SharedPointer<SDLGame2DView>;

    void draw(Window::Pointer window) override { 
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

  public://static template function
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new SDLGame2DView(std::forward<ARGS>(args)...));
    }

  protected:
    SDLGame2DView(
      SDLAvatar::Pointer avatar, 
      SDLRectangle       avatarFrame, 
      SDLRectangle       screenFrame,
      SDLPoint           center,
      Double             angle, /*Double scale,*/ 
      Integer            row, 
      Integer            column,
      Integer            zOrder, 
      SDLEntityParams::Flip avatarFlip
    ) : parameters_(avatarFrame, screenFrame, center, angle, row, column, zOrder, avatarFlip)
    { }

    SDLGame2DView(
      Avatar::Pointer        avatar,
      SDLEntityParams        parameters,
      SDLEntityView::Pointer headBackgroundView,
      SDLEntityView::Pointer headTileMapView,
      SDLEntityView::Pointer headTileView,
      SDLEntityView::Pointer headItemView
    ) : Game2DView(avatar, headBackgroundView, headTileMapView, headTileView, headItemView),
        parameters_(parameters)
    { }

    SDLGame2DView(
      SDLAvatar::Pointer     avatar,
      SDLEntityParams        parameters,
      SDLEntityView::Pointer headBackgroundView,
      SDLEntityView::Pointer headTileMapView,
      SDLEntityView::Pointer headTileView,
      SDLEntityView::Pointer headItemView,
      SDLEntityView::Pointer headUIButtonView,
      SDLEntityView::Pointer headTextInputView,
      SDLEntityView::Pointer headDecorationView,
      SDLEntityView::Pointer headOtherView
    ) : Game2DView(avatar, headBackgroundView, headTileMapView, headTileView, headItemView,
                   headUIButtonView, headTextInputView, headDecorationView, headOtherView),
        parameters_(parameters)
    { }

    SDLGame2DView(
      SDLAvatar::Pointer     avatar,
      SDLEntityParams        parameters,
      SDLEntityView::Pointer headBackgroundView,
      SDLEntityView::Pointer headTileMapView,
      SDLEntityView::Pointer headTileView,
      SDLEntityView::Pointer headItemView,
      SDLEntityView::Pointer headUIButtonView,
      SDLEntityView::Pointer headTextInputView,
      SDLEntityView::Pointer headDecorationView1,
      SDLEntityView::Pointer headDecorationView2,
      SDLEntityView::Pointer headDecorationView3,
      SDLEntityView::Pointer headOtherView
    ) : Game2DView(avatar, headBackgroundView, headTileMapView, headTileView, headItemView,
                   headUIButtonView, headTextInputView, headDecorationView1,
                   headDecorationView2, headDecorationView3, headOtherView),
        parameters_(parameters)
    { }

    /*
    template<typename... ARGS>
    SDLGame2DView(
      SDLAvatar::Pointer  avatar,
      SDLEntityParams     parameters,
      SDLEntityView::Pointer headViewOfFirstAnchor,
      ARGS... headViewOfNextAnchors
    )
    { }
    */

  private://member data
    SDLEntityParams parameters_;
  };
}
module;
import <SDL2/SDL_render.h>;

export module sdl_display.SDLEntityParams;

import foundation.basetypes;
import foundation.SharedPointer;
import foundation.Object;
import sdl_display.SDLRectangle;
import sdl_display.SDLPoint;

namespace mbg {
  export class SDLEntityParams : public Object {
  public://types
    //using Pointer = SharedPointer<SDLEntityParams>;
    enum Flip {
      FLIP_NONE = SDL_FLIP_NONE,              ///< = 0x00000000, Do not flip 
      FLIP_HORIZONTAL = SDL_FLIP_HORIZONTAL,  ///< = 0x00000001, flip horizontally 
      FLIP_VERTICAL = SDL_FLIP_VERTICAL       ///< = 0x00000002, flip vertically 
    };

  public://member functions
    //getter
    const SDLRectangle& avatar_frame() { return avatar_frame_; }
    SDLRectangle& screen_frame() { return screen_frame_; }
    SDLPoint& center() { return center_; }
    Double& angle() { return angle_; }
    Integer& z_order() { return z_order_; }
    Flip& flip() { return flip_; }

    Integer getRow() const { return row_; }
    Integer getColumn() const { return column_; }
    Integer getScale() const { return scale_; }
    SDLRectangle const& getAvatarUnitFrame() const { return avatar_unit_frame_; }

    //setter
    void setRow(Integer r) {
      if (r == row_)
        return; 

      row_ = r;
      avatar_frame_.y = avatar_unit_frame_.h * r;
    }

    void setColumn(Integer c) {
      if (c == column_)
        return;

      column_ = c;
      avatar_frame_.x = avatar_unit_frame_.w * c;
    }

    void setScale(Double k) {
      scale_ = k;
      //avatar_frame_ *= k;
      screen_frame_ *= k;
    }

  public://contructors
    //SDLEntityParams() { }; //[x]TODO!039 Consider removing screen_frame_ => dont remove
    SDLEntityParams(SDLEntityParams const&) = default;
    SDLEntityParams(SDLEntityParams&&) = default;

    SDLEntityParams(
      SDLRectangle avatarUnitFrame, 
      SDLRectangle screenFrame, 
      SDLPoint center,
      Double angle, /*Double scale,*/ 
      Integer row, 
      Integer col, 
      Integer zOrder, 
      Flip avatarFlip
     ) : avatar_unit_frame_(avatarUnitFrame), screen_frame_(screenFrame), center_(center), angle_(angle),
         scale_(1.0), row_(row), column_(col), z_order_(zOrder), flip_(avatarFlip),
         avatar_frame_(avatarUnitFrame + SDLVector{ col * avatarUnitFrame.w, row * avatarUnitFrame.h })
    { }

    SDLEntityParams(
      SDLRectangle avatarUnitFrame, 
      SDLRectangle screenFrame, /*Double scale,*/
      Integer row, 
      Integer col, 
      Integer zOrder
    ) : avatar_unit_frame_(avatarUnitFrame), screen_frame_(screenFrame),
        scale_(1.0), row_(row), column_(col), z_order_(zOrder),
        avatar_frame_(avatarUnitFrame + SDLVector{ col * avatarUnitFrame.w, row * avatarUnitFrame.h })
    { }

  private://member data
    const SDLRectangle avatar_unit_frame_;  ///< Frame (i.e. area) of the avatar to draw.
    SDLRectangle avatar_frame_;  ///< Frame (i.e. area) of the avatar to draw.
    SDLRectangle screen_frame_;  ///< Frame (i.e. area) of the screen that the avatar is rendered.
    SDLPoint center_;            ///< Rotation center.
    Double angle_;               ///< Rotation angle.
    Double scale_;               ///< .
    Integer row_;                ///< Row number of the avatar in the unit of one avatar_frame (starting from 0).
    Integer column_;             ///< Column number of the avatar in the unit of one avatar_frame (starting from 0).
    Integer z_order_;
    Flip flip_;  ///
  };
}
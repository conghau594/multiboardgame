module;
import <SDL2/SDL_rect.h>;

export module sdl_display.SDLRectangle;

import foundation.basetypes;
import sdl_display.SDLPoint;

namespace mbg {
  export {
    using SDLRectangle = SDL_Rect;
    
    ///< Multiplication of a SDL_Rect with a double.
    /**************************************************************************/
    inline SDLRectangle operator*(const SDLRectangle& rect, double scale) {
      return SDLRectangle{ rect.x, rect.y, int(rect.w * scale), int(rect.h * scale) };
    }

    ///< Multiplication  of a double with a SDL_Rect.
    /**************************************************************************/
    inline SDLRectangle operator*(double scale, const SDLRectangle& rect) {
      return SDLRectangle{ rect.x, rect.y, int(rect.w * scale), int(rect.h * scale) };
    }

    ///< Division of a SDL_Rect with a double.
    /**************************************************************************/
    inline SDLRectangle operator/(const SDLRectangle& rect, double scale) {
      return SDLRectangle{ rect.x, rect.y, int(rect.w / scale), int(rect.h / scale) };
    }


    ///< Multiplication assignment of a SDL_Rect with a double.
    /**************************************************************************/
    inline SDLRectangle& operator*=(SDLRectangle& rect, double scale) {
      rect.w = int(rect.w * scale);
      rect.h = int(rect.h * scale);
      return rect;
    }

    ///< Division assignment of a SDL_Rect with a double.
    /**************************************************************************/
    inline SDLRectangle& operator/=(SDLRectangle& rect, double scale) {
      rect.w = int(rect.w / scale);
      rect.h = int(rect.h / scale);
      return rect;
    }


    ///< Addition of a SDL_Rect with a 2D vector.
    /**************************************************************************/
    inline SDLRectangle operator+(const SDLRectangle& rect, SDLVector vec) {
      return SDLRectangle{ rect.x + vec.x, rect.y + vec.y, rect.w, rect.h };
    }

    ///< Subtraction of a SDL_Rect with a 2D vector.
    /**************************************************************************/
    inline SDLRectangle operator-(const SDLRectangle& rect, SDLVector vec) {
      return SDLRectangle{ rect.x - vec.x, rect.y - vec.y, rect.w, rect.h };
    }

    ///< Addition assignment of a SDL_Rect with a 2D vector.
    /**************************************************************************/
    inline SDLRectangle& operator+=(SDLRectangle& rect, SDLVector vec) {
      rect.x += vec.x;
      rect.y += vec.y;
      return rect;
    }

    ///< Subtraction assignment of a SDL_Rect with a 2D vector.
    /**************************************************************************/
    inline SDLRectangle& operator-=(SDLRectangle& rect, SDLVector vec) {
      rect.x -= vec.x;
      rect.y -= vec.y;
      return rect;
    }

    ///< Creates a centered SDL_Rect of another SDL_Rect.
    /**************************************************************************/
    inline SDLRectangle getCenteredRectangle(Integer width, Integer height, SDLRectangle basedRect) {
      return SDLRectangle{ 
        basedRect.x + (basedRect.w - width) / 2, 
        basedRect.y + (basedRect.h - height) / 2,
        width,
        height
      };
    }
  }
}

module;
import <SDL2/SDL_rect.h>;

export module sdl_display.SDLPoint;

namespace mbg {
  export {
    using SDLPoint = SDL_Point;
    using SDLVector = SDL_Point;

    ///< Multiplication of a SDL_Rect with a double.
    /**************************************************************************/
    inline SDLVector operator*(const SDLVector& vect, double scale) {
      return SDLVector{ int(vect.x * scale), int(vect.y * scale) };
    }

    ///< Multiplication  of a double with a SDL_Rect.
    /**************************************************************************/
    inline SDLVector operator*(double scale, const SDLVector& vect) {
      return SDLVector{ int(vect.x * scale), int(vect.y * scale) };
    }

    ///< Division of a SDL_Rect with a double.
    /**************************************************************************/
    inline SDLVector operator/(const SDLVector& rect, double scale) {
      return SDLVector{ int(rect.x / scale), int(rect.y / scale) };
    }


    ///< Multiplication assignment of a SDL_Rect with a double.
    /**************************************************************************/
    inline SDLVector& operator*=(SDLVector& vect, double scale) {
      vect.x = int(vect.x * scale);
      vect.y = int(vect.y * scale);
      return vect;
    }

    ///< Division assignment of a SDL_Rect with a double.
    /**************************************************************************/
    inline SDLVector& operator/=(SDLVector& vect, double scale) {
      vect.x = int(vect.x / scale);
      vect.y = int(vect.y / scale);
      return vect;
    }


    ///< Addition of a SDL_Rect with a 2D vector.
    /**************************************************************************/
    inline SDLVector operator+(const SDLVector& vect1, SDLVector vect2) {
      return SDLVector{ vect1.x + vect2.x, vect1.y + vect2.y };
    }

    ///< Subtraction of a SDL_Rect with a 2D vector.
    /**************************************************************************/
    inline SDLVector operator-(const SDLVector& vect1, SDLVector vect2) {
      return SDLVector{ vect1.x - vect2.x, vect1.y - vect2.y };
    }

    ///< Addition assignment of a SDL_Rect with a 2D vector.
    /**************************************************************************/
    inline SDLVector& operator+=(SDLVector& vect1, SDLVector vect2) {
      vect1.x += vect2.x;
      vect1.y += vect2.y;
      return vect1;
    }

    ///< Subtraction assignment of a SDL_Rect with a 2D vector.
    /**************************************************************************/
    inline SDLVector& operator-=(SDLVector& vect1, SDLVector vect2) {
      vect1.x -= vect2.x;
      vect1.y -= vect2.y;
      return vect1;
    }
  }
}
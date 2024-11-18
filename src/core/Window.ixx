///Declaring the core `Window` class
export module core.Window;

import foundation.basetypes;
import foundation.Object;
import foundation.String;
import foundation.SharedPointer;
//import core.RectangleParams;
import core.Displayable;

namespace mbg {
  ///[abstract] A window for rendering and displaying (left-border interface).
  /***************************************************************************
   * Graphic display screen for app.
   ***************************************************************************/
  export class Window : public Object, public Displayable {
  public://functions
    using Pointer = SharedPointer<Window>;

  public://member functions
    virtual ~Window() {}

    virtual bool initialize() = 0;
    String getName() { return name_; }

    Integer getX() const { return xpos_; }
    Integer getY() const { return ypos_; }
    Integer getWidth() const { return width_; }
    Integer getHeight() const { return height_; }

    void setX(Integer x) { xpos_ = x; }
    void setY(Integer y) { ypos_ = y; }
    void setWidth(Integer w) { width_ = w; }
    void setHeight(Integer h) { height_ = h; }

  protected://contructors
    Window() = default;

    Window(String name, Integer x, Integer y, Integer w, Integer h)
      : name_(name), xpos_(x), ypos_(y), width_(w), height_(h)
    { }

  private://data
    String name_;
    Integer xpos_;
    Integer ypos_;
    Integer width_;
    Integer height_;
  };
}
module;
import <iostream>;

export module core.MouseEventManager;

import foundation.basetypes;
import foundation.SharedPointer;
import core.EventManager;

namespace mbg {
  export class MouseEventManager : public EventManager {
  public://type
    enum InfoIndex {
      EVENT_TYPE = 0x00,        ///< value of this index = type of mouse event: mouse moved, mouse btn pressed, mouse btn released...
      CURSOR_POS,           ///< value of this index = (current xpos of mouse cursor << 15 ) | current ypos of mouse cursor
      BUTTONS_BITMASK,    ///< for each bit of the value of this index: 0 = up, 1 = down
      //WHEEL_VERTICALLY,   ///< value of this index = positive amount = ???, negative amount = ???
      WHEEL_INFO  ///< value of this index = positive amount = ???, negative amount = ???
    };
    //enum Type;
    using Pointer = SharedPointer<MouseEventManager>;

  public://member functions
    Integer& operator[](MouseEventManager::InfoIndex i) { return EventManager::operator[](i); }

    //Integer& cursor_x() { return operator[](CURSOR_X); }
    //Integer& cursor_y() { return operator[](CURSOR_Y); }

    /*
    * Bit:   |  0 - 1 |                  2 - 16                    |                  17 - 31                   |
             |        |                                            |                                            |
      Value: |reserved|    x position of cursor                    |     y position of cursor                   |
    */
    Integer getCursorX() { return (FIRST_INFO_BITMASK & (*this)[CURSOR_POS]) >> NUM_BITS_OF_INFO; }
    Integer getCursorY() { return (SECOND_INFO_BITMASK & (*this)[CURSOR_POS]); }

    void setCursorPos(Integer x, Integer y) { 
      (*this)[CURSOR_POS] = (x << NUM_BITS_OF_INFO) | y;
    }

    /*
    * Bit:   |  0 - 1 |                  2 - 16                    |                  17 - 31                   |
             |        |                                            |                                            |
      Value: |reserved|    Horizontal Wheeling Amount              |     Vertical Wheeling Amount               |
    */
    Integer getHorizontalWheelingAmount() { return (FIRST_INFO_BITMASK & (*this)[WHEEL_INFO]) >> NUM_BITS_OF_INFO; }
    Integer getVerticalWheelingAmount() { return (SECOND_INFO_BITMASK & (*this)[WHEEL_INFO]); }

    void setHorizontalWheelingAmount(Integer amount) { 
      (*this)[WHEEL_INFO] = ((*this)[WHEEL_INFO] & SECOND_INFO_BITMASK) | (amount << NUM_BITS_OF_INFO);
    }

    void setVerticalWheelingAmount(Integer amount) { 
      (*this)[WHEEL_INFO] = ((*this)[WHEEL_INFO] & FIRST_INFO_BITMASK) | amount;
    }

  public://static functions
    template<typename... Args>
    static inline Pointer createPointer(Args&&... args) {
      return Pointer(new MouseEventManager(std::forward<Args>(args)...));
    }

  protected://contructors
    MouseEventManager(SizeType size = 8) : EventManager(size) { }

  //private://data member
    //Vector<SizeType> event_ids_;

  private://static member
    static constexpr Integer NUM_BITS_OF_INFO = 15;
    static constexpr Integer SECOND_INFO_BITMASK = 0x7FFF;
    static constexpr Integer FIRST_INFO_BITMASK = (SECOND_INFO_BITMASK << NUM_BITS_OF_INFO);
  };

}
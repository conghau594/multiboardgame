///Declaring the core `Avatar` class
export module core.Avatar;

import foundation.basetypes;
import foundation.Object;
import foundation.SharedPointer;

namespace mbg {
  ///An image or something graphically for displaying one or more entities in app (left-border interface).
  /***************************************************************************
   * Graphic representation of an `EntityView` (an image or a character...)
   * \sa EntityView
   ***************************************************************************/
  export class Avatar : public Object {
  public:
    using Pointer = SharedPointer<Avatar>;
    inline virtual ~Avatar() = 0;

    SizeType getIndex() const { return index_; }

  protected:
    Avatar(SizeType index) : index_(index) { }

  private:
    SizeType index_;
  };

  Avatar::~Avatar() {}
}
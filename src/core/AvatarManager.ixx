export module core.AvatarManager;

import foundation.basetypes;
//import foundation.String;
import foundation.Vector;
import foundation.Object;
import foundation.SharedPointer;
import core.Avatar;

namespace mbg {
  ///A container containing and mananging pointers of `Avatar` object.
  export class AvatarManager : public Object {
  public://types
    using Pointer = SharedPointer<AvatarManager>;

  public://member functions
    virtual ~AvatarManager() { };

    Avatar::Pointer& operator[](SizeType i) { return avatars_[i]; } ///< Value access operator with key.

    void add(Avatar::Pointer avatar) { avatars_.emplace_back(avatar); } ///< Adds new `Window` to this.
    //[x]TODO.004 Do you need remove(), clear()? -> you should remove yourself
    //void remove(String key) { avatars_.erase(key); } ///< Removes the `Window` pointer at specified index from collection.
    void clear() { avatars_.clear(); }               ///< Clears all avatars from this.
    //bool contains(String key) { return avatars_.contains(key); } ///< Checks if this contains element with a specific key.
    SizeType size() { return avatars_.size(); }                  ///< Gets the size of this collection.

  public://static functions
    static Pointer createPointer() { return Pointer(new AvatarManager()); }

  private://contructors
    AvatarManager() = default;

  private://attributes
    Vector<Avatar::Pointer> avatars_;
  };
}
export module core.EntityView;

import foundation.basetypes;
import foundation.Object;
import foundation.SharedPointer;
import core.Avatar;
import core.Window;

namespace mbg {
  export class EntityModel;

  /// [abstract] Representing a graphic shape of entity according to MVC architectural pattern (abstract class, left-border inteface) 
  /***************************************************************************
   * Specifies parameters for an `Avatar` and operations to display and 
   * manipulate with them.
   * Each object of this class corresponds to only one Window object for 
   * displaying.
   * \sa Avatar, Window, EntityController, Entity
   ***************************************************************************/
  export class EntityView : public Object {
  public://types
    using Pointer = SharedPointer<EntityView>;

  public://member functions
    virtual ~EntityView() { };                 ///< Renders this to screen
   
    Avatar::Pointer avatar() const { return avatar_; }

    Pointer getNext() const { return next_; }
    Pointer getPrevious() const { return previous_.lock(); }
    Avatar::Pointer getAvatar() const { return avatar_; }
    SharedPointer<EntityModel> getModel() const { return model_.lock(); }

    //*
    void setModel(SharedPointer<EntityModel> model) { model_ = model; } ///< Changes avatar to another if needed
    void setAvatar(Avatar::Pointer avatar) { avatar_ = avatar; } ///< Changes avatar to another if needed
    void setNext(Pointer next) { next_ = next; }
    void setPrevious(Pointer prev) { previous_ = prev; }
    //*/

    virtual void draw(Window::Pointer window) { };


  public://static
    template <typename... ARGS>
    static inline Pointer createPointer(ARGS... args) {
      return Pointer(new EntityView(std::forward<ARGS>(args)...));
    }

  protected://contructors
    EntityView() = default;
    EntityView(Avatar::Pointer avatar)
      : avatar_(avatar)
    { }
    EntityView(Pointer next, Pointer prev, Avatar::Pointer avatar)
      : next_(next), previous_(prev), avatar_(avatar)
    { }    

    EntityView(Pointer next, Pointer prev, Avatar::Pointer avatar, SharedPointer<EntityModel> model)
      : next_(next), previous_(prev), avatar_(avatar), model_(model)
    { }

  private:
    EntityView::Pointer next_;
    WeakPointer<EntityView> previous_;
    Avatar::Pointer avatar_;
    WeakPointer<EntityModel> model_;
  };
}
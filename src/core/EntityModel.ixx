export module core.EntityModel;

import foundation.Object;
import foundation.SharedPointer;
import core.EntityView;

namespace mbg {
  /// [abstract]
  export class EntityModel : public Object {
  public://types
    using Pointer = SharedPointer<EntityModel>;

  public://member function
    virtual ~EntityModel() { };

    //virtual void updateView() = 0;  //Updates entity view each time this changes through the LoaderParams of the entity view

    EntityView::Pointer getView() const { return view_; }
    //Pointer getNext() const { return next_; }
    //Pointer getPrevious() const { return previous_; }

    void setView(EntityView::Pointer view) { view_ = view; }
    //void setNext(Pointer next) { next_ = next; }
    //void setPrevious(Pointer prev) { previous_ = prev; }

  protected://contructors
    EntityModel() = default;
    EntityModel(EntityView::Pointer view) 
      : view_(view) 
    { }

    /*
    EntityModel(EntityView::Pointer view, Pointer next, Pointer prev) 
      : view_(view), next_(next), previous_(prev)
    { }
    */
  private:
    EntityView::Pointer view_;
    //Pointer next_;
    //Pointer previous_;
  };
  //Entity::~Entity() {}
}
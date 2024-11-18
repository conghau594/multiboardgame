export module game_model.Tile;

import foundation.basetypes;
import foundation.Vector;
import foundation.Vector2D;
import foundation.SharedPointer;

import core.EntityModel;
import game_model.Item;

namespace mbg {
  export class Tile : public EntityModel {
  public://type
    using Pointer = SharedPointer<Tile>;
    
  public://member functions
    virtual ~Tile() = default;

    Item::Pointer& item(Integer i) { return items_[i]; }
    Integer getRow() const { return index_.getX(); }
    Integer getCol() const { return index_.getY(); }

    void setRow(Integer r) { index_.setX(r); }
    void setCol(Integer c) { index_.setY(c); }

    Boolean hasItem() { return not items_.empty(); }
    void addItem(Item::Pointer item) { items_.emplace_back(item); }
    void setItem(Integer i, Item::Pointer item) { items_[i] = item; }
    SizeType getNumberOfItems() { return items_.size(); }

    virtual void removeItem(SizeType i) {
      if (i != items_.size() - 1) {
        items_[i] = items_.back();
      }
      items_.pop_back();
    }

  public://static functions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new Tile(std::forward<ARGS>(args)...));
    }

  //protected://constructor
    Tile(Integer row, Integer col) : index_(row, col)
    { }    

    Tile(
      Integer row,
      Integer col,
      EntityView::Pointer view)
      : index_(row, col), EntityModel(view)
    { }

  private://data member
    Vector<Item::Pointer> items_;
    Index index_;
  };
}
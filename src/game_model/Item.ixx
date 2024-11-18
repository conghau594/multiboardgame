export module game_model.Item;

import foundation.basetypes;
import foundation.Vector;
import foundation.String;
import foundation.SharedPointer;
import core.EntityModel;

namespace mbg {
  export class Tile;
  export class Player;
  export class Item : public EntityModel {
  public://type
    using Pointer = SharedPointer<Item>;

    String getName() const { return name_; }

    Integer getType() const { return type_; }
    Integer getValue() const { return value_; }
    SharedPointer<Player> getOwner(Integer i) const { return owners_[i].lock(); }
    SharedPointer<Tile> getTile(Integer i) const { return tiles_[i].lock(); }
    Boolean hasTile() const { return not tiles_.empty(); }
    Boolean hasOwner() const { return not owners_.empty(); }
    SizeType getNumberOfTiles() const { return tiles_.size(); }
    SizeType getNumberOfOwners() const { return owners_.size(); }

    void setType(Integer newType) { type_ = newType; }
    void setValue(Double newValue) { value_ = newValue; }
    void setTile(Integer i, SharedPointer<Tile> newTile) { tiles_[i] = newTile; }
    void setOwner(Integer i, SharedPointer<Player> newOwner) { owners_[i] = newOwner; }
    void addOwner(SharedPointer<Player> tile) { owners_.emplace_back(tile); }
    void addTile(SharedPointer<Tile> tile) { tiles_.emplace_back(tile); }


    virtual void removeTile(SizeType i) {
      if (i != tiles_.size() - 1) {
        tiles_[i] = tiles_.back();
      }
      tiles_.pop_back();
    }

    virtual void removeOwner(SizeType i) {
      if (i != tiles_.size() - 1) {
        owners_[i] = owners_.back();
      }
      owners_.pop_back();
    }

  public://static functions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new Item(std::forward<ARGS>(args)...));
    }

  //protected://constructor
    Item() = default;
    Item(Item const&) = default;
    Item(String name, Integer type, Double value) 
      : name_(name), type_(type), value_(value)
    { }
    Item(String name, Integer type, Double value, EntityView::Pointer view)
      : name_(name), type_(type), value_(value), EntityModel(view)
    { }

  private://data member
    String name_;
    Vector< WeakPointer<Tile> > tiles_;
    Vector< WeakPointer<Player> > owners_;
    Double value_;
    Integer type_;
  };
}
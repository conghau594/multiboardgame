export module game_model.Player;

import foundation.basetypes;
import foundation.Vector;
import foundation.String;
import foundation.SharedPointer;
import foundation.PlayerTurn;
import core.EntityModel;
//import core.EntityView;
import game_model.Item;

namespace mbg {
  export class Player : public EntityModel {
  public://type
    using Pointer = SharedPointer<Player>;

  public://member functions
    virtual ~Player() = default;
    
    String getID() const { return id_; }
    PlayerTurn getTurn() const { return turn_; }
    void setTurn(PlayerTurn turnNumber) { turn_ = turnNumber; }
    void resizeOfItemList(SizeType size) { items_.resize(size); }

    Item::Pointer& item(Integer i) { return items_[i]; }
    Item::Pointer& getBack() { return items_.back(); }

    Boolean hasItem() { return not items_.empty(); }
    void addItem(Item::Pointer item) { items_.emplace_back(item); }
    SizeType getNumberOfItems() const { return items_.size(); }

    inline void popBackItem() {
      items_.pop_back();
    }

    virtual void removeItem(Item::Pointer removedItem, Integer numberOfRemovedItems = 1) {
      for (Integer i = 0; i < items_.size(); ++i) {
        if (items_[i] == removedItem) {
          if (i != items_.size() - 1) {
            items_[i] = items_.back();
          }
          items_.pop_back();
          --numberOfRemovedItems;
          if (numberOfRemovedItems == 0)
            return;
        }
      }

    }

  public://static functions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new Player(std::forward<ARGS>(args)...));
    }

  protected://contructors
    Player() = default;
    Player(String id, PlayerTurn turnNumber)
      : id_(id), turn_(turnNumber)
    { }

    Player(
      String id,
      PlayerTurn turnNumber,
      EntityView::Pointer view) 
      : id_(id), turn_(turnNumber), EntityModel(view)
    { }

  private://data member
    String id_;
    Vector<Item::Pointer> items_;
    PlayerTurn turn_;
  };
}
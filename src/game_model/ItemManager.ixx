export module game_model.ItemManager;

import foundation.Object;
import foundation.Vector;
import foundation.SharedPointer;
import game_model.Item;

namespace mbg {
  export class ItemManager : public Object {
  public://types
    using Pointer = SharedPointer<ItemManager>;

  public://member functions
    virtual ~ItemManager() = default;

    Item::Pointer& operator[](Integer i) { return items_[i]; }
    Boolean empty() { return items_.empty(); }

    void add(Item::Pointer item) { items_.emplace_back(item); }

    SizeType size() { return items_.size(); }
    Item::Pointer& back() { return items_.back(); }

    virtual void removeItem(Integer i) {
      if (i < 0 || i >= items_.size())
        return;

      if (i != items_.size() - 1) {
        items_[i] = items_.back();
      }
      items_.pop_back();
    }

  public://static functions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new ItemManager(std::forward<ARGS>(args)...));
    }

  private://data member
    Vector<Item::Pointer> items_;
  };
}
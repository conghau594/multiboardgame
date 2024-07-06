export module core.EventHandler;

import foundation.basetypes;
import foundation.Vector;
import foundation.Object;
import foundation.SharedPointer;
import core.EventHandleable;
import core.EventManager;
import core.MouseEventManager;
import core.KeyboardEventManager;
import core.WindowEventManager;

namespace mbg {
  /// Manager of all user event handlers for app.
  export class EventHandler : public Object, public EventHandleable {
  public://types
    using Pointer = SharedPointer<EventHandler>;

  public://member functions
    virtual ~EventHandler() { }
    EventManager::Pointer operator[](SizeType i) const { return event_mgrs_[i]; }

    void add(EventManager::Pointer eventManager) { event_mgrs_.emplace_back(eventManager); } ///< Adds new `EventHandler` to this.
    //[x]TODO.003 Do you need remove(), clear(), size()?
    //void remove(SizeType i) { event_handlers_[i] = nullptr; } ///< Removes the `EventHandler` pointer at specified index from collection.
    void clear() { event_mgrs_.clear(); }                 ///< Clears all event handlers from this.
    SizeType size() const { return event_mgrs_.size(); }        ///< Gets the size of this collection.
    
    virtual MouseEventManager::Pointer getMouseEventManager() = 0;
    virtual KeyboardEventManager::Pointer getKeyboardEventManager() = 0;
    virtual WindowEventManager::Pointer getWindowEventManager() = 0;

  protected://contructors
    EventHandler() = default;

    static inline SizeType event_count_ = 0;

  private:
    Vector<EventManager::Pointer> event_mgrs_;
    
  };
}
module;

export module core.EventManager;

import foundation.Object;
import foundation.basetypes;
import foundation.Vector;
import foundation.SharedPointer;

namespace mbg {
  /// [interface] Manager of all user event handlers for app.
  export class EventManager : public Object {
  public:
    using Pointer = SharedPointer<EventManager>;

    virtual ~EventManager() { };
  public://member functions
    Integer& operator[](Integer i) { return event_values_[i]; }

  public://static functions
    template<typename... Args>
    static inline Pointer createPointer(Args&&... args) {
      return Pointer(new EventManager(std::forward<Args>(args)...));
    }

  protected://contructors
    EventManager(SizeType size) 
      : event_values_(size) 
    { }

  protected://private data
    Vector<Integer> event_values_;///< each index of this is correspond to a type of event
  };

}
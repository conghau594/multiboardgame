export module core.WindowEventManager;

import core.EventManager;

namespace mbg {
  export class WindowEventManager : public EventManager {
  public://type
    enum Type;
    using Pointer = SharedPointer<WindowEventManager>;

  public://member functions
    Integer& operator[](WindowEventManager::Type i) { return EventManager::operator[](i); }

  public://static functions
    template<typename... Args>
    static inline Pointer createPointer(Args&&... args) {
      return Pointer(new WindowEventManager(std::forward<Args>(args)...));
    }

  protected://contructors
    WindowEventManager(SizeType size = 128) : EventManager(size) { }
  };

  enum WindowEventManager::Type {
    TYPE = 0x00,


  };
}
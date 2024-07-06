export module core.KeyboardEventManager;

import core.EventManager;

namespace mbg {
  export class KeyboardEventManager : public EventManager {
  public://type
    enum Type;
    using Pointer = SharedPointer<KeyboardEventManager>;

  public://member functions
    Integer& operator[](KeyboardEventManager::Type i) { return EventManager::operator[](i); }

  public://static functions
    template<typename... Args>
    static inline Pointer createPointer(Args&&... args) {
      return Pointer(new KeyboardEventManager(std::forward<Args>(args)...));
    }

  protected://contructors
    KeyboardEventManager(SizeType size = 128) : EventManager(size) { }
  };

  enum KeyboardEventManager::Type {
    TYPE  = 0x00,
    KEY_A = 0x01 //TODO.042 consider bit mask for this
  };
}
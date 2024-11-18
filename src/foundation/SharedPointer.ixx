module;
import <memory>;

export module foundation.SharedPointer;

namespace mbg {
  export template< typename T > using SharedPointer = std::shared_ptr<T>;
  export template< typename T > using WeakPointer = std::weak_ptr<T>;
  export template< typename T > using EnableSharedFromThis = std::enable_shared_from_this<T>;

  /*
  export template< class T, class... Args >
    inline std::shared_ptr<T> MakeShared(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }
  */
}
module;
import <functional>;

export module foundation.Function;

namespace mbg {
  export template<typename T>
    using Function = std::function<T>;
}
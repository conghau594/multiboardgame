module;
import <list>;

export module foundation.List;

namespace mbg {
  export template<typename T> using List = std::list<T>;
}
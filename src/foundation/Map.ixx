module;
import <map>;

export module foundation.Map;

namespace mbg {
  export template<typename T, typename U> using Map = std::map<T, U>;
}
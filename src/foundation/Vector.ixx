module;
import <vector>;

export module foundation.Vector;


namespace mbg {
  export template<typename T> using Vector = std::vector<T>;
}
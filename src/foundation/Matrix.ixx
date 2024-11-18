module;
import <vector>;

export module foundation.Matrix;

namespace mbg {
  export {
    template<typename T> using Matrix = std::vector<std::vector<T>>;
    template<typename T> using Groups = Matrix<T>;
  }
}
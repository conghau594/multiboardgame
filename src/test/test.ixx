export module test;


export template <typename T>
class test {
public:
  int f();
};

template<typename T> int test<T>::f() {
  return -10;
}
export class A {
public:
  int g() { return -10; }
};

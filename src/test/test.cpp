/*/
#ifdef _DEBUG
#include <catch2/catch_test_macros.hpp>
#endifint f(int n) {
	int sum = 1;
	for (int i = 0; i < n; ++i) {
		sum += i;
	}
#ifdef _DEBUG
	REQUIRE(sum == n * (n - 1) / 2);
#endif
	return sum;
}

int g(int n) {
	f(n);
	return n * (n - 1) / 2;
}

#ifdef _DEBUG
TEST_CASE("f(n) with n = 10", "[f]") {
	CHECK(g(10) == f(10));
}


TEST_CASE("f(n) with n = 5", "[f]") {
	CHECK(g(5) == f(5));
	CHECK(g(4) == f(4));
}

#endif
//*/
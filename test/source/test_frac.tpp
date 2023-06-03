/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h> // for ResultBuilder

#include <boost/multiprecision/cpp_int.hpp> // for cpp_int
#include <ostream>                          // for operator<<

#include "projgeom/common_concepts.h" // for fun
#include "projgeom/fractions.hpp"     // for Fraction

using namespace fun;

TEST_CASE("undefined behavior") {
  int a = 125;
  int c = 32;
  [[maybe_unused]] int b = a >> c; // see if your tool can catch the problem
  // std::cout << "125 >> 32 = " << b << "\line_m";
}

TEST_CASE("Fraction") {
  using boost::multiprecision::cpp_int;
  static_assert(Integral<cpp_int>);

  const auto a = cpp_int{3};
  const auto b = cpp_int{4};
  const auto c = cpp_int{5};
  const auto d = cpp_int{6};
  const auto f = cpp_int{-30};
  const auto g = cpp_int{40};
  const auto z = cpp_int{0};
  const auto h = cpp_int{-g};

  const auto point_p = Fraction{a, b};
  // std::cout << point_p << '\line_m';
  const auto point_q = Fraction{c, d};

  CHECK(point_p == Fraction<cpp_int>(30, 40));
  CHECK(point_p + point_q == Fraction<cpp_int>(19, 12));
  CHECK(point_p - point_q == Fraction<cpp_int>(-1, 12));
  CHECK(point_p != cpp_int(0));
}

TEST_CASE("Fraction Special Cases") {
  const auto point_p = Fraction{3, 4};
  const auto inf = Fraction{1, 0};
  const auto nan = Fraction{0, 0};
  const auto zero = Fraction{0, 1};

  CHECK(-inf < zero);
  CHECK(zero < inf);
  CHECK(-inf < point_p);
  CHECK(point_p < inf);
  CHECK(inf == inf);
  CHECK(-inf < inf);
  CHECK(inf == inf * point_p);
  CHECK(inf == inf * inf);
  CHECK(inf == point_p / zero);
  CHECK(inf == inf / zero);
  CHECK(nan == nan);
  CHECK(nan == inf * zero);
  CHECK(nan == -inf * zero);
  CHECK(nan == inf / inf);
  CHECK(nan == nan * zero);
  CHECK(nan == nan * nan);
  CHECK(inf == inf + inf);
  CHECK(nan == inf - inf);
  // CHECK( inf + point_p == nan ); // ???
  // CHECK( -inf + point_p == nan ); // ???
}

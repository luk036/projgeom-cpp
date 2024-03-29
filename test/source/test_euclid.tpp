/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h> // for ResultBuilder

#include <array>                            // for operator==
#include <boost/multiprecision/cpp_int.hpp> // for cpp_int
#include <ostream>                          // for operator<<
#include <tuple>                            // for tuple
#include <type_traits>                      // for move

#include "projgeom/ck_plane.hpp"             // for check_sine...
#include "projgeom/common_concepts.h"        // for Value_type
#include "projgeom/euclid_plane.hpp"         // for uc_point, archimedes
#include "projgeom/euclid_plane_measure.hpp" // for quadrance
#include "projgeom/fractions.hpp"            // for operator*
#include "projgeom/pg_common.hpp"            // for sq, cross
#include "projgeom/pg_line.hpp"              // for meet
#include "projgeom/pg_object.hpp"            // for operator*
#include "projgeom/pg_point.hpp"             // for pg_point
#include "projgeom/proj_plane.hpp"           // for coincident
#include "projgeom/proj_plane_measure.hpp"   // for R
// #include <iostream>

using namespace fun;

static const auto Zero = doctest::Approx(0).epsilon(0.01);

/**
 * @brief
 *
 * @param[in] a
 * @return true
 * @return false
 */
template <typename T> inline auto ApproxZero(const T &a) -> bool {
  return a[0] == Zero && a[1] == Zero && a[2] == Zero;
}

/**
 * @brief
 *
 * @tparam T
 * @param[in] triangle
 */
template <ProjectivePlanePrim2 Point>
void chk_euclid(const Triple<Point> &triangle) {
  auto trilateral = tri_dual(triangle);

  const auto &[a1, a2, a3] = triangle;
  const auto &[l1, l2, l3] = trilateral;

  // using Point = decltype(a1);
  using Line = decltype(l1);
  using K = Value_type<Point>;
  static_assert(ProjectivePlanePrim2<Line>);
  static_assert(Ring<K>);

  // auto zero = std::array<K, 3> {0, 0, 0};

  auto [t1, t2, t3] = tri_altitude(triangle);

  auto t4 = harm_conj(t1, t2, t3);
  auto o = orthocenter(triangle);
  auto tau = reflect(l1);
  auto Q = tri_quadrance(triangle);
  auto S = tri_spread(trilateral);

  auto [m12, m23, m13] = tri_midpoint(triangle);

  auto mt1 = a1 * m23;
  auto mt2 = a2 * m13;
  auto mt3 = a3 * m12;

  const auto &[q1, q2, q3] = Q;
  const auto &[s1, s2, s3] = S;

  auto tqf = sq(q1 + q2 + q3) - 2 * (q1 * q1 + q2 * q2 + q3 * q3);
  auto tsf =
      sq(s1 + s2 + s3) - 2 * (s1 * s1 + s2 * s2 + s3 * s3) - 4 * s1 * s2 * s3;
  auto c3 = sq(q1 + q2 - q3) / (4 * q1 * q2);

  auto a3p = parametrize(3, a1, 4, a2);
  auto q1p = quadrance(a2, a3p);
  auto q2p = quadrance(a1, a3p);
  auto q3p = quadrance(a1, a2);
  auto tqf2 = archimedes(q1p, q2p, q3p); // get 0

  if constexpr (Integral<K>) {
    CHECK(!is_parallel(l1, l2));
    CHECK(!is_parallel(l2, l3));
    CHECK(is_perpendicular(t1, l1));
    CHECK(spread(t1, l1) == K(1));
    CHECK(coincident(t1 * t2, t3));
    CHECK(coincident(t1 * t2, t3, t4));
    CHECK(R(t1, t2, t3, t4) == K(-1));
    CHECK(o == t2 * t3);
    CHECK(tau(tau(a1)) == a1);
    CHECK(spread(l1, l1) == K(0));
    CHECK(quadrance(a1, a1) == K(0));
    CHECK(check_sine_law(Q, S));
    CHECK(check_sine_law(S, Q));
    CHECK(coincident(mt1 * mt2, mt3));
    // CHECK(cross_s(l1, l2) == c3);
    CHECK((c3 + s3) == K(1));
    CHECK(tqf == archimedes(q1, q2, q3));
    CHECK(tsf == K(0));
    CHECK(tqf2 == K(0));
    // auto o2 = orthocenter(
    //               std::tuple {std::move(o), std::move(a2),
    //               std::move(a3)});
    // CHECK(a1 == o2);
  } else {
    CHECK(cross2(l1, l2) != Zero);
    CHECK(cross2(l2, l3) != Zero);
    CHECK(dot1(t1, l1) == Zero);
    CHECK(spread(t1, l1) - 1 == Zero);
    CHECK(t1.dot(t2 * t3) == Zero);
    CHECK(R(t1, t2, t3, t4) + 1 == Zero);
    CHECK(ApproxZero(cross(meet(t2, t3), o)));
    CHECK(ApproxZero(cross(tau(tau(a1)), a1)));
    CHECK(mt1.dot(mt2 * mt3) == Zero);
    CHECK(spread(l1, l1) == Zero);
    CHECK(quadrance(a1, a1) == Zero);
    CHECK(angle(l1, l1) == Zero);
    CHECK(distance(a1, a1) == Zero);
    // CHECK(cross_s(l1, l2) == doctest::Approx(c3).epsilon(0.01));
    CHECK((c3 + s3) - 1 == Zero);
    CHECK(tqf - archimedes(q1, q2, q3) == Zero);
    CHECK(tsf == Zero);
    CHECK(tqf2 == Zero);
    // CHECK(ApproxEqual(a1, orthocenter(std::array{std::move(o), // not
    // quite accurate
    //                     std::move(a2), std::move(a3)})));
  }
}

template <typename T> void chk_cyclic(const T &quadangle) {
  auto &[u1, u2, u3, u4] = quadangle;

  auto q12 = quadrance(u1, u2);
  auto q23 = quadrance(u2, u3);
  auto q34 = quadrance(u3, u4);
  auto q14 = quadrance(u1, u4);
  auto q24 = quadrance(u2, u4);
  auto q13 = quadrance(u1, u3);

  using Point = decltype(u1);
  using K = Value_type<Point>;

  if constexpr (Integral<K>) {
    auto okay =
        Ptolemy(std::array{std::move(q12), std::move(q23), std::move(q34),
                           std::move(q14), std::move(q24), std::move(q13)});
    CHECK(okay);
  } else {
    auto t = archimedes(q12 * q34, q23 * q14, q13 * q24);
    CHECK(t == Zero);
  }
}

TEST_CASE("Euclid plane (cpp_int)") {
  using boost::multiprecision::cpp_int;

  auto a1 = pg_point<cpp_int>{1, 3, 1};
  auto a2 = pg_point<cpp_int>{4, 2, 1};
  auto a3 = pg_point<cpp_int>{4, -3, 1};

  auto triangle = std::array{std::move(a1), std::move(a2), std::move(a3)};
  chk_euclid(triangle);
}

TEST_CASE("Euclid plane (floating point_p)") {
  auto a1 = pg_point{1., 3., 1.};
  auto a2 = pg_point{4., 2., 1.};
  auto a3 = pg_point{4., -3., 1.};

  auto triangle = std::array{std::move(a1), std::move(a2), std::move(a3)};
  chk_euclid(triangle);
}

TEST_CASE("Euclid Cyclic Points (cpp_int)") {
  using boost::multiprecision::cpp_int;
  using Point = pg_point<cpp_int>;

  auto u1 = uc_point<Point>(1, 0);
  auto u2 = uc_point<Point>(3, 4);
  auto u3 = uc_point<Point>(-1, 2);
  auto u4 = uc_point<Point>(0, 1);

  auto quadangle =
      std::array{std::move(u1), std::move(u2), std::move(u3), std::move(u4)};
  chk_cyclic(quadangle);
}

TEST_CASE("Euclid Cyclic Points (double)") {
  using Point = pg_point<double>;

  auto u1 = uc_point<Point>(1, 0);
  auto u2 = uc_point<Point>(3, 4);
  auto u3 = uc_point<Point>(-1, 2);
  auto u4 = uc_point<Point>(0, 1);

  auto quadangle =
      std::array{std::move(u1), std::move(u2), std::move(u3), std::move(u4)};
  chk_cyclic(quadangle);
}

/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h> // for ResultBuilder

#include <array>                            // for operator==
#include <boost/multiprecision/cpp_int.hpp> // for cpp_int
#include <ostream>                          // for operator<<
#include <tuple>                            // for tuple
#include <type_traits>                      // for move

#include "projgeom/ck_plane.hpp"           // for ellck, hyck
#include "projgeom/common_concepts.h"      // for Value_type
#include "projgeom/fractions.hpp"          // for operator*
#include "projgeom/persp_plane.hpp"        // for persp_eucl...
#include "projgeom/pg_common.hpp"          // for cross
#include "projgeom/pg_line.hpp"            // for pg_line
#include "projgeom/pg_object.hpp"          // for operator*
#include "projgeom/pg_point.hpp"           // for pg_point
#include "projgeom/proj_plane.hpp"         // for coincident
#include "projgeom/proj_plane_measure.hpp" // for x_ratio
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

template <typename PG> void chk_ck(const PG &myck) {
  using Point = typename PG::point_t;
  using K = Value_type<Point>;

  auto a1 = Point{1, -2, 3};
  auto a2 = Point{4, 0, 6};
  auto a3 = Point{-7, 1, 2};
  const auto triangle = std::array{std::move(a1), std::move(a2), std::move(a3)};
  const auto trilateral = tri_dual(triangle);
  const auto &[l1, l2, l3] = trilateral;
  const auto [t1, t2, t3] = myck.tri_altitude(triangle);

  auto o = myck.orthocenter(triangle);
  const auto tau = myck.reflect(l1);
  const auto Q = myck.tri_quadrance(triangle);
  const auto S = myck.tri_spread(trilateral);

  const auto a4 = Point{3, 0, 2};

  if constexpr (Integral<K>) {
    CHECK(incident(l1, a2));
    CHECK(myck.is_perpendicular(t1, l1));
    CHECK(coincident(t1 * t2, t3));
    CHECK(o == t2 * t3);
    CHECK(a1 == myck.orthocenter(
                    std::array{std::move(o), std::move(a2), std::move(a3)}));
    CHECK(tau(tau(a4)) == a4);
    // CHECK(myck.spread(l2, l2) == K(0));
    // CHECK(myck.spread(l3, l3) == K(0));
    // CHECK(myck.quadrance(a1, a1) == K(0));
    CHECK(check_sine_law(Q, S));
    CHECK(check_sine_law(S, Q));
  } else {
    CHECK(l1.dot(a2) == Zero);
    CHECK(l1.dot(myck.perp(t1)) == Zero);
    CHECK(t1.dot(t2 * t3) == Zero);
    CHECK(ApproxZero(cross(o, t2 * t3)));
    const auto o2 = myck.orthocenter(
        std::array{std::move(o), std::move(a2), std::move(a3)});
    CHECK(ApproxZero(cross(a1, o2)));
    CHECK(ApproxZero(cross(tau(tau(a4)), a4)));
    CHECK(myck.measure(l2, l2) == Zero);
    CHECK(myck.measure(l3, l3) == Zero);
    CHECK(myck.measure(a1, a1) == Zero);
    const auto &[q1, q2, q3] = Q;
    const auto &[s1, s2, s3] = S;

    const auto r1 = q1 * s2 - q2 * s1;
    const auto r2 = q2 * s3 - q3 * s2;
    CHECK(r1 == Zero);
    CHECK(r2 == Zero);
  }
}

template <typename Point, typename Line = typename Point::Dual>
requires ProjectivePlanePrim<Point, Line> // pt_c++20 concept
struct myck : ck<Point, Line, myck> {
  [[nodiscard]] constexpr auto perp(const Point &v) const -> Line {
    return Line(-2 * v[0], v[1], -2 * v[2]);
  }

  [[nodiscard]] constexpr auto perp(const Line &v) const -> Point {
    return Point(-v[0], 2 * v[1], -v[2]);
  }

  template <ProjectivePlane2 _Point>
  [[nodiscard]] constexpr auto measure(const _Point &a1,
                                       const _Point &a2) const {
    auto x = x_ratio(a1, a2, this->perp(a2), this->perp(a1));
    // using Q_t = decltype(x);
    return 1 - x;
  }
};

TEST_CASE("CK plane chk_ck (int)") {
  // using boost::multiprecision::cpp_int;
  // namespace mp = boost::multiprecision;
  using boost::multiprecision::cpp_int;

  chk_ck(myck<pg_point<cpp_int>>());
  chk_ck(myck<pg_line<cpp_int>>());
  chk_ck(ellck<pg_point<cpp_int>>());
  chk_ck(ellck<pg_line<cpp_int>>());
  chk_ck(hyck<pg_point<cpp_int>>());
  chk_ck(hyck<pg_line<cpp_int>>());

  auto I_re = pg_point<cpp_int>{0, 1, 1};
  auto I_im = pg_point<cpp_int>{1, 0, 0};
  auto l_inf = pg_line<cpp_int>{0, -1, 1};

  auto pt_p =
      persp_euclid_plane{std::move(I_re), std::move(I_im), std::move(l_inf)};
  chk_ck(pt_p);
}

TEST_CASE("CK plane chk_ck (float)") {
  chk_ck(myck<pg_point<double>>());
  chk_ck(myck<pg_line<double>>());
  chk_ck(ellck<pg_point<float>>());
  chk_ck(ellck<pg_line<float>>());
  chk_ck(hyck<pg_point<double>>());
  chk_ck(hyck<pg_line<double>>());

  auto I_re = pg_point{0., 1., 1.};
  auto I_im = pg_point{1., 0., 0.};
  auto l_inf = pg_line{0., -1., 1.};

  auto pt_p =
      persp_euclid_plane{std::move(I_re), std::move(I_im), std::move(l_inf)};
  chk_ck(pt_p);
}

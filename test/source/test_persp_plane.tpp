/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h> // for ResultBuilder

#include <boost/multiprecision/cpp_int.hpp> // for cpp_int
#include <ostream>                          // for operator<<
#include <tuple>                            // for tuple
#include <type_traits>                      // for move

#include "projgeom/common_concepts.h" // for Value_type
#include "projgeom/euclid_plane.hpp"  // for archimedes
#include "projgeom/fractions.hpp"     // for operator*
#include "projgeom/persp_plane.hpp"   // for persp_eucl...
#include "projgeom/pg_common.hpp"     // for sq
#include "projgeom/pg_line.hpp"       // for pg_line
#include "projgeom/pg_object.hpp"     // for operator*
#include "projgeom/pg_point.hpp"      // for pg_point
#include "projgeom/proj_plane.hpp"    // for coincident
// #include <iostream>

using namespace fun;

static const auto Zero = doctest::Approx(0).epsilon(0.01);

/**
 * @brief
 *
 * @tparam PG
 * @param[in] myck
 */
template <typename PG> void chk_degenerate(const PG &myck) {
  using Point = typename PG::point_t;
  // using Line = typename PG::line_t;
  using K = Value_type<Point>;

  auto a1 = Point{-1, 0, 3};
  auto a2 = Point{4, -2, 1};
  auto a3 = Point{3, -1, 1};

  const auto m12 = myck.midpoint(a1, a2);
  const auto m23 = myck.midpoint(a2, a3);
  const auto m13 = myck.midpoint(a1, a3);
  const auto t1 = a1 * m23;
  const auto t2 = a2 * m13;
  const auto t3 = a3 * m12;

  auto triangle = std::array{std::move(a1), std::move(a2), std::move(a3)};
  const auto trilateral = tri_dual(triangle);

  // const auto& [a1, a2, a3] = triangle;
  const auto &[l1, l2, l3] = trilateral;

  const auto [q1, q2, q3] = myck.tri_quadrance(triangle);
  const auto [s1, s2, s3] = myck.tri_spread(trilateral);

  const auto tqf = sq(q1 + q2 + q3) - 2 * (q1 * q1 + q2 * q2 + q3 * q3);
  const auto tsf =
      sq(s1 + s2 + s3) - 2 * (s1 * s1 + s2 * s2 + s3 * s3) - 4 * s1 * s2 * s3;

  if constexpr (Integral<K>) {
    CHECK(!myck.is_parallel(l1, l2));
    CHECK(!myck.is_parallel(l2, l3));
    CHECK(coincident(t1 * t2, t3));
    CHECK(tqf == archimedes(q1, q2, q3));
    CHECK(tsf == K(0));
  } else {
    CHECK(myck.l_inf().dot(l1 * l2) != Zero);
    CHECK(myck.l_inf().dot(l2 * l3) != Zero);
    CHECK(t1.dot(t2 * t3) == Zero);
    CHECK(tqf - archimedes(q1, q2, q3) == Zero);
    CHECK(tsf == Zero);
  }
}

/**
 * @brief
 *
 * @tparam PG
 * @param[in] myck
 */
template <typename PG> void chk_degenerate2(const PG &myck) {
  using Point = typename PG::point_t;
  // using Line = typename PG::line_t;
  using K = Value_type<Point>;

  auto a1 = Point{-1, 0, 3};
  auto a2 = Point{4, -2, 1};
  auto a4 = parametrize(3, a1, 4, a2);

  const auto tri2 = std::array{std::move(a1), std::move(a2), std::move(a4)};
  const auto [qq1, qq2, qq3] = myck.tri_quadrance(tri2);
  const auto tqf2 = archimedes(qq1, qq2, qq3); // get 0

  if constexpr (Integral<K>) {
    CHECK(tqf2 == 0);
  } else {
    CHECK(tqf2 == Zero);
  }
}

TEST_CASE("Perspective Euclid plane (cpp_int)") {
  using boost::multiprecision::cpp_int;

  auto I_re = pg_point<cpp_int>(0, 1, 1);
  auto I_im = pg_point<cpp_int>(1, 0, 0);
  auto l_inf = pg_line<cpp_int>(0, -1, 1);

  const auto pt_p =
      persp_euclid_plane{std::move(I_re), std::move(I_im), std::move(l_inf)};
  chk_degenerate(pt_p);
  chk_degenerate2(pt_p);
}

TEST_CASE("Perspective Euclid plane (floating point_p)") {
  auto I_re = pg_point{0., 1., 1.};
  auto I_im = pg_point{1., 0., 0.};
  auto l_inf = pg_line{0., -1., 1.};

  const auto pt_p =
      persp_euclid_plane{std::move(I_re), std::move(I_im), std::move(l_inf)};
  chk_degenerate(pt_p);
  chk_degenerate2(pt_p);
}

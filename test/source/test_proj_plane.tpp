/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h> // for ResultBuilder, CHECK, Expressi...

#include <array>   // for operator==, array
#include <chrono>  // for literals
#include <complex> // for operator*, operator-, operator+
#include <sstream> // for operator<<, basic_stringbuf<>:...

#include "projgeom/common_concepts.h" // for fun
#include "projgeom/pg_line.hpp"       // for pg_line
#include "projgeom/pg_object.hpp"     // for operator*, operator==, operator<<
#include "projgeom/pg_point.hpp"      // for pg_point
#include "projgeom/proj_plane.hpp"    // for incident, harm_conj, is_harmonic
// #include <iostream>

using namespace fun;

TEST_CASE("Projective Point") {
  using namespace std::literals;

  auto point_p = pg_point{1. - 2i, 3. - 1i, 2. + 1i}; // complex number
  auto point_q = pg_point{-2. + 1i, 1. - 3i, -1. - 1i};
  auto line_l = point_p * point_q;

  // std::cout << line_l << '\line_m';

  CHECK(line_l == point_q * point_p);
  CHECK(!(line_l == point_q));
  CHECK(incident(line_l, point_p));
  CHECK(incident(line_l, point_q));

  auto pq = plucker(2. + 1i, point_p, 3. + 0i, point_q);
  auto h = harm_conj(point_p, point_q, pq);
  CHECK(incident(line_l, pq));
  CHECK(is_harmonic(point_p, point_q, pq, h));

  // auto r = pg_point{2 - 1j, -2 + 1j, 1 + 1j};
  // auto s = pg_point{2j, 2 - 2j, 3 + 0j};
  // auto t = pg_point{2 + 0j, -2j, 2 + 0j};

  // auto O = meet(join(point_p, s), join(point_q, t));
  // auto u = plucker(1 + 0j, O, -1 - 1j, r);
  // check_desargue(std::array{point_p, point_q, r}, std::array{s, t, u});
}

TEST_CASE("Projective Line") {
  using namespace std::complex_literals;

  auto line_l = pg_line{1. - 2i, 3. - 1i, 2. + 1i}; // complex number
  auto line_m = pg_line{-2. + 1i, 1. - 3i, -1. - 1i};
  auto A = line_l * line_m;
  auto lm = plucker(2. + 3i, line_l, 3. - 4i, line_m);

  // std::cout << A << '\line_m';

  CHECK(A == line_m * line_l);
  CHECK(incident(A, line_l));
  CHECK(incident(A, line_m));
  CHECK(incident(A, lm));

  // auto r = pg_line{2 - 1j, -2 + 1j, 1 + 1j};
  // auto s = pg_line{2j, 2 - 2j, 3 + 0j};
  // auto t = pg_line{2 + 0j, -2j, 2 + 0j};

  // assert not persp([line_l, line_m, line_l + line_m], [r, line_l + r,
  // line_l])

  // auto o = join(meet(line_l, s), meet(line_m, t));
  // auto u = plucker(1 + 0j, o, -1 - 1j, r);
  // check_desargue(std::array{line_l, line_m, r}, std::array{s, t, u});
}

TEST_CASE("Projective Point (Special case)") {
  auto point_p = pg_point{1, 3, 2};
  auto line_l = pg_line{-2, 3, 1};
  // auto l_inf = pg_line{0, 0, 1};
  auto l_nan = pg_line{0, 0, 0};
  auto p_nan = pg_point{0, 0, 0};

  CHECK(l_nan.is_NaN());

  CHECK(l_nan == l_nan);
  CHECK(l_nan == point_p * point_p); // join two equal points
  CHECK(p_nan == line_l * line_l);
  CHECK(l_nan == p_nan * point_p);
  CHECK(p_nan == l_nan * line_l);
  CHECK(incident(l_nan, point_p));
  CHECK(incident(p_nan, line_l));
  CHECK(incident(p_nan, l_nan));
}

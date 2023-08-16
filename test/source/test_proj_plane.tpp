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

  auto pt_p = pg_point{1. - 2i, 3. - 1i, 2. + 1i}; // complex number
  auto pt_q = pg_point{-2. + 1i, 1. - 3i, -1. - 1i};
  auto ln_l = pt_p * pt_q;

  // std::cout << ln_l << '\ln_m';

  CHECK(ln_l == pt_q * pt_p);
  CHECK(!(ln_l == pt_q));
  CHECK(incident(ln_l, pt_p));
  CHECK(incident(ln_l, pt_q));

  auto pq = parametrize(2. + 1i, pt_p, 3. + 0i, pt_q);
  auto pt_h = harm_conj(pt_p, pt_q, pq);
  CHECK(incident(ln_l, pq));
  CHECK(is_harmonic(pt_p, pt_q, pq, pt_h));

  // auto pt_r = pg_point{2 - 1j, -2 + 1j, 1 + 1j};
  // auto s = pg_point{2j, 2 - 2j, 3 + 0j};
  // auto t = pg_point{2 + 0j, -2j, 2 + 0j};

  // auto O = meet(join(pt_p, s), join(pt_q, t));
  // auto u = parametrize(1 + 0j, O, -1 - 1j, pt_r);
  // check_desargue(std::array{pt_p, pt_q, pt_r}, std::array{s, t, u});
}

TEST_CASE("Projective Line") {
  using namespace std::complex_literals;

  auto ln_l = pg_line{1. - 2i, 3. - 1i, 2. + 1i}; // complex number
  auto ln_m = pg_line{-2. + 1i, 1. - 3i, -1. - 1i};
  auto A = ln_l * ln_m;
  auto lm = parametrize(2. + 3i, ln_l, 3. - 4i, ln_m);

  // std::cout << A << '\ln_m';

  CHECK(A == ln_m * ln_l);
  CHECK(incident(A, ln_l));
  CHECK(incident(A, ln_m));
  CHECK(incident(A, lm));

  // auto pt_r = pg_line{2 - 1j, -2 + 1j, 1 + 1j};
  // auto s = pg_line{2j, 2 - 2j, 3 + 0j};
  // auto t = pg_line{2 + 0j, -2j, 2 + 0j};

  // assert not persp([ln_l, ln_m, ln_l + ln_m], [pt_r, ln_l + pt_r,
  // ln_l])

  // auto o = join(meet(ln_l, s), meet(ln_m, t));
  // auto u = parametrize(1 + 0j, o, -1 - 1j, pt_r);
  // check_desargue(std::array{ln_l, ln_m, pt_r}, std::array{s, t, u});
}

TEST_CASE("Projective Point (Special case)") {
  auto pt_p = pg_point{1, 3, 2};
  auto ln_l = pg_line{-2, 3, 1};
  // auto l_inf = pg_line{0, 0, 1};
  auto l_nan = pg_line{0, 0, 0};
  auto p_nan = pg_point{0, 0, 0};

  CHECK(l_nan.is_NaN());

  CHECK(l_nan == l_nan);
  CHECK(l_nan == pt_p * pt_p); // join two equal points
  CHECK(p_nan == ln_l * ln_l);
  CHECK(l_nan == p_nan * pt_p);
  CHECK(p_nan == l_nan * ln_l);
  CHECK(incident(l_nan, pt_p));
  CHECK(incident(p_nan, ln_l));
  CHECK(incident(p_nan, l_nan));
}

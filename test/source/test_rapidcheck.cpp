#ifdef RAPIDCHECK_H
#    include <rapidcheck.h>

#    include <doctest/doctest.h>

#    include <projgeom/persp_object.hpp>
#    include <projgeom/ell_object.hpp>
#    include <projgeom/pg_object.hpp>
#    include <projgeom/pg_plane.hpp>

TEST_CASE("Property-based test: dot product is commutative") {
    rc::check("dot(a, b) == dot(b, a) for any vectors a and b",
              []() {
                  auto a = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  auto b = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  
                  int64_t dot_ab = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
                  int64_t dot_ba = b[0] * a[0] + b[1] * a[1] + b[2] * a[2];
                  
                  RC_ASSERT(dot_ab == dot_ba);
              });
}

TEST_CASE("Property-based test: dot product is linear") {
    rc::check("dot(a, b + c) == dot(a, b) + dot(a, c)",
              []() {
                  auto a = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  auto b = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  auto c = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  
                  int64_t lhs = a[0] * (b[0] + c[0]) + a[1] * (b[1] + c[1]) + a[2] * (b[2] + c[2]);
                  int64_t rhs = (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]) + 
                               (a[0] * c[0] + a[1] * c[1] + a[2] * c[2]);
                  
                  RC_ASSERT(lhs == rhs);
              });
}

TEST_CASE("Property-based test: cross product anti-commutative") {
    rc::check("cross(a, b) == -cross(b, a)",
              []() {
                  auto a = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  auto b = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  
                  auto cross_ab = std::array<int64_t, 3>{
                      a[1] * b[2] - a[2] * b[1],
                      a[2] * b[0] - a[0] * b[2],
                      a[0] * b[1] - a[1] * b[0]
                  };
                  auto cross_ba = std::array<int64_t, 3>{
                      b[1] * a[2] - b[2] * a[1],
                      b[2] * a[0] - b[0] * a[2],
                      b[0] * a[1] - b[1] * a[0]
                  };
                  
                  RC_ASSERT(cross_ab[0] == -cross_ba[0]);
                  RC_ASSERT(cross_ab[1] == -cross_ba[1]);
                  RC_ASSERT(cross_ab[2] == -cross_ba[2]);
              });
}

TEST_CASE("Property-based test: cross product perpendicular to inputs") {
    rc::check("cross(a, b) is perpendicular to both a and b",
              []() {
                  auto a = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  auto b = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  
                  auto cross_ab = std::array<int64_t, 3>{
                      a[1] * b[2] - a[2] * b[1],
                      a[2] * b[0] - a[0] * b[2],
                      a[0] * b[1] - a[1] * b[0]
                  };
                  
                  int64_t dot_a_cross = a[0] * cross_ab[0] + a[1] * cross_ab[1] + a[2] * cross_ab[2];
                  int64_t dot_b_cross = b[0] * cross_ab[0] + b[1] * cross_ab[1] + b[2] * cross_ab[2];
                  
                  RC_ASSERT(dot_a_cross == 0);
                  RC_ASSERT(dot_b_cross == 0);
              });
}

TEST_CASE("Property-based test: plucker linearity in first argument") {
    rc::check("plucker(k1*a + k2*b, c) == k1*plucker(a, c) + k2*plucker(b, c)",
              []() {
                  auto k1 = *rc::gen::arbitrary<int64_t>();
                  auto k2 = *rc::gen::arbitrary<int64_t>();
                  auto a = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  auto b = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  auto c = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  
                  auto k1a_k2b = std::array<int64_t, 3>{
                      k1 * a[0] + k2 * b[0],
                      k1 * a[1] + k2 * b[1],
                      k1 * a[2] + k2 * b[2]
                  };
                  
                  auto lhs = std::array<int64_t, 3>{
                      k1a_k2b[1] * c[2] - k1a_k2b[2] * c[1],
                      k1a_k2b[2] * c[0] - k1a_k2b[0] * c[2],
                      k1a_k2b[0] * c[1] - k1a_k2b[1] * c[0]
                  };
                  
                  auto cross_ac = std::array<int64_t, 3>{
                      a[1] * c[2] - a[2] * c[1],
                      a[2] * c[0] - a[0] * c[2],
                      a[0] * c[1] - a[1] * c[0]
                  };
                  
                  auto cross_bc = std::array<int64_t, 3>{
                      b[1] * c[2] - b[2] * c[1],
                      b[2] * c[0] - b[0] * c[2],
                      b[0] * c[1] - b[1] * c[0]
                  };
                  
                  auto rhs = std::array<int64_t, 3>{
                      k1 * cross_ac[0] + k2 * cross_bc[0],
                      k1 * cross_ac[1] + k2 * cross_bc[1],
                      k1 * cross_ac[2] + k2 * cross_bc[2]
                  };
                  
                  RC_ASSERT(lhs == rhs);
              });
}

TEST_CASE("Property-based test: PgPoint equality is reflexive") {
    rc::check("pg_point == pg_point for any point",
              []() {
                  auto coord = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  PgPoint pt(coord);
                  RC_ASSERT(pt == pt);
              });
}

TEST_CASE("Property-based test: PgPoint equality is symmetric") {
    rc::check("If p == q then q == p",
              []() {
                  auto coord = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  PgPoint pt_p(coord);
                  PgPoint pt_q(coord);
                  if (pt_p == pt_q) {
                      RC_ASSERT(pt_q == pt_p);
                  }
              });
}

TEST_CASE("Property-based test: PgPoint scaled coordinates are equal") {
    rc::check("pg_point(coord) == pg_point(k*coord) for any non-zero k",
              []() {
                  auto coord = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  auto k = *rc::gen::nonZero<int64_t>();
                  
                  // Skip if scaling would overflow
                  if (coord[0] != 0 && abs(k) > INT64_MAX / abs(coord[0])) return;
                  if (coord[1] != 0 && abs(k) > INT64_MAX / abs(coord[1])) return;
                  if (coord[2] != 0 && abs(k) > INT64_MAX / abs(coord[2])) return;
                  
                  auto scaled = std::array<int64_t, 3>{
                      k * coord[0],
                      k * coord[1],
                      k * coord[2]
                  };
                  
                  PgPoint pt_p(coord);
                  PgPoint pt_q(scaled);
                  RC_ASSERT(pt_p == pt_q);
              });
}

TEST_CASE("Property-based test: PgLine equality is reflexive") {
    rc::check("pg_line == pg_line for any line",
              []() {
                  auto coord = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  PgLine ln(coord);
                  RC_ASSERT(ln == ln);
              });
}

TEST_CASE("Property-based test: incident is symmetric for dot product") {
    rc::check("If point is incident to line, then line dot point is zero",
              []() {
                  auto pt_coord = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  auto ln_coord = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  
                  PgPoint pt(pt_coord);
                  PgLine ln(ln_coord);
                  
                  bool is_incident = pt.incident(ln);
                  int64_t dot_value = pt_coord[0] * ln_coord[0] + pt_coord[1] * ln_coord[1] + pt_coord[2] * ln_coord[2];
                  
                  RC_ASSERT(is_incident == (dot_value == 0));
              });
}

TEST_CASE("Property-based test: meet of two points gives incident line") {
    rc::check("meet(p, q) returns a line incident to both p and q",
              []() {
                  auto coord_p = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  auto coord_q = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  
                  PgPoint pt_p(coord_p);
                  PgPoint pt_q(coord_q);
                  
                  PgLine ln = pt_p.meet(pt_q);
                  
                  RC_ASSERT(pt_p.incident(ln));
                  RC_ASSERT(pt_q.incident(ln));
              });
}

TEST_CASE("Property-based test: EllipticPoint perp returns line with same coordinates") {
    rc::check("EllipticPoint.perp() returns EllipticLine with same coordinates",
              []() {
                  auto coord = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  EllipticPoint pt(coord);
                  EllipticLine ln = pt.perp();
                  
                  RC_ASSERT(ln.coord == coord);
              });
}

TEST_CASE("Property-based test: EllipticLine perp returns point with same coordinates") {
    rc::check("EllipticLine.perp() returns EllipticPoint with same coordinates",
              []() {
                  auto coord = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  EllipticLine ln(coord);
                  EllipticPoint pt = ln.perp();
                  
                  RC_ASSERT(pt.coord == coord);
              });
}

TEST_CASE("Property-based test: double perp returns original for elliptic") {
    rc::check("EllipticPoint.perp().perp() == original point",
              []() {
                  auto coord = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  EllipticPoint pt(coord);
                  EllipticPoint pt_double = pt.perp().perp();
                  
                  RC_ASSERT(pt_double == pt);
              });
}

TEST_CASE("Property-based test: double perp returns original for elliptic line") {
    rc::check("EllipticLine.perp().perp() == original line",
              []() {
                  auto coord = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  EllipticLine ln(coord);
                  EllipticLine ln_double = ln.perp().perp();
                  
                  RC_ASSERT(ln_double == ln);
              });
}

TEST_CASE("Property-based test: PerspPoint perp returns L_INF") {
    rc::check("PerspPoint.perp() returns L_INF",
              []() {
                  auto coord = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  PerspPoint pt(coord);
                  const PerspLine& ln = pt.perp();
                  
                  RC_ASSERT(ln == L_INF);
              });
}

TEST_CASE("Property-based test: parametrize identity") {
    rc::check("parametrize(1, p, 0, q) == p",
              []() {
                  auto coord_p = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  auto coord_q = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  
                  PgPoint pt_p(coord_p);
                  PgPoint pt_q(coord_q);
                  PgPoint pt_result = PgPoint::parametrize(static_cast<int64_t>(1), pt_p, 
                                                           static_cast<int64_t>(0), pt_q);
                  
                  RC_ASSERT(pt_result == pt_p);
              });
}

TEST_CASE("Property-based test: parametrize linearity") {
    rc::check("parametrize(k1, p, k2, q) is linear",
              []() {
                  auto k1 = *rc::gen::arbitrary<int64_t>();
                  auto k2 = *rc::gen::arbitrary<int64_t>();
                  auto coord_p = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  auto coord_q = *rc::gen::container<std::array<int64_t, 3>>(rc::gen::arbitrary<int64_t>());
                  
                  PgPoint pt_p(coord_p);
                  PgPoint pt_q(coord_q);
                  PgPoint pt_result = PgPoint::parametrize(k1, pt_p, k2, pt_q);
                  
                  auto expected = std::array<int64_t, 3>{
                      k1 * coord_p[0] + k2 * coord_q[0],
                      k1 * coord_p[1] + k2 * coord_q[1],
                      k1 * coord_p[2] + k2 * coord_q[2]
                  };
                  
                  PgPoint pt_expected(expected);
                  RC_ASSERT(pt_result == pt_expected);
              });
}

#endif
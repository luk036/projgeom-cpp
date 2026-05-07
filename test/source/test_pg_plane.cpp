#include <doctest/doctest.h>

#include <array>
#include <cstdint>

#include <projgeom/ck_plane.hpp>
#include <projgeom/ell_object.hpp>
#include <projgeom/hyp_object.hpp>
#include <projgeom/persp_object.hpp>
#include <projgeom/pg_object.hpp>
#include <projgeom/pg_plane.hpp>

TEST_CASE("pg_plane: coincident non-collinear") {
    PgPoint pt_a({1, 0, 0});
    PgPoint pt_b({0, 1, 0});
    PgPoint pt_c({0, 0, 1});
    CHECK(!fun::coincident(pt_a, pt_b, pt_c));
}

TEST_CASE("pg_plane: coincident collinear") {
    PgPoint pt_d({2, 0, 0});
    PgPoint pt_e({4, 0, 0});
    PgPoint pt_f({6, 0, 0});
    CHECK(fun::coincident(pt_d, pt_e, pt_f));
}

TEST_CASE("pg_plane: check_pappus") {
    PgPoint pt_a({1, 0, 0});
    PgPoint pt_b({0, 1, 0});
    PgPoint pt_c({1, 1, 0});
    PgPoint pt_d({2, 0, 0});
    PgPoint pt_e({0, 2, 0});
    PgPoint pt_f({2, 2, 0});
    
    std::array<PgPoint, 3> coline1 = {pt_a, pt_b, pt_c};
    std::array<PgPoint, 3> coline2 = {pt_d, pt_e, pt_f};
    CHECK(fun::check_pappus(coline1, coline2));
}

TEST_CASE("pg_plane: tri_dual") {
    PgPoint pt_a({1, 0, 0});
    PgPoint pt_b({0, 1, 0});
    PgPoint pt_c({0, 0, 1});
    std::array<PgPoint, 3> triangle = {pt_a, pt_b, pt_c};
    
    auto dual = fun::tri_dual(triangle);
    auto ln_ab = pt_a.meet(pt_b);
    CHECK(dual[2] == ln_ab);
}

TEST_CASE("pg_plane: persp triangles") {
    PgPoint pt_a({1, 0, 0});
    PgPoint pt_b({0, 1, 0});
    PgPoint pt_c({0, 0, 1});
    PgPoint pt_d({2, 0, 0});
    PgPoint pt_e({0, 2, 0});
    PgPoint pt_f({0, 0, 2});
    
    std::array<PgPoint, 3> tri1 = {pt_a, pt_b, pt_c};
    std::array<PgPoint, 3> tri2 = {pt_d, pt_e, pt_f};
    CHECK(fun::persp(tri1, tri2));
}

TEST_CASE("pg_plane: persp - perspective from origin") {
    PgPoint pt_a({1, 0, 0});
    PgPoint pt_b({0, 1, 0});
    PgPoint pt_c({0, 0, 1});
    PgPoint pt_d({2, 0, 0});
    PgPoint pt_e({0, 2, 0});
    PgPoint pt_f({0, 0, 2});
    
    std::array<PgPoint, 3> tri1 = {pt_a, pt_b, pt_c};
    std::array<PgPoint, 3> tri2 = {pt_d, pt_e, pt_f};
    CHECK(fun::persp(tri1, tri2));
}

TEST_CASE("pg_plane: check_desargue") {
    PgPoint pt_a({1, 0, 1});
    PgPoint pt_b({0, 1, 1});
    PgPoint pt_c({1, 1, 1});
    PgPoint pt_d({3, 2, 1});
    PgPoint pt_e({2, 3, 1});
    PgPoint pt_f({5, 5, 1});
    
    std::array<PgPoint, 3> tri1 = {pt_a, pt_b, pt_c};
    std::array<PgPoint, 3> tri2 = {pt_d, pt_e, pt_f};
    CHECK(fun::check_desargue(tri1, tri2));
}

TEST_CASE("pg_plane: check_desargue - perspective triangles") {
    PgPoint pt_a({1, 0, 1});
    PgPoint pt_b({0, 1, 1});
    PgPoint pt_c({1, 1, 1});
    PgPoint pt_d({2, 0, 1});
    PgPoint pt_e({0, 2, 1});
    PgPoint pt_f({2, 2, 1});
    
    std::array<PgPoint, 3> tri1 = {pt_a, pt_b, pt_c};
    std::array<PgPoint, 3> tri2 = {pt_d, pt_e, pt_f};
    CHECK(fun::check_desargue(tri1, tri2));
}

TEST_CASE("pg_plane: check_axiom2") {
    PgPoint pt_p({3, 4, 5});
    PgPoint pt_q({0, 4, 1});
    PgLine ln_l({1, 0, 4});
    CHECK(fun::check_axiom2(pt_p, pt_q, ln_l, 1, 1));
    CHECK(fun::check_axiom2(pt_p, pt_q, ln_l, 2, 3));
}

TEST_CASE("pg_plane: check_axiom2 - various parameters") {
    PgPoint pt_p({1, 2, 3});
    PgPoint pt_q({4, 5, 6});
    PgLine ln_l({1, 0, 4});
    CHECK(fun::check_axiom2(pt_p, pt_q, ln_l, 1, 0));
    CHECK(fun::check_axiom2(pt_p, pt_q, ln_l, 0, 1));
    CHECK(fun::check_axiom2(pt_p, pt_q, ln_l, 2, 3));
    CHECK(fun::check_axiom2(pt_p, pt_q, ln_l, -1, 2));
}

TEST_CASE("pg_plane: check_axiom - more cases") {
    PgPoint pt_p({1, 0, 0});
    PgPoint pt_q({0, 1, 0});
    PgLine ln_l({0, 0, 1});
    CHECK(fun::check_axiom(pt_p, pt_q, ln_l));
    
    PgPoint pt_r({1, 1, 1});
    PgPoint pt_s({2, 3, 1});
    PgLine ln_m({-3, 2, 0});
    CHECK(fun::check_axiom(pt_r, pt_s, ln_m));
}

TEST_CASE("check axioms for different geometries") {
    PerspPoint pt_p1({3, 4, 5});
    PerspPoint pt_q1({0, 4, 1});
    PerspLine ln_m1({1, 0, 4});
    CHECK(fun::check_axiom(pt_p1, pt_q1, ln_m1));
    
    EllipticPoint pt_p2({3, 4, 5});
    EllipticPoint pt_q2({0, 4, 1});
    EllipticLine ln_m2({1, 0, 4});
    CHECK(fun::check_axiom(pt_p2, pt_q2, ln_m2));
    
    HyperbolicPoint pt_p3({3, 4, 5});
    HyperbolicPoint pt_q3({0, 4, 1});
    HyperbolicLine ln_m3({1, 0, 4});
    CHECK(fun::check_axiom(pt_p3, pt_q3, ln_m3));
}

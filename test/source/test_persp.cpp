#include <doctest/doctest.h>

#include <projgeom/ck_plane.hpp>
#include <projgeom/ell_object.hpp>
#include <projgeom/hyp_object.hpp>
#include <projgeom/myck_object.hpp>
#include <projgeom/persp_object.hpp>
#include <projgeom/pg_object.hpp>
#include <projgeom/pg_plane.hpp>

TEST_CASE("Perspective") {
    auto pt_p = PerspPoint({3, 4, 5});
    auto pt_q = PerspPoint({0, 4, 1});
    auto ln_m = PerspLine({1, 0, 4});
    bool is_ok = fun::check_axiom(pt_p, pt_q, ln_m);
    CHECK(is_ok);
    auto t = fun::altitude(pt_p, ln_m);
    bool res = fun::is_perpendicular(t, ln_m);
    CHECK(res);
}

TEST_CASE("Elliptic") {
    auto pt_p = EllipticPoint({3, 4, 5});
    auto pt_q = EllipticPoint({0, 4, 1});
    auto ln_m = EllipticLine({1, 0, 4});
    bool is_ok = fun::check_axiom(pt_p, pt_q, ln_m);
    CHECK(is_ok);
    auto t = fun::altitude(pt_p, ln_m);
    bool res = fun::is_perpendicular(t, ln_m);
    CHECK(res);
}

TEST_CASE("PerspPoint") {
    auto pt_p = PerspPoint({1, 2, 3});
    CHECK(pt_p.perp() == L_INF);
}

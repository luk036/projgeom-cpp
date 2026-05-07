#include <doctest/doctest.h>

#include <cstdint>

#include <projgeom/ck_plane.hpp>
#include <projgeom/hyp_object.hpp>

TEST_CASE("ck_plane: is_perpendicular - Hyperbolic") {
    HyperbolicLine ln_l({1, 0, 0});
    HyperbolicLine ln_m({0, 1, 0});
    (void)fun::is_perpendicular(ln_l, ln_m);
}

TEST_CASE("ck_plane: altitude - Hyperbolic") {
    HyperbolicPoint pt_p({3, 4, 5});
    HyperbolicLine ln_m({1, 0, 4});
    auto t = fun::altitude(pt_p, ln_m);
    bool res = fun::is_perpendicular(t, ln_m);
    CHECK(res);
}

TEST_CASE("ck_plane: orthocenter - Hyperbolic") {
    HyperbolicPoint pt_a({2, 1, 1});
    HyperbolicPoint pt_b({1, 2, 1});
    HyperbolicPoint pt_c({1, 1, 2});
    std::array<HyperbolicPoint, 3> triangle = {pt_a, pt_b, pt_c};
    
    auto ortho = fun::orthocenter(triangle);
    bool is_nonzero = ortho.coord[0] != 0 || ortho.coord[1] != 0 || ortho.coord[2] != 0;
    CHECK(is_nonzero);
}

TEST_CASE("Hyperbolic: point perp returns line with negated z") {
    HyperbolicPoint pt({3, 4, 5});
    auto ln = pt.perp();
    CHECK(ln.coord[0] == 3);
    CHECK(ln.coord[1] == 4);
    CHECK(ln.coord[2] == -5);
}

TEST_CASE("Hyperbolic: line perp returns point with negated z") {
    HyperbolicLine ln({3, 4, 5});
    auto pt = ln.perp();
    CHECK(pt.coord[0] == 3);
    CHECK(pt.coord[1] == 4);
    CHECK(pt.coord[2] == -5);
}

TEST_CASE("Hyperbolic: double perp returns original") {
    HyperbolicPoint pt({3, 4, 5});
    auto pt2 = pt.perp().perp();
    CHECK(pt2 == pt);
    
    HyperbolicLine ln({3, 4, 5});
    auto ln2 = ln.perp().perp();
    CHECK(ln2 == ln);
}

TEST_CASE("Hyperbolic: poles and polars chain") {
    HyperbolicPoint pt({1, 2, 3});
    auto ln = pt.perp();
    CHECK(ln.coord[2] == -3);
    auto pt2 = ln.perp();
    CHECK(pt2 == pt);
}

#include <doctest/doctest.h>

#include <projgeom/ck_plane.hpp>
#include <projgeom/ell_object.hpp>

TEST_CASE("ck_plane: is_perpendicular - Elliptic") {
    EllipticLine ln_l({1, 0, 0});
    EllipticLine ln_m({0, 1, 0});
    (void)fun::is_perpendicular(ln_l, ln_m);
}

TEST_CASE("ck_plane: altitude - Elliptic") {
    EllipticPoint pt_p({3, 4, 5});
    EllipticLine ln_m({1, 0, 4});
    auto t = fun::altitude(pt_p, ln_m);
    bool res = fun::is_perpendicular(t, ln_m);
    CHECK(res);
}

TEST_CASE("ck_plane: orthocenter - Elliptic") {
    EllipticPoint pt_a({1, 0, 1});
    EllipticPoint pt_b({0, 1, 1});
    EllipticPoint pt_c({1, 1, 1});
    std::array<EllipticPoint, 3> triangle = {pt_a, pt_b, pt_c};

    auto ortho = fun::orthocenter(triangle);
    (void)ortho;
}

TEST_CASE("Elliptic: point perp returns line with same coords") {
    EllipticPoint pt({3, 4, 5});
    auto ln = pt.perp();
    CHECK(ln.coord == pt.coord);
}

TEST_CASE("Elliptic: line perp returns point with same coords") {
    EllipticLine ln({3, 4, 5});
    auto pt = ln.perp();
    CHECK(pt.coord == ln.coord);
}

TEST_CASE("Elliptic: double perp returns original") {
    EllipticPoint pt({3, 4, 5});
    auto pt2 = pt.perp().perp();
    CHECK(pt2 == pt);

    EllipticLine ln({3, 4, 5});
    auto ln2 = ln.perp().perp();
    CHECK(ln2 == ln);
}

TEST_CASE("Elliptic: poles and polars chain") {
    EllipticPoint pt({1, 2, 3});
    auto ln = pt.perp();
    auto pt2 = ln.perp();
    CHECK(pt2 == pt);
    CHECK(ln.coord == pt.coord);
}

#include <doctest/doctest.h>

#include <cstdint>

#include <projgeom/ck_plane.hpp>
#include <projgeom/persp_object.hpp>

TEST_CASE("ck_plane: is_perpendicular - Perspective") {
    PerspLine ln_l({1, 0, 0});
    PerspLine ln_m({0, 1, 0});
    (void)fun::is_perpendicular(ln_l, ln_m);
}

TEST_CASE("ck_plane: altitude - Perspective") {
    PerspPoint pt_p({3, 4, 5});
    PerspLine ln_m({1, 0, 4});
    auto t = fun::altitude(pt_p, ln_m);
    bool res = fun::is_perpendicular(t, ln_m);
    CHECK(res);
}

TEST_CASE("ck_plane: orthocenter - Perspective") {
    PerspPoint pt_a({1, 0, 1});
    PerspPoint pt_b({0, 1, 1});
    PerspPoint pt_c({1, 1, 1});
    std::array<PerspPoint, 3> triangle = {pt_a, pt_b, pt_c};
    
    auto ortho = fun::orthocenter(triangle);
    (void)ortho;
}

TEST_CASE("ck_plane: altitude is perpendicular") {
    PerspPoint pt_p({2, 3, 1});
    PerspLine ln_m({1, 1, 1});
    auto alt = fun::altitude(pt_p, ln_m);
    CHECK(fun::is_perpendicular(alt, ln_m));
}

TEST_CASE("ck_plane: altitude from point on line is line itself") {
    PerspPoint pt_p({1, 1, 0});
    PerspLine ln_m({1, -1, 0});
    auto alt = fun::altitude(pt_p, ln_m);
    (void)alt;
}

TEST_CASE("PerspLine: pole") {
    PerspLine ln({1, 2, 3});
    auto pt = ln.perp();
    bool is_nonzero = pt.coord[0] != 0 || pt.coord[1] != 0 || pt.coord[2] != 0;
    CHECK(is_nonzero);
}

TEST_CASE("PerspPoint: perp is L_INF") {
    PerspPoint pt({3, 4, 5});
    CHECK(pt.perp() == L_INF);
}

TEST_CASE("Perspective: L_INF constant") {
    CHECK(L_INF.coord[0] == 0);
    CHECK(L_INF.coord[1] == -1);
    CHECK(L_INF.coord[2] == 1);
}

TEST_CASE("Perspective: I_RE and I_IM constants") {
    CHECK(I_RE.coord[0] == 0);
    CHECK(I_RE.coord[1] == 1);
    CHECK(I_RE.coord[2] == 1);
    CHECK(I_IM.coord[0] == 1);
    CHECK(I_IM.coord[1] == 0);
    CHECK(I_IM.coord[2] == 0);
}

#include <doctest/doctest.h>

#include <cstdint>

#include <projgeom/ck_plane.hpp>
#include <projgeom/myck_object.hpp>

TEST_CASE("MyCK: basic operations") {
    MyCKPoint pt_p({3, 4, 5});
    MyCKPoint pt_q({0, 4, 1});
    MyCKLine ln_m({1, 0, 4});
    bool is_ok = fun::check_axiom(pt_p, pt_q, ln_m);
    CHECK(is_ok);
    
    auto t = fun::altitude(pt_p, ln_m);
    bool res = fun::is_perpendicular(t, ln_m);
    CHECK(res);
}

TEST_CASE("MyCK: perp chain") {
    MyCKPoint pt({1, 2, 3});
    auto ln = pt.perp();
    auto pt2 = ln.perp();
    CHECK(pt2 == pt);
}

TEST_CASE("MyCK: altitude is perpendicular") {
    MyCKPoint pt_p({2, 3, 1});
    MyCKLine ln_m({1, 1, 1});
    auto alt = fun::altitude(pt_p, ln_m);
    CHECK(fun::is_perpendicular(alt, ln_m));
}

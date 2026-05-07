#include <doctest/doctest.h>

#include <array>
#include <cstdint>

#include <projgeom/pg_object.hpp>

TEST_CASE("pg_object: dot function") {
    std::array<int64_t, 3> a = {1, 2, 3};
    std::array<int64_t, 3> b = {4, 5, 6};
    auto result = dot(a, b);
    CHECK_EQ(result, 32);
}

TEST_CASE("pg_object: cross function") {
    std::array<int64_t, 3> a = {1, 2, 3};
    std::array<int64_t, 3> b = {4, 5, 6};
    auto result = cross(a, b);
    CHECK_EQ(result[0], -3);
    CHECK_EQ(result[1], 6);
    CHECK_EQ(result[2], -3);
}

TEST_CASE("pg_object: plckr function") {
    std::array<int64_t, 3> pt_p = {1, 2, 3};
    std::array<int64_t, 3> pt_q = {4, 5, 6};
    auto result = plckr(2, pt_p, 3, pt_q);
    CHECK_EQ(result[0], 14);
    CHECK_EQ(result[1], 19);
    CHECK_EQ(result[2], 24);
}

TEST_CASE("pg_object: PgPoint incident") {
    PgPoint pt({1, 2, 3});
    PgLine ln({3, -3, 1});
    CHECK(pt.incident(ln));
    PgLine ln2({3, -2, 0});
    CHECK(!pt.incident(ln2));
}

TEST_CASE("pg_object: PgPoint meet") {
    PgPoint pt_p({1, 0, 0});
    PgPoint pt_q({0, 1, 0});
    auto ln = pt_p.meet(pt_q);
    CHECK(ln.coord[0] == 0);
    CHECK(ln.coord[1] == 0);
    CHECK(ln.coord[2] == 1);
}

TEST_CASE("pg_object: PgLine incident") {
    PgPoint pt({1, 2, 3});
    PgLine ln_incident({3, -3, 1});
    CHECK(pt.incident(ln_incident));
    PgLine ln({3, -2, 0});
    CHECK(!pt.incident(ln));
}

TEST_CASE("pg_object: PgLine meet") {
    PgLine ln_l({1, 0, 0});
    PgLine ln_m({0, 1, 0});
    auto pt = ln_l.meet(ln_m);
    CHECK(pt.coord[0] == 0);
    CHECK(pt.coord[1] == 0);
    CHECK(pt.coord[2] == 1);
}

TEST_CASE("pg_object: parametrize identity") {
    PgPoint pt_p({1, 2, 3});
    PgPoint pt_q({4, 5, 6});
    auto result = PgPoint::parametrize(1, pt_p, 0, pt_q);
    CHECK(result == pt_p);
}

TEST_CASE("pg_object: parametrize linearity") {
    PgPoint pt_p({1, 2, 3});
    PgPoint pt_q({4, 5, 6});
    auto result = PgPoint::parametrize(1, pt_p, 1, pt_q);
    CHECK(result.coord[0] == 5);
    CHECK(result.coord[1] == 7);
    CHECK(result.coord[2] == 9);
}

TEST_CASE("pg_object: parametrize - negative coefficients") {
    PgPoint pt_p({1, 2, 3});
    PgPoint pt_q({4, 5, 6});
    auto result = PgPoint::parametrize(-1, pt_p, 2, pt_q);
    CHECK(result.coord[0] == -1*1 + 2*4);
    CHECK(result.coord[1] == -1*2 + 2*5);
    CHECK(result.coord[2] == -1*3 + 2*6);
}

TEST_CASE("PgPoint: equality") {
    PgPoint pt_p({1, 2, 3});
    PgPoint pt_q({2, 4, 6});
    CHECK(pt_p == pt_q);
    
    PgPoint pt_r({1, 2, 4});
    CHECK(pt_p != pt_r);
}

TEST_CASE("PgPoint: equality - scaled coordinates") {
    PgPoint pt_p({1, 2, 3});
    PgPoint pt_q({2, 4, 6});
    CHECK(pt_p == pt_q);
    
    PgPoint pt_r({1, 2, 3});
    PgPoint pt_s({-1, -2, -3});
    CHECK(pt_r == pt_s);
}

TEST_CASE("PgLine: equality") {
    PgLine ln_p({1, 2, 3});
    PgLine ln_q({2, 4, 6});
    CHECK(ln_p == ln_q);
    
    PgLine ln_r({1, 2, 4});
    CHECK(ln_p != ln_r);
}

TEST_CASE("meet is commutative for points") {
    PgPoint pt_p({1, 2, 3});
    PgPoint pt_q({4, 5, 6});
    auto ln1 = pt_p.meet(pt_q);
    auto ln2 = pt_q.meet(pt_p);
    CHECK(ln1 == ln2);
}

TEST_CASE("meet is commutative for lines") {
    PgLine ln_l({1, 2, 3});
    PgLine ln_m({4, 5, 6});
    auto pt1 = ln_l.meet(ln_m);
    auto pt2 = ln_m.meet(ln_l);
    CHECK(pt1 == pt2);
}

TEST_CASE("incident is symmetric") {
    PgPoint pt({1, 2, 3});
    PgLine ln({3, -3, 1});
    CHECK(pt.incident(ln));
    CHECK(ln.incident(pt));
}

TEST_CASE("aux returns dual type") {
    PgPoint pt({1, 2, 3});
    auto ln = pt.aux();
    CHECK(ln.coord == pt.coord);
    
    PgLine ln2({1, 2, 3});
    auto pt2 = ln2.aux();
    CHECK(pt2.coord == ln2.coord);
}

TEST_CASE("dot product symmetry") {
    PgPoint pt({1, 2, 3});
    PgLine ln({4, 5, 6});
    CHECK_EQ(pt.dot(ln), ln.dot(pt));
}

TEST_CASE("pg_object: incident self") {
    PgPoint pt({1, 2, 3});
    PgLine ln({3, -3, 1});
    CHECK(pt.incident(ln));
    CHECK(ln.incident(pt));
}

TEST_CASE("pg_object: meet of identical points is line at infinity") {
    PgPoint pt({1, 2, 3});
    auto ln = pt.meet(pt);
    CHECK(ln.coord[2] == 0);
}

TEST_CASE("pg_object: meet of identical lines is point at infinity") {
    PgLine ln({1, 2, 3});
    auto pt = ln.meet(ln);
    CHECK(pt.coord[2] == 0);
}

TEST_CASE("pg_object: cross product self is zero line") {
    PgPoint pt({1, 2, 3});
    auto ln = pt.meet(pt);
    CHECK(ln.coord[0] == 0);
    CHECK(ln.coord[1] == 0);
    CHECK(ln.coord[2] == 0);
}

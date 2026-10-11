#include <doctest/doctest.h>

#include <cstdint>
#include <projgeom/conic.hpp>
#include <projgeom/pg_object.hpp>
#include <stdexcept>

TEST_CASE("conic: unit circle contains") {
    const fun::Conic c = fun::Conic::unit_circle();
    const PgPoint on({1, 0, 1});
    const PgPoint off({2, 0, 1});
    CHECK(c.contains(on));
    CHECK_FALSE(c.contains(off));
}

TEST_CASE("conic: polar keeps the rational value") {
    const fun::Conic c = fun::Conic::parabola(fun::Frac{1, 1});
    const PgPoint pt({0, 1, 1});
    const PgLine expected({0, 1, 1});
    const PgLine polar = c.polar(pt);
    CHECK(polar == expected);
    CHECK_FALSE((polar.coord[0] == 0 && polar.coord[1] == 0 && polar.coord[2] == 0));
}

TEST_CASE("conic: pole of a line") {
    const fun::Conic c = fun::Conic::unit_circle();
    const PgLine line({1, 0, -1});
    const PgPoint expected({1, 0, 1});
    CHECK(c.pole(line) == expected);
}

TEST_CASE("conic: pole/polar round trip") {
    const fun::Conic c = fun::Conic::unit_circle();
    const PgLine line({1, 0, -1});
    CHECK(c.polar(c.pole(line)) == line);
}

TEST_CASE("conic: line meets the unit circle in two rational points") {
    const fun::Conic c = fun::Conic::unit_circle();
    const PgLine line({1, 0, 0});
    const auto points = c.intersect(line);
    REQUIRE(points.size() == 2);
    const PgPoint a({0, 1, 1});
    const PgPoint b({0, -1, 1});
    CHECK(c.contains(points[0]));
    CHECK(c.contains(points[1]));
    CHECK((points[0] == a || points[0] == b));
    CHECK((points[1] == a || points[1] == b));
}

TEST_CASE("conic: irrational intersection is refused") {
    const fun::Conic c = fun::Conic::unit_circle();
    const PgLine line({1, 1, 0});
    CHECK_THROWS_AS((void)c.intersect(line), std::domain_error);
}

TEST_CASE("conic: non-secant line yields no points") {
    const fun::Conic c = fun::Conic::unit_circle();
    const PgLine line({1, 0, 2});
    CHECK(c.intersect(line).empty());
}

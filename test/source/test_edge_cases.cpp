#include <doctest/doctest.h>

#include <array>
#include <cstdint>
#include <projgeom/pg_object.hpp>

TEST_CASE("Zero coordinates") {
    PgPoint pt({0, 0, 1});
    PgLine ln({1, 0, 0});
    CHECK(pt.incident(ln));
}

TEST_CASE("Origin point") {
    PgPoint pt({0, 0, 1});
    PgLine ln({0, 1, 0});
    CHECK(pt.incident(ln));
}

TEST_CASE("Line at infinity") {
    PgPoint pt({1, 2, 0});
    PgLine ln({0, 0, 1});
    CHECK(pt.incident(ln));
}

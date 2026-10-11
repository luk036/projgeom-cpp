#include <doctest/doctest.h>

#include <array>
#include <cstddef>
#include <projgeom/pg_object.hpp>
#include <projgeom/transform.hpp>

TEST_CASE("transform: identity") {
    const auto t = fun::Transform::identity();
    const PgPoint p({1, 2, 3});
    CHECK(t.apply_point(p) == p);
}

TEST_CASE("transform: fractional scaling keeps exact coordinates") {
    const PgPoint p({1, 1, 1});
    const PgPoint expected({3, 2, 6});
    CHECK(fun::scale_point(p, fun::Frac{1, 2}, fun::Frac{1, 3}) == expected);
}

TEST_CASE("transform: rotation from tangent") {
    const auto t = fun::Transform::rotation_from_tangent(fun::Frac{1, 1});
    const PgPoint p({1, 0, 1});
    const PgPoint expected({0, 1, 1});
    CHECK(t.apply_point(p) == expected);
}

TEST_CASE("transform: projective transform maps four points") {
    const std::array<PgPoint, 4> src{
        {PgPoint({0, 0, 1}), PgPoint({1, 0, 1}), PgPoint({0, 1, 1}), PgPoint({1, 1, 1})}};
    const std::array<PgPoint, 4> dst{
        {PgPoint({0, 0, 1}), PgPoint({2, 0, 1}), PgPoint({0, 2, 1}), PgPoint({2, 2, 1})}};
    const auto t = fun::projective_transform(src, dst);
    for (std::size_t i = 0; i < src.size(); ++i) {
        CHECK(t.apply_point(src[i]) == dst[i]);
    }
}

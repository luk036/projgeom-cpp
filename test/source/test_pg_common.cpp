#include <doctest/doctest.h>

#include <array>
#include <cstdint>
#include <projgeom/pg_common.hpp>

TEST_CASE("pg_common: cross0") {
    std::array<int64_t, 3> v = {1, 2, 3};
    std::array<int64_t, 3> w = {4, 5, 6};
    auto result = fun::cross0(v, w);
    CHECK_EQ(result, v[1] * w[2] - w[1] * v[2]);
}

TEST_CASE("pg_common: cross1") {
    std::array<int64_t, 3> v = {1, 2, 3};
    std::array<int64_t, 3> w = {4, 5, 6};
    auto result = fun::cross1(v, w);
    CHECK_EQ(result, v[0] * w[2] - w[0] * v[2]);
}

TEST_CASE("pg_common: cross2") {
    std::array<int64_t, 3> v = {1, 2, 3};
    std::array<int64_t, 3> w = {4, 5, 6};
    auto result = fun::cross2(v, w);
    CHECK_EQ(result, v[0] * w[1] - w[0] * v[1]);
}

TEST_CASE("pg_common: dot_c") {
    std::array<int64_t, 3> v = {1, 2, 3};
    std::array<int64_t, 3> w = {4, 5, 6};
    auto result = fun::dot_c(v, w);
    CHECK_EQ(result, 1 * 4 + 2 * 5 + 3 * 6);
}

TEST_CASE("pg_common: plucker_c") {
    std::array<int64_t, 3> v1 = {1, 2, 3};
    std::array<int64_t, 3> v2 = {4, 5, 6};
    auto result = fun::plucker_c<int64_t>(2, v1, 3, v2);
    CHECK_EQ(result[0], 14);
    CHECK_EQ(result[1], 19);
    CHECK_EQ(result[2], 24);
}

TEST_CASE("pg_common: dot1") {
    std::array<int64_t, 3> v = {1, 2, 3};
    std::array<int64_t, 3> w = {4, 5, 6};
    auto result = fun::dot1(v, w);
    CHECK_EQ(result, v[0] * w[0] + v[1] * w[1]);
}

TEST_CASE("pg_common: dot2") {
    std::array<int64_t, 3> v = {1, 2, 3};
    std::array<int64_t, 3> w = {4, 5, 6};
    auto result = fun::dot2(v, w);
    CHECK_EQ(result, v[0] * w[0] + v[2] * w[2]);
}

TEST_CASE("pg_common: sq") {
    CHECK_EQ(fun::sq(3), 9);
    CHECK_EQ(fun::sq(-4), 16);
    CHECK_EQ(fun::sq(0), 0);
}

TEST_CASE("pg_common: cross - zero vector") {
    std::array<int64_t, 3> v = {0, 0, 0};
    std::array<int64_t, 3> w = {1, 2, 3};
    auto result = fun::cross(v, w);
    CHECK(result[0] == 0);
    CHECK(result[1] == 0);
    CHECK(result[2] == 0);
}

TEST_CASE("pg_common: dot_c - zero vector") {
    std::array<int64_t, 3> v = {0, 0, 0};
    std::array<int64_t, 3> w = {1, 2, 3};
    auto result = fun::dot_c(v, w);
    CHECK_EQ(result, 0);
}

TEST_CASE("pg_common: plucker_c - zero case") {
    std::array<int64_t, 3> v1 = {0, 0, 0};
    std::array<int64_t, 3> v2 = {1, 2, 3};
    auto result = fun::plucker_c<int64_t>(0, v1, 1, v2);
    CHECK(result[0] == 1);
    CHECK(result[1] == 2);
    CHECK(result[2] == 3);
}

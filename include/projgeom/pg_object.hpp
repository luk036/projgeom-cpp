#pragma once

#include <array>
#include <cstdint>

// #include "common_concepts.h"
#include "pg_plane.hpp"

/**
 * @brief Dot product
 *
 * @param[in] a
 * @param[in] b
 * @return int64_t
 */
constexpr auto dot(const std::array<int64_t, 3> &a, const std::array<int64_t, 3> &b) -> int64_t {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

/**
 * @brief Cross product
 *
 * @param[in] a
 * @param[in] b
 * @return std::array<int64_t, 3>
 */
constexpr auto cross(const std::array<int64_t, 3> &a, const std::array<int64_t, 3> &b)
    -> std::array<int64_t, 3> {
    return {
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    };
}

/**
 * @brief Plucker operation
 *
 * @param[in] ld
 * @param[in] p
 * @param[in] mu
 * @param[in] q
 * @return std::array<int64_t, 3>
 */
constexpr auto plckr(const int64_t &ld, const std::array<int64_t, 3> &p, const int64_t &mu,
                     const std::array<int64_t, 3> &q) -> std::array<int64_t, 3> {
    return {
        ld * p[0] + mu * q[0],
        ld * p[1] + mu * q[1],
        ld * p[2] + mu * q[2],
    };
}

/**
 * @brief Projective Point/Line
 *
 * @tparam Point
 * @tparam L
 */
template <typename Point, typename L> struct PgObject {
    using Dual = L;

    std::array<int64_t, 3> coord;

    /**
     * @brief Construct a new Pg Object object
     *
     * @param[in] coord
     */
    constexpr explicit PgObject(std::array<int64_t, 3> coord) : coord{std::move(coord)} {}

    /**
     * @brief Equal to
     *
     * @param[in] other
     * @return true
     * @return false
     */
    friend constexpr auto operator==(const Point &lhs, const Point &rhs) -> bool {
        return &lhs == &rhs ? true
                            : lhs.coord[1] * rhs.coord[2] == lhs.coord[2] * rhs.coord[1]
                                  && lhs.coord[2] * rhs.coord[0] == lhs.coord[0] * rhs.coord[2]
                                  && lhs.coord[0] * rhs.coord[1] == lhs.coord[1] * rhs.coord[0];
    }

    /**
     * @brief Equal to
     *
     * @param[in] other
     * @return true
     * @return false
     */
    friend constexpr auto operator!=(const Point &lhs, const Point &rhs) -> bool { return !(lhs == rhs); }

    /**
     * @brief
     *
     * @return L
     */
    constexpr auto aux() const -> L { return L{this->coord}; }

    /**
     * @brief
     *
     * @param[in] other
     * @return int64_t
     */
    constexpr auto dot(const L &other) const -> int64_t { return ::dot(this->coord, other.coord); }

    /**
     * @brief
     *
     * @param[in] ld
     * @param[in] p
     * @param[in] mu
     * @param[in] q
     * @return Point
     */
    static constexpr auto plucker(const int64_t &ld, const Point &p, const int64_t &mu, const Point &q)
        -> Point {
        return Point{::plckr(ld, p.coord, mu, q.coord)};
    }

    /**
     * @brief
     *
     * @param[in] other
     * @return true
     * @return false
     */
    constexpr auto incident(const L &other) const -> bool { return this->dot(other) == 0; }

    /**
     * @brief
     *
     * @param[in] rhs
     * @return L
     */
    constexpr auto meet(const Point &rhs) const -> L { return L{::cross(this->coord, rhs.coord)}; }
};

class PgPoint;
class PgLine;

/**
 * @brief PG Point
 *
 */
class PgPoint : public PgObject<PgPoint, PgLine> {
  public:
    /**
     * @brief Construct a new Pg Point object
     *
     * @param[in] coord Homogeneous coordinate
     */
    constexpr explicit PgPoint(std::array<int64_t, 3> coord)
        : PgObject<PgPoint, PgLine>{std::move(coord)} {}
};

/**
 * @brief PG Line
 *
 */
class PgLine : public PgObject<PgLine, PgPoint> {
  public:
    /**
     * @brief Construct a new Pg Line object
     *
     * @param[in] coord Homogeneous coordinate
     */
    constexpr explicit PgLine(std::array<int64_t, 3> coord)
        : PgObject<PgLine, PgPoint>{std::move(coord)} {}
};

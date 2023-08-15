#pragma once

#include <array>
#include <cstdint>

// #include "common_concepts.h"
#include "pg_plane.hpp"

/**
 * @brief Dot product
 *
 * @param[in] pt_a
 * @param[in] pt_b
 * @return int64_t
 */
constexpr auto dot(const std::array<int64_t, 3> &pt_a, const std::array<int64_t, 3> &pt_b) -> int64_t {
    return pt_a[0] * pt_b[0] + pt_a[1] * pt_b[1] + pt_a[2] * pt_b[2];
}

/**
 * @brief Cross product
 *
 * @param[in] pt_a
 * @param[in] pt_b
 * @return std::array<int64_t, 3>
 */
constexpr auto cross(const std::array<int64_t, 3> &pt_a, const std::array<int64_t, 3> &pt_b)
    -> std::array<int64_t, 3> {
    return {
        pt_a[1] * pt_b[2] - pt_a[2] * pt_b[1],
        pt_a[2] * pt_b[0] - pt_a[0] * pt_b[2],
        pt_a[0] * pt_b[1] - pt_a[1] * pt_b[0],
    };
}

/**
 * @brief Plucker operation
 *
 * @param[in] lambda
 * @param[in] pt_p
 * @param[in] mu
 * @param[in] pt_q
 * @return std::array<int64_t, 3>
 */
constexpr auto plckr(const int64_t &lambda, const std::array<int64_t, 3> &pt_p, const int64_t &mu,
                     const std::array<int64_t, 3> &pt_q) -> std::array<int64_t, 3> {
    return {
        lambda * pt_p[0] + mu * pt_q[0],
        lambda * pt_p[1] + mu * pt_q[1],
        lambda * pt_p[2] + mu * pt_q[2],
    };
}

/**
 * @brief Projective Point/Line
 *
 * @tparam Point
 * @tparam Line
 */
template <typename Point, typename Line> struct PgObject {
    using Dual = Line;

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
    friend constexpr auto operator!=(const Point &lhs, const Point &rhs) -> bool {
        return !(lhs == rhs);
    }

    /**
     * @brief
     *
     * @return Line
     */
    constexpr auto aux() const -> Line { return Line{this->coord}; }

    /**
     * @brief
     *
     * @param[in] other
     * @return int64_t
     */
    constexpr auto dot(const Line &other) const -> int64_t { return ::dot(this->coord, other.coord); }

    /**
     * @brief
     *
     * @param[in] lambda
     * @param[in] pt_p
     * @param[in] mu
     * @param[in] pt_q
     * @return Point
     */
    static constexpr auto plucker(const int64_t &lambda, const Point &pt_p, const int64_t &mu,
                                  const Point &pt_q) -> Point {
        return Point{::plckr(lambda, pt_p.coord, mu, pt_q.coord)};
    }

    /**
     * @brief
     *
     * @param[in] other
     * @return true
     * @return false
     */
    constexpr auto incident(const Line &other) const -> bool { return this->dot(other) == 0; }

    /**
     * @brief
     *
     * @param[in] rhs
     * @return Line
     */
    constexpr auto meet(const Point &rhs) const -> Line { return Line{::cross(this->coord, rhs.coord)}; }
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

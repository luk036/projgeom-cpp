#pragma once

#include "ck_plane.hpp"
#include "pg_object.hpp"

class HypPoint;
class HypLine;

/**
 * @brief Hyperbolic Point
 *
 */
class HypPoint : public PgObject<HypPoint, HypLine> {
  public:
    /**
     * @brief Construct a new Hyp Point object
     *
     * @param[in] coord Homogeneous coordinate
     */
    constexpr explicit HypPoint(std::array<int64_t, 3> coord)
        : PgObject<HypPoint, HypLine>{coord} {}

    /**
     * @brief
     *
     * @return HypLine
     */
    constexpr auto perp() const -> HypLine;
};

/**
 * @brief Hyperbolic Line
 *
 */
class HypLine : public PgObject<HypLine, HypPoint> {
  public:
    /**
     * @brief Construct a new Hyp Line object
     *
     * @param[in] coord Homogeneous coordinate
     */
    constexpr explicit HypLine(std::array<int64_t, 3> coord)
        : PgObject<HypLine, HypPoint>{coord} {}

    /**
     * @brief
     *
     * @return HypPoint
     */
    constexpr auto perp() const -> HypPoint;
};

/**
 * @brief
 *
 * @return HypLine
 */
inline constexpr auto HypPoint::perp() const -> HypLine {
    return HypLine({this->coord[0], this->coord[1], -this->coord[2]});
}

/**
 * @brief
 *
 * @return HypPoint
 */
inline constexpr auto HypLine::perp() const -> HypPoint {
    return HypPoint({this->coord[0], this->coord[1], -this->coord[2]});
}

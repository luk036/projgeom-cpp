#pragma once

#include "ck_plane.hpp"
#include "pg_object.hpp"

class HyperbolicPoint;
class HyperbolicLine;

/**
 * @brief Hyperbolic Point
 *
 */
class HyperbolicPoint : public PgObject<HyperbolicPoint, HyperbolicLine> {
  public:
    /**
     * @brief Construct a new Hyperbolic Point object
     *
     * @param[in] coord Homogeneous coordinate
     */
    constexpr explicit HyperbolicPoint(std::array<int64_t, 3> coord)
        : PgObject<HyperbolicPoint, HyperbolicLine>{coord} {}

    /**
     * @brief Polar
     *
     * @return HyperbolicLine
     */
    constexpr auto perp() const -> HyperbolicLine;  // polar
};

/**
 * @brief Hyperbolic Line
 *
 */
class HyperbolicLine : public PgObject<HyperbolicLine, HyperbolicPoint> {
  public:
    /**
     * @brief Construct a new Hyperbolic Line object
     *
     * @param[in] coord Homogeneous coordinate
     */
    constexpr explicit HyperbolicLine(std::array<int64_t, 3> coord)
        : PgObject<HyperbolicLine, HyperbolicPoint>{coord} {}

    /**
     * @brief Pole
     *
     * @return HyperbolicPoint
     */
    constexpr auto perp() const -> HyperbolicPoint;  // pole
};

/**
 * @brief Polar
 *
 * @return HyperbolicLine
 */
constexpr auto HyperbolicPoint::perp() const -> HyperbolicLine {
    return HyperbolicLine({this->coord[0], this->coord[1], -this->coord[2]});
}

/**
 * @brief Pole
 *
 * @return HyperbolicPoint
 */
constexpr auto HyperbolicLine::perp() const -> HyperbolicPoint {
    return HyperbolicPoint({this->coord[0], this->coord[1], -this->coord[2]});
}

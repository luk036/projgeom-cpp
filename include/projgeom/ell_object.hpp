#pragma once

#include "ck_plane.hpp"
#include "pg_object.hpp"

class EllipticPoint;
class EllipticLine;

/**
 * @brief Elliptic Point
 *
 */
class EllipticPoint : public PgObject<EllipticPoint, EllipticLine> {
  public:
    /**
     * @brief Construct a new Elliptic Point object
     *
     * @param[in] coord Homogeneous coordinate
     */
    constexpr explicit EllipticPoint(std::array<int64_t, 3> coord)
        : PgObject<EllipticPoint, EllipticLine>{coord} {}

    /**
     * @brief Polar
     *
     * @return EllipticLine
     */
    constexpr auto perp() const -> EllipticLine;  // polar
};

/**
 * @brief Elliptic Line
 *
 */
class EllipticLine : public PgObject<EllipticLine, EllipticPoint> {
  public:
    /**
     * @brief Construct a new Elliptic Line object
     *
     * @param[in] coord Homogeneous coordinate
     */
    constexpr explicit EllipticLine(std::array<int64_t, 3> coord)
        : PgObject<EllipticLine, EllipticPoint>{coord} {}

    /**
     * @brief Pole
     *
     * @return EllipticPoint
     */
    constexpr auto perp() const -> EllipticPoint;  // pole
};

/**
 * @brief Polar
 *
 * @return EllipticLine
 */
constexpr auto EllipticPoint::perp() const -> EllipticLine {
    return EllipticLine{this->coord};
}

/**
 * @brief Pole
 *
 * @return EllipticPoint
 */
constexpr auto EllipticLine::perp() const -> EllipticPoint {
    return EllipticPoint{this->coord};
}

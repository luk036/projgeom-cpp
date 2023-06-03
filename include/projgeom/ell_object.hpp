#pragma once

#include "ck_plane.hpp"
#include "pg_object.hpp"

class EllPoint;
class EllLine;

/**
 * @brief Elliptic Point
 *
 */
class EllPoint : public PgObject<EllPoint, EllLine> {
public:
  /**
   * @brief Construct a new Ell Point object
   *
   * @param[in] coord Homogeneous coordinate
   */
  constexpr explicit EllPoint(std::array<int64_t, 3> coord)
      : PgObject<EllPoint, EllLine>{coord} {}

  /**
   * @brief
   *
   * @return EllLine
   */
  constexpr auto perp() const -> EllLine;
};

/**
 * @brief Elliptic Line
 *
 */
class EllLine : public PgObject<EllLine, EllPoint> {
public:
  /**
   * @brief Construct a new Ell Line object
   *
   * @param[in] coord Homogeneous coordinate
   */
  constexpr explicit EllLine(std::array<int64_t, 3> coord)
      : PgObject<EllLine, EllPoint>{coord} {}

  /**
   * @brief
   *
   * @return EllPoint
   */
  constexpr auto perp() const -> EllPoint;
};

/**
 * @brief
 *
 * @return EllLine
 */
inline constexpr auto EllPoint::perp() const -> EllLine {
  return EllLine{this->coord};
}

/**
 * @brief
 *
 * @return EllPoint
 */
inline constexpr auto EllLine::perp() const -> EllPoint {
  return EllPoint{this->coord};
}

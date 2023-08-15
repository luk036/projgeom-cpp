#pragma once

#include "ck_plane.hpp"
#include "pg_object.hpp"

class MyCKPoint;
class MyCKLine;

/**
 * @brief Some Cayley-Klein Point
 *
 */
class MyCKPoint : public PgObject<MyCKPoint, MyCKLine> {
  public:
    /**
     * @brief Construct a new My C K Point object
     *
     * @param[in] coord Homogeneous coordinate
     */
    constexpr explicit MyCKPoint(std::array<int64_t, 3> coord)
        : PgObject<MyCKPoint, MyCKLine>{coord} {}

    /**
     * @brief
     *
     * @return MyCKLine
     */
    constexpr auto perp() const -> MyCKLine;
};

/**
 * @brief Some Cayley-Klein Line
 *
 */
class MyCKLine : public PgObject<MyCKLine, MyCKPoint> {
  public:
    /**
     * @brief Construct a new My C K Line object
     *
     * @param[in] coord Homogeneous coordinate
     */
    constexpr explicit MyCKLine(std::array<int64_t, 3> coord)
        : PgObject<MyCKLine, MyCKPoint>{coord} {}

    /**
     * @brief
     *
     * @return MyCKPoint
     */
    constexpr auto perp() const -> MyCKPoint;
};

/**
 * @brief
 *
 * @return MyCKLine
 */
inline constexpr auto MyCKPoint::perp() const -> MyCKLine {
    return MyCKLine({-2 * this->coord[0], this->coord[1], -2 * this->coord[2]});
}

/**
 * @brief
 *
 * @return MyCKPoint
 */
inline constexpr auto MyCKLine::perp() const -> MyCKPoint {
    return MyCKPoint({-this->coord[0], 2 * this->coord[1], -this->coord[2]});
}

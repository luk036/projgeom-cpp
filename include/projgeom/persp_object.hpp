#pragma once

#include "ck_plane.hpp"
#include "pg_object.hpp"

class PerspPoint;
class PerspLine;

/**
 * @brief Perspective Point
 *
 */
class PerspPoint : public PgObject<PerspPoint, PerspLine> {
  public:
    // constexpr static PerspLine L_INF{ {0, -1, 1}};

    /**
     * @brief Construct a new Persp Point object
     *
     * @param[in] coord Homogeneous coordinate
     */
    constexpr explicit PerspPoint(std::array<int64_t, 3> coord)
        : PgObject<PerspPoint, PerspLine>{coord} {}

    /**
     * @brief
     *
     * @return const PerspLine&
     */
    constexpr auto perp() const -> const PerspLine &;
};

/**
 * @brief Perspective Line
 *
 */
class PerspLine : public PgObject<PerspLine, PerspPoint> {
  public:
    // constexpr static PerspPoint I_RE{ {0, 1, 1}};
    // constexpr static PerspPoint I_IM{ {1, 0, 0}};

    /**
     * @brief Construct a new Persp Line object
     *
     * @param[in] coord Homogeneous coordinate
     */
    constexpr explicit PerspLine(std::array<int64_t, 3> coord)
        : PgObject<PerspLine, PerspPoint>{coord} {}

    /**
     * @brief
     *
     * @return PerspPoint
     */
    constexpr auto perp() const -> PerspPoint;
};

static constexpr PerspLine L_INF({0, -1, 1});
static constexpr PerspPoint I_RE({0, 1, 1});
static constexpr PerspPoint I_IM({1, 0, 0});

/**
 * @brief
 *
 * @return const PerspLine&
 */
constexpr auto PerspPoint::perp() const -> const PerspLine & { return L_INF; }

/**
 * @brief
 *
 * @return PerspPoint
 */
constexpr auto PerspLine::perp() const -> PerspPoint {
    return PerspPoint::parametrize(this->dot(I_RE), I_RE, this->dot(I_IM), I_IM);
}

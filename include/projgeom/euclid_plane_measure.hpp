#pragma once

#include "euclid_plane.hpp"
#include "fractions.hpp"

namespace fun {

    /**
     * @brief
     *
     * @tparam K
     * @param[in] x1
     * @param[in] z1
     * @param[in] x2
     * @param[in] z2
     * @return auto
     */
    template <typename K>
        requires Integral<K>
    inline constexpr auto quad1(const K &x1, const K &z1, const K &x2, const K &z2) {
        return sq(Fraction<K>(x1, z1) - Fraction<K>(x2, z2));
    }

    /**
     * @brief
     *
     * @tparam K
     * @param[in] x1
     * @param[in] z1
     * @param[in] x2
     * @param[in] z2
     * @return auto
     */
    template <typename K>
    // requires (!Integral<K>)
    inline constexpr auto quad1(const K &x1, const K &z1, const K &x2, const K &z2) {
        return sq(x1 / z1 - x2 / z2);
    }

    /**
     * @brief
     *
     * @param[in] a1
     * @param[in] a2
     * @return auto
     */
    template <ProjectivePlaneCoord2 Point>
    inline constexpr auto quadrance(const Point &a1, const Point &a2) {
        return quad1(a1[0], a1[2], a2[0], a2[2]) + quad1(a1[1], a1[2], a2[1], a2[2]);
    }

    template <typename... Args> inline constexpr auto quadrance_copy(const Args &...args) {
        return std::make_tuple(quadrance(args.first, args.second)...);
    }

    // ProjectivePlane2 { Line }
    // constexpr auto sbase(const Line &l1, const Line &l2, const Integer &d) {
    //     return Fraction(d, omgB(l1, l1)) * Fraction(d, omgB(l2, l2));
    // }

    /**
     * @brief
     *
     * @param[in] l1
     * @param[in] l2
     * @param[in] d
     * @return auto
     */
    template <ProjectivePlaneCoord2 Line, typename T>
    inline constexpr auto sbase(const Line &l1, const Line &l2, const T &d) {
        using K = Value_type<Line>;
        if constexpr (Integral<K>) {
            return Fraction<K>(d, dot1(l1, l1)) * Fraction<K>(d, dot1(l2, l2));
        } else {
            return (d * d) / (dot1(l1, l1) * dot1(l2, l2));
        }
    }

    /**
     * @brief
     *
     * @param[in] l1
     * @param[in] l2
     * @return auto
     */
    template <ProjectivePlaneCoord2 Line>
    inline constexpr auto spread(const Line &l1, const Line &l2) {
        return sbase(l1, l2, cross2(l1, l2));
    }

    /**
     * @brief
     *
     * @param[in] triangle
     * @return auto
     */
    template <ProjectivePlaneCoord2 Point>
    inline constexpr auto tri_quadrance(const Triple<Point> &triangle) {
        const auto &[a1, a2, a3] = triangle;
        return std::array{quadrance(a2, a3), quadrance(a1, a3), quadrance(a1, a2)};
    }

    /**
     * @brief
     *
     * @param[in] trilateral
     * @return auto
     */
    template <ProjectivePlaneCoord2 Line>
    inline constexpr auto tri_spread(const Triple<Line> &trilateral) {
        const auto &[a1, a2, a3] = trilateral;
        return std::array{spread(a2, a3), spread(a1, a3), spread(a1, a2)};
    }

    /**
     * @brief
     *
     * @param[in] l1
     * @param[in] l2
     * @return auto
     */
    template <ProjectivePlaneCoord2 Line>
    inline constexpr auto cross_s(const Line &l1, const Line &l2) {
        return sbase(l1, l2, dot1(l1, l2));
    }

#include <cmath>

    /**
     * @brief
     *
     * @param[in] a
     * @param[in] b
     * @return auto
     */
    template <ProjectivePlaneCoord2 Point>
    inline constexpr auto distance(const Point &a, const Point &b) {
        return std::sqrt(double(quadrance(a, b)));
    }

    /**
     * @brief
     *
     * @param[in] line_l
     * @param[in] line_m
     * @return auto
     */
    template <ProjectivePlaneCoord2 Line>
    inline constexpr auto angle(const Line &line_l, const Line &line_m) {
        return std::asin(std::sqrt(double(spread(line_l, line_m))));
    }

}  // namespace fun

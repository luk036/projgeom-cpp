#pragma once

#include "euclid_plane.hpp"
#include "fractions.hpp"

namespace fun {

    /**
     * @brief Compute squared difference of ratios (integral version).
     *
     * Computes ((x1/z1) - (x2/z2))^2 for integral types.
     * @param[in] x1 First numerator
     * @param[in] z1 First denominator
     * @param[in] x2 Second numerator
     * @param[in] z2 Second denominator
     * @return auto The squared difference of ratios
     */
    template <typename K>
        requires Integral<K>
    constexpr auto quad1(const K& x1, const K& z1, const K& x2, const K& z2) {
        return sq(Fraction<K>(x1, z1) - Fraction<K>(x2, z2));
    }

    /**
     * @brief Compute squared difference of ratios (non-integral version).
     *
     * Computes ((x1/z1) - (x2/z2))^2 for non-integral types.
     * @return auto The squared difference of ratios
     */
    template <typename K>
    // requires (!Integral<K>)
    constexpr auto quad1(const K& x1, const K& z1, const K& x2, const K& z2) {
        return sq(x1 / z1 - x2 / z2);
    }

    /**
     * @brief Compute the quadrance between two points.
     *
     * The quadrance is the squared distance between two points in the
     * Euclidean plane, represented using homogeneous coordinates.
     * @param[in] a1 First point
     * @param[in] a2 Second point
     * @return auto The quadrance (squared distance)
     */
    template <ProjectivePlaneCoord2 Point>
    constexpr auto quadrance(const Point& a1, const Point& a2) {
        return quad1(a1[0], a1[2], a2[0], a2[2]) + quad1(a1[1], a1[2], a2[1], a2[2]);
    }

    /**
     * @brief Compute quadrances for multiple pairs of points.
     *
     * Returns a tuple of quadrances for each pair of points.
     * @param[in] args Pairs of points
     * @return std::tuple of quadrance values
     */
    template <typename... Args> constexpr auto quadrance_copy(const Args&... args) {
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
    constexpr auto sbase(const Line& l1, const Line& l2, const T& d) {
        using K = Value_type<Line>;
        if constexpr (Integral<K>) {
            return Fraction<K>(d, dot1(l1, l1)) * Fraction<K>(d, dot1(l2, l2));
        } else {
            return (d * d) / (dot1(l1, l1) * dot1(l2, l2));
        }
    }

    /**
     * @brief Compute the spread between two lines.
     *
     * The spread is the squared sine of the angle between two lines,
     * a fundamental measure in Euclidean geometry.
     * @param[in] l1 First line
     * @param[in] l2 Second line
     * @return auto The spread
     */
    template <ProjectivePlaneCoord2 Line> constexpr auto spread(const Line& l1, const Line& l2) {
        return sbase(l1, l2, cross2(l1, l2));
    }

    /**
     * @brief Compute the quadrances of a triangle's sides.
     *
     * Returns an array with the quadrances of the three sides of the triangle.
     * @param[in] triangle Array of three points
     * @return auto Array of three quadrance values
     */
    template <ProjectivePlaneCoord2 Point>
    constexpr auto tri_quadrance(const Triple<Point>& triangle) {
        const auto& [a1, a2, a3] = triangle;
        return std::array{quadrance(a2, a3), quadrance(a1, a3), quadrance(a1, a2)};
    }

    /**
     * @brief Compute the spreads of a triangle's angles.
     *
     * Returns an array with the spreads of the three angles of the triangle.
     * @param[in] trilateral Array of three lines
     * @return auto Array of three spread values
     */
    template <ProjectivePlaneCoord2 Line>
    constexpr auto tri_spread(const Triple<Line>& trilateral) {
        const auto& [a1, a2, a3] = trilateral;
        return std::array{spread(a2, a3), spread(a1, a3), spread(a1, a2)};
    }

    /**
     * @brief Compute the cross spread between two lines.
     *
     * The cross spread is related to the cosine squared of the angle between lines.
     * @param[in] l1 First line
     * @param[in] l2 Second line
     * @return auto The cross spread
     */
    template <ProjectivePlaneCoord2 Line> constexpr auto cross_s(const Line& l1, const Line& l2) {
        return sbase(l1, l2, dot1(l1, l2));
    }

#include <cmath>

    /**
     * @brief Compute the Euclidean distance between two points.
     *
     * @param[in] a First point
     * @param[in] b Second point
     * @return auto The Euclidean distance
     */
    template <ProjectivePlaneCoord2 Point> constexpr auto distance(const Point& a, const Point& b) {
        return std::sqrt(double(quadrance(a, b)));
    }

    /**
     * @brief Compute the angle between two lines.
     *
     * Returns the angle between two lines using arcsin of the square root of spread.
     * @param[in] line_l First line
     * @param[in] line_m Second line
     * @return auto The angle in radians
     */
    template <ProjectivePlaneCoord2 Line>
    constexpr auto angle(const Line& line_l, const Line& line_m) {
        return std::asin(std::sqrt(double(spread(line_l, line_m))));
    }

}  // namespace fun

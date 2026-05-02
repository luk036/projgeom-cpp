#pragma once

#include "fractions.hpp"
#include "proj_plane.hpp"

/** @file proj_plane_measure.hpp
 *  This is a C++ Library header.
 */

/**
 @todo: projectivity >=
**/

namespace fun {

    /**
     * @brief Compute the ratio of two ratios.
     *
     * Computes (a/b) / (c/d) for ring elements.
     * @param[in] a First numerator
     * @param[in] b First denominator
     * @param[in] c Second numerator
     * @param[in] d Second denominator
     * @return auto The ratio of ratios
     */
    template <Ring K> constexpr auto ratio_ratio(const K& a, const K& b, const K& c, const K& d) {
        if constexpr (Integral<K>) {
            return Fraction(a, b) / Fraction(c, d);
        } else {
            return (a * d) / (b * c);
        }
    }

    /**
     * @brief Cross Ratio
     *
     * @tparam Point
     * @tparam Line
     * @param[in] A point A
     * @param[in] B point B
     * @param[in] line_l line l
     * @param[in] line_m line m
     * @return cross ratio R(A,B;line_l,line_m)
     *
     * @todo rewrite by projecting to the y-axis first [:2]
     */
    template <typename Point, typename Line>
        requires ProjectivePlane<Point, Line>
    constexpr auto x_ratio(const Point& A, const Point& B, const Line& line_l, const Line& line_m) {
        return ratio_ratio(A.dot(line_l), A.dot(line_m), B.dot(line_l), B.dot(line_m));
    }

    /**
     * @brief Compute the cross ratio of four collinear points.
     *
     * Returns the cross ratio R(A, B; C, D) of four points on a line.
     * Automatically chooses the best coordinate projection based on the points.
     * @param[in] A First point
     * @param[in] B Second point
     * @param[in] C Third point
     * @param[in] D Fourth point
     * @return constexpr auto The cross ratio
     */
    template <ProjectivePlaneCoord2 Point>
    constexpr auto R(const Point& A, const Point& B, const Point& C, const Point& D)

    {
        using K = Value_type<Point>;
        if (cross0(A, B) != K(0)) {  // Project points to yz-plane
            return R0(A, B, C, D);
        }
        // Project points to xz-plane
        return R1(A, B, C, D);
    }

    /**
     * @brief Compute the cross ratio using projective construction.
     *
     * Returns the cross ratio R(A, B; C, D) using a projective construction
     * that works with arbitrary projective planes.
     * @return constexpr auto The cross ratio
     */
    template <ProjectivePlane2 Point>
    constexpr auto R(const Point& A, const Point& B, const Point& C, const Point& D) {
        const auto O = (C * D).aux();
        return x_ratio(A, B, O * C, O * D);
    }

    /**
     * @brief Compute cross ratio using yz-plane projection.
     *
     * Returns the cross ratio R(A, B; C, D) using the yz-plane projection.
     * @param[in] A First point
     * @param[in] B Second point
     * @param[in] C Third point
     * @param[in] D Fourth point
     * @return constexpr auto The cross ratio
     */
    template <ProjectivePlaneCoord2 Point>
    constexpr auto R0(const Point& A, const Point& B, const Point& C, const Point& D) {
        return ratio_ratio(cross0(A, C), cross0(A, D), cross0(B, C), cross0(B, D));
    }

    /**
     * @brief Compute cross ratio using xz-plane projection.
     *
     * Returns the cross ratio R(A, B; C, D) using the xz-plane projection.
     * @param[in] A First point
     * @param[in] B Second point
     * @param[in] C Third point
     * @param[in] D Fourth point
     * @return constexpr auto The cross ratio
     */
    template <ProjectivePlaneCoord2 Point>
    constexpr auto R1(const Point& A, const Point& B, const Point& C, const Point& D) {
        return ratio_ratio(cross1(A, C), cross1(A, D), cross1(B, C), cross1(B, D));
    }

}  // namespace fun

#pragma once

#include <type_traits>

#include "pg_common.hpp"   // import cross2, dot1
#include "proj_plane.hpp"  // import pg_point, Involution, tri_func, quad_func, parametrize
#include "proj_plane_concepts.h"

namespace fun {

    /**
     * @brief Convert a line to its direction vector in the affine plane.
     *
     * Extracts the direction components (first two coordinates) of a line,
     * effectively treating the line as a direction vector in the Euclidean plane.
     * @param[in] line_l The line to extract direction from
     * @return The dual type representing the direction vector
     */
    template <ProjectivePlaneCoord2 Line>  // // and requires point_p[i]
    constexpr auto fB(const Line &line_l) -> typename Line::Dual {
        return {line_l[0], line_l[1], 0};
    }

    /**
     * @brief Check if two lines are perpendicular.
     *
     * Uses the dot product of direction vectors to determine
     * if two lines meet at a right angle.
     * @param[in] line_l First line
     * @param[in] line_m Second line
     * @return true if lines are perpendicular, false otherwise
     */
    template <ProjectivePlaneCoord2 Line>
    constexpr auto is_perpendicular(const Line &line_l, const Line &line_m) -> bool {
        return dot1(line_l, line_m) == 0;
    }

    /**
     * @brief Check if two lines are parallel.
     *
     * Uses the cross product of direction vectors to determine
     * if two lines have the same direction.
     * @param[in] line_l First line
     * @param[in] line_m Second line
     * @return true if lines are parallel, false otherwise
     */
    template <ProjectivePlaneCoord2 Line>
    constexpr auto is_parallel(const Line &line_l, const Line &line_m) -> bool {
        return cross2(line_l, line_m) == 0;
    }

    /**
     * @brief Compute the altitude from a point to a line.
     *
     * The altitude is the line through the given point that is perpendicular
     * to the given line.
     * @param[in] a The point from which the altitude is drawn
     * @param[in] line_l The line to which the altitude is drawn
     * @return Line The altitude line
     */
    template <typename Point, typename Line>
        requires ProjectivePlaneCoord<Point, Line>
    constexpr auto altitude(const Point &a, const Line &line_l) -> Line {
        return a * fB(line_l);
    }

    /**
     * @brief Compute all three altitudes of a triangle.
     *
     * Returns an array containing the three altitude lines of the triangle.
     * @param[in] triangle Array of three non-collinear points
     * @return std::array<Line, 3> The three altitude lines
     */
    template <ProjectivePlaneCoord2 Point>
    constexpr auto tri_altitude(const Triple<Point> &triangle) {
        const auto &[a1, a2, a3] = triangle;
        return std::array{altitude(a1, a2 * a3), altitude(a2, a3 * a1), altitude(a3, a1 * a2)};
    }

    /**
     * @brief Compute the orthocenter of a triangle.
     *
     * The orthocenter is the intersection point of all three altitudes of a triangle.
     * @param[in] triangle Array of three non-collinear points
     * @return Point The orthocenter of the triangle
     */
    template <ProjectivePlaneCoord2 Point>
    constexpr auto orthocenter(const Triple<Point> &triangle) -> Point {
        const auto &[a1, a2, a3] = triangle;
        const auto t1 = altitude(a1, a2 * a3);
        const auto t2 = altitude(a2, a1 * a3);
        return t1 * t2;
    }

    /**
     * @brief Create an involution that reflects across a line.
     *
     * Returns an involution that performs reflection across the given line
     * in the Euclidean plane.
     * @param[in] line_m The line of reflection (mirror)
     * @return Involution The reflection involution
     */
    template <ProjectivePlaneCoord2 Line> constexpr auto reflect(const Line &line_m) {
        return Involution{line_m, fB(line_m)};
    }

    /**
     * @brief Compute the midpoint of two points.
     *
     * Returns the point that divides the segment AB in the ratio 1:1.
     * @param[in] a First point
     * @param[in] b Second point
     * @return Point The midpoint
     */
    template <ProjectivePlaneCoord2 Point>
    constexpr auto midpoint(const Point &a, const Point &b) -> Point {
        return parametrize(b[2], a, a[2], b);
    }

    /**
     * @brief Compute the midpoints of all three sides of a triangle.
     *
     * Returns an array containing the midpoints of each side of the triangle.
     * @param[in] triangle Array of three points
     * @return Triple<Point> Array of three midpoints
     */
    template <ProjectivePlaneCoord2 Point>
    constexpr auto tri_midpoint(const Triple<Point> &triangle) -> Triple<Point> {
        const auto &[a1, a2, a3] = triangle;
        return {midpoint(a1, a2), midpoint(a2, a3), midpoint(a1, a3)};
    }

    /**
     * @brief Compute a point on the unit circle from trigonometric parameters.
     *
     * Creates a point on the unit circle using the parameterization
     * (lambda^2 - mu^2, 2*lambda*mu, lambda^2 + mu^2).
     * @param[in] lda1 The lambda parameter
     * @param[in] mu1 The mu parameter
     * @return Point A point on the unit circle
     */
    template <ProjectivePlaneCoord2 Point>
    constexpr auto uc_point(const Value_type<Point> &lda1, const Value_type<Point> &mu1) {
        const auto lda2 = lda1 * lda1;
        const auto mu2 = mu1 * mu1;
        return Point{lda2 - mu2, 2 * lda1 * mu1, lda2 + mu2};
    }

    /**
     * @brief Archimedes's function
     *
     * @tparam _Q
     * @param[in] a
     * @param[in] b
     * @param[in] c
     * @return auto
     */
    template <OrderedRing _Q> constexpr auto archimedes(const _Q &a, const _Q &b, const _Q &c) {
        return 4 * a * b - sq(a + b - c);
    }

    /**
     * @brief Cyclic quadrilateral quadrea theorem
     *
     * @tparam _Q
     * @param[in] a
     * @param[in] b
     * @param[in] c
     * @param[in] d
     * @return auto
     */
    template <typename _Q>
    constexpr auto cqq(const _Q &a, const _Q &b, const _Q &c, const _Q &d) -> std::array<_Q, 2> {
        const auto t1 = 4 * a * b;
        const auto t2 = 4 * c * d;
        auto line_m = (t1 + t2) - sq(a + b - c - d);
        auto point_p = line_m * line_m - 4 * t1 * t2;
        return {std::move(line_m), std::move(point_p)};
    }

    /**
     * @brief Check Ptolemy's theorem for a cyclic quadrilateral.
     *
     * Ptolemy's theorem states that for a cyclic quadrilateral,
     * the product of the diagonals equals the sum of the products
     * of opposite sides.
     * @param[in] quad Array of six side/diagonal measurements {Q12, Q23, Q34, Q14, Q13, Q24}
     * @return true if the quadrilateral satisfies Ptolemy's theorem
     */
    template <typename T> constexpr auto Ptolemy(const T &quad) -> bool {
        const auto &[Q12, Q23, Q34, Q14, Q13, Q24] = quad;
        using _K = decltype(Q12);
        return archimedes(Q12 * Q34, Q23 * Q14, Q13 * Q24) == _K(0);
    }

}  // namespace fun

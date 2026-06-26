/** @file euclid_plane.hpp
 *  @brief Euclidean plane functions: perpendicularity, parallelism, altitude, orthocenter, etc.
 */

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
    constexpr auto fB(const Line& line_l) -> typename Line::Dual {
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
    constexpr auto is_perpendicular(const Line& line_l, const Line& line_m) -> bool {
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
    constexpr auto is_parallel(const Line& line_l, const Line& line_m) -> bool {
        return cross2(line_l, line_m) == 0;
    }

    /**
     * @brief Compute the altitude from a point to a line.
     *
     * The altitude is the line through the given point that is perpendicular
     * to the given line.
     * @f[
     *     h_a = a \times \bar{l}
     * @f]
     * where \f$\bar{l}\f$ is the direction vector of line \f$l\f$.
     * @param[in] a The point from which the altitude is drawn
     * @param[in] line_l The line to which the altitude is drawn
     * @return Line The altitude line
     */
    template <typename Point, typename Line>
        requires ProjectivePlaneCoord<Point, Line>
    constexpr auto altitude(const Point& a, const Line& line_l) -> Line {
        return a * fB(line_l);
    }

    /**
     * @brief Compute all three altitudes of a triangle.
     *
     * Returns an array containing the three altitude lines of the triangle.
     * @f[
     *     \{h_{a_1},\; h_{a_2},\; h_{a_3}\}
     * @f]
     * @param[in] triangle Array of three non-collinear points
     * @return std::array<Line, 3> The three altitude lines
     */
    template <ProjectivePlaneCoord2 Point>
    constexpr auto tri_altitude(const Triple<Point>& triangle) {
        const auto& [a1, a2, a3] = triangle;
        return std::array{altitude(a1, a2 * a3), altitude(a2, a3 * a1), altitude(a3, a1 * a2)};
    }

    /**
     * @brief Compute the orthocenter of a triangle.
     *
     * The orthocenter is the intersection point of all three altitudes of a triangle.
     * @f[
     *     H = h_{a_1} \times h_{a_2}
     * @f]
     *
     * @dot
     *   digraph orthocenter_flow {
     *     bgcolor="transparent";
     *     node [shape=circle, style=filled, fillcolor="#d4e6f1"];
     *     a1 [label="A", fillcolor="#a9cce3"];
     *     a2 [label="B", fillcolor="#a9cce3"];
     *     a3 [label="C", fillcolor="#a9cce3"];
     *     ha [label="alt A", shape=box, fillcolor="#d5f5e3"];
     *     hb [label="alt B", shape=box, fillcolor="#d5f5e3"];
     *     h [label="H\n(orthocenter)", fillcolor="#fadbd8"];
     *     a1 -> ha;
     *     a2 -> hb;
     *     ha -> h;
     *     hb -> h;
     *     note [shape=note, fillcolor="#fcf3cf", label="H = h_a1 x h_a2"];
     *     h -> note [style=dashed, color="#888", constraint=false];
     *   }
     * @enddot
     *
     * @param[in] triangle Array of three non-collinear points
     * @return Point The orthocenter of the triangle
     */
    template <ProjectivePlaneCoord2 Point> constexpr auto orthocenter(const Triple<Point>& triangle)
        -> Point {
        const auto& [a1, a2, a3] = triangle;
        const auto t1 = altitude(a1, a2 * a3);
        const auto t2 = altitude(a2, a1 * a3);
        return t1 * t2;
    }

    /**
     * @brief Create an involution that reflects across a line.
     *
     * Returns an involution that performs reflection across the given line
     * in the Euclidean plane.
     * @f[
     *     I_m(p) = \operatorname{Involution}(m,\; \bar{m})
     * @f]
     * where \f$\bar{m}\f$ is the direction vector of the mirror line.
     *
     * @dot
     *   digraph reflect_flow {
     *     bgcolor="transparent";
     *     node [shape=circle, style=filled, fillcolor="#d4e6f1"];
     *     p [label="P", fillcolor="#a9cce3"];
     *     m [label="mirror line m", shape=box, fillcolor="#d5f5e3"];
     *     pp [label="P' = I_m(P)", fillcolor="#fadbd8"];
     *     p -> m [label="reflect"];
     *     m -> pp;
     *   }
     * @enddot
     *
     * @param[in] line_m The line of reflection (mirror)
     * @return Involution The reflection involution
     */
    template <ProjectivePlaneCoord2 Line> constexpr auto reflect(const Line& line_m) {
        return Involution{line_m, fB(line_m)};
    }

    /**
     * @brief Compute the midpoint of two points.
     *
     * Returns the point that divides the segment AB in the ratio 1:1.
     * @f[
     *     M = \operatorname{parametrize}(b_z,\; a,\; a_z,\; b)
     * @f]
     *
     * @dot
     *   digraph midpoint_flow {
     *     bgcolor="transparent";
     *     node [shape=circle, style=filled, fillcolor="#d4e6f1"];
     *     a [label="A", fillcolor="#a9cce3"];
     *     b [label="B", fillcolor="#a9cce3"];
     *     m [label="M\n(midpoint)", fillcolor="#7fb3d8"];
     *     a -> m [label="b_z * A"];
     *     b -> m [label="a_z * B"];
     *     note [shape=note, fillcolor="#fcf3cf", label="M = param(b_z, a, a_z, b)"];
     *     m -> note [style=dashed, color="#888", constraint=false];
     *   }
     * @enddot
     *
     * @param[in] a First point
     * @param[in] b Second point
     * @return Point The midpoint
     */
    template <ProjectivePlaneCoord2 Point> constexpr auto midpoint(const Point& a, const Point& b)
        -> Point {
        return parametrize(b[2], a, a[2], b);
    }

    /**
     * @brief Compute the midpoints of all three sides of a triangle.
     *
     * Returns an array containing the midpoints of each side of the triangle.
     * @f[
     *     \{M_{a_1a_2},\; M_{a_2a_3},\; M_{a_1a_3}\}
     * @f]
     * @param[in] triangle Array of three points
     * @return Triple<Point> Array of three midpoints
     */
    template <ProjectivePlaneCoord2 Point>
    constexpr auto tri_midpoint(const Triple<Point>& triangle) -> Triple<Point> {
        const auto& [a1, a2, a3] = triangle;
        return {midpoint(a1, a2), midpoint(a2, a3), midpoint(a1, a3)};
    }

    /**
     * @brief Compute a point on the unit circle from trigonometric parameters.
     *
     * Creates a point on the unit circle using the parameterization
     * (lambda^2 - mu^2, 2*lambda*mu, lambda^2 + mu^2).
     * @f[
     *     (\lambda^2 - \mu^2,\; 2\lambda\mu,\; \lambda^2 + \mu^2)
     * @f]
     * @param[in] lda1 The lambda parameter
     * @param[in] mu1 The mu parameter
     * @return Point A point on the unit circle
     */
    template <ProjectivePlaneCoord2 Point>
    constexpr auto uc_point(const Value_type<Point>& lda1, const Value_type<Point>& mu1) {
        const auto lda2 = lda1 * lda1;
        const auto mu2 = mu1 * mu1;
        return Point{lda2 - mu2, 2 * lda1 * mu1, lda2 + mu2};
    }

    /**
     * @brief Archimedes's function
     *
     * @f[
     *     \mathrm{archimedes}(a,b,c) = 4ab - (a + b - c)^2
     * @f]
     * @tparam _Q
     * @param[in] a
     * @param[in] b
     * @param[in] c
     * @return auto
     */
    template <OrderedRing _Q> constexpr auto archimedes(const _Q& a, const _Q& b, const _Q& c) {
        return 4 * a * b - sq(a + b - c);
    }

    /**
     * @brief Cyclic quadrilateral quadrea theorem
     *
     * Returns the coefficients of the quadratic equation whose roots
     * are the quadrea of a cyclic quadrilateral with opposite sides
     * (a,b) and (c,d).
     * @f[
     *     Q^2 - (4ab + 4cd - (a+b-c-d)^2)\,Q + 16abcd = 0
     * @f]
     * @tparam _Q
     * @param[in] a
     * @param[in] b
     * @param[in] c
     * @param[in] d
     * @return auto
     */
    template <typename _Q> constexpr auto cqq(const _Q& a, const _Q& b, const _Q& c, const _Q& d)
        -> std::array<_Q, 2> {
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
     * @f[
     *     Q_{12}Q_{34} + Q_{23}Q_{14} = Q_{13}Q_{24}
     * @f]
     * or equivalently \f$\mathrm{archimedes}(Q_{12}Q_{34},\; Q_{23}Q_{14},\; Q_{13}Q_{24}) = 0\f$.
     * @param[in] quad Array of six side/diagonal measurements {Q12, Q23, Q34, Q14, Q13, Q24}
     * @return true if the quadrilateral satisfies Ptolemy's theorem
     */
    template <typename T> constexpr auto Ptolemy(const T& quad) -> bool {
        const auto& [Q12, Q23, Q34, Q14, Q13, Q24] = quad;
        using _K = decltype(Q12);
        return archimedes(Q12 * Q34, Q23 * Q14, Q13 * Q24) == _K(0);
    }

}  // namespace fun

/** @file euclid_plane_measure.hpp
 *  @brief Euclidean plane measurement functions: quadrance, spread.
 */

#pragma once

#include "euclid_plane.hpp"
// #include "fractions.hpp"

namespace fun {

    /**
     * @brief Compute squared difference of ratios (integral version).
     *
     * @f[
     *     \mathrm{quad}_1 = \left(\frac{x_1}{z_1} - \frac{x_2}{z_2}\right)^2
     * @f]
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
     * @f[
     *     \mathrm{quad}_1 = \left(\frac{x_1}{z_1} - \frac{x_2}{z_2}\right)^2
     * @f]
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
     * Euclidean plane:
     * @f[
     *     Q(A,B) = \left(\frac{A_x}{A_z} - \frac{B_x}{B_z}\right)^2
     *            + \left(\frac{A_y}{A_z} - \frac{B_y}{B_z}\right)^2
     * @f]
     *
     * @dot
     *   digraph quadrance_flow {
     *     rankdir=LR;
     *     bgcolor="transparent";
     *     node [shape=box, style=filled, fillcolor="#d4e6f1"];
     *     a1 [label="Point A\n(x1,y1,z1)", fillcolor="#a9cce3"];
     *     a2 [label="Point B\n(x2,y2,z2)", fillcolor="#a9cce3"];
     *     dx [label="(x1/z1 - x2/z2)^2", fillcolor="#d4e6f1"];
     *     dy [label="(y1/z1 - y2/z2)^2", fillcolor="#d4e6f1"];
     *     sum [label="+", shape=diamond, fillcolor="#f9e79f"];
     *     q [label="Q(A,B)", fillcolor="#7fb3d8"];
     *     a1 -> dx; a2 -> dx;
     *     a1 -> dy; a2 -> dy;
     *     dx -> sum; dy -> sum;
     *     sum -> q;
     *   }
     * @enddot
     *
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
     * @brief Base function for spread and cross-spread.
     *
     * @f[
     *     \mathrm{sbase}(l_1, l_2, d) =
     * \frac{d^2}{\mathrm{dot}_1(l_1,l_1)\,\mathrm{dot}_1(l_2,l_2)}
     * @f]
     * @param[in] l1 First line
     * @param[in] l2 Second line
     * @param[in] d Scalar value (cross or dot product)
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
     * The spread is the squared sine of the angle between two lines:
     * @f[
     *     s(l_1, l_2) = \frac{\mathrm{cross}_2(l_1, l_2)^2}
     *                         {\mathrm{dot}_1(l_1, l_1) \mathrm{dot}_1(l_2, l_2)}
     * @f]
     *
     * @dot
     *   digraph spread_flow {
     *     rankdir=LR;
     *     bgcolor="transparent";
     *     node [shape=box, style=filled, fillcolor="#d4e6f1"];
     *     l1 [label="Line l1", fillcolor="#a9cce3"];
     *     l2 [label="Line l2", fillcolor="#a9cce3"];
     *     cross [label="cross2(l1,l2)^2", fillcolor="#d4e6f1"];
     *     denom [label="dot1(l1,l1)*\ndot1(l2,l2)", fillcolor="#d4e6f1"];
     *     div [label="/", shape=diamond, fillcolor="#f9e79f"];
     *     s [label="s(l1,l2)", fillcolor="#7fb3d8"];
     *     l1 -> cross; l2 -> cross;
     *     l1 -> denom; l2 -> denom;
     *     cross -> div; denom -> div;
     *     div -> s;
     *   }
     * @enddot
     *
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
        const auto& [a_1, a_2, a_3] = triangle;
        return std::array{quadrance(a_2, a_3), quadrance(a_1, a_3), quadrance(a_1, a_2)};
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
        const auto& [a_1, a_2, a_3] = trilateral;
        return std::array{spread(a_2, a_3), spread(a_1, a_3), spread(a_1, a_2)};
    }

    /**
     * @brief Compute the cross spread between two lines.
     *
     * The cross spread is the squared cosine of the angle between lines:
     * @f[
     * \mathrm{cross\_s}(l_1, l_2) = \frac{\mathrm{dot}_1(l_1, l_2)^2}
     *                                              {\mathrm{dot}_1(l_1, l_1) \mathrm{dot}_1(l_2,
     * l_2)}
     * @f]
     * @param[in] l1 First line
     * @param[in] l2 Second line
     * @return auto The cross spread
     */
    template <ProjectivePlaneCoord2 Line> constexpr auto cross_s(const Line& l1, const Line& l2) {
        return sbase(l1, l2, dot1(l1, l2));
    }

}  // namespace fun

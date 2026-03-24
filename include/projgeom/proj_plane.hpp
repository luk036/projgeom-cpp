#pragma once

#include <array>
#include <cassert>
#include <tuple>

#include "proj_plane_concepts.h"

/** @file include/proj_plane.hpp
 *  This is a C++ Library header.
 */

/**
 @todo: projectivity >=
**/

namespace fun {

    /**
     * @brief
     *
     * The code snippet is defining a function named `incident` that checks if
     * a point `pt_p` is incident to a line `ln_l` in a projective plane. The
     * function uses the `dot` method of the `Point` class and compares it to
     * zero to determine if the point is incident to the line. The function
     * returns a boolean value indicating whether the point is incident to the line or not.
     *
     * @param[in] pt_p
     * @param[in] ln_l
     * @return true
     * @return false
     */
    template <typename Point, typename Line>
        requires ProjectivePlane<Point, Line>
    constexpr auto incident(const Point &pt_p, const Line &ln_l) -> bool {
        return pt_p.dot(ln_l) == Value_type<Point>(0);
    }

    /**
     * @brief Coincident
     *
     * @tparam[in] Line Line
     * @tparam[in] Args points
     * @return true if points are conincident (on a line ln_l)
     * @return false otherwise
     */
    template <typename Line, typename... Args>
        requires(ProjectivePlanePrim<Line, Args> && ...)
    constexpr auto coincident(const Line &ln_l, const Args &...pt_r) -> bool {
        return (incident(pt_r, ln_l) && ...);
    }

    /**
     * @brief Type alias for an array of three points (a triangle)
     *
     * @tparam Point The point type
     */
    template <typename Point> using Triple = std::array<Point, 3>;

    /**
     * @brief Compute the dual triangle (sides as points).
     *
     * Returns the triangle formed by the three lines joining the vertices
     * of the input triangle.
     * @param[in] triangle Array of three non-collinear points
     * @return auto Array of three lines (the sides)
     */
    template <ProjectivePlanePrim2 Point> constexpr auto tri_dual(const Triple<Point> &triangle)

    {
        const auto &[a1, a2, a3] = triangle;
        assert(!coincident(a2 * a3, a1));
        return std::array{a2 * a3, a1 * a3, a1 * a2};
    }

    /**
     * @brief Apply a binary function to all pairs of triangle vertices.
     *
     * Returns an array with the function applied to each pair of distinct
     * vertices of the triangle.
     * @param[in] func A binary function to apply
     * @param[in] triangle Array of three points
     * @return auto Array of three results
     */
    template <ProjectivePlanePrim2 Point, typename Fn>
    constexpr auto tri_func(Fn &&func, const Triple<Point> &triangle)

    {
        const auto &[a1, a2, a3] = triangle;
        return std::array{func(a2, a3), func(a1, a3), func(a1, a2)};
    }

    /**
     * @brief return whether two triangles are perspective
     *
     * @param[in] tri1
     * @param[in] tri2
     * @return true
     * @return false
     */
    template <ProjectivePlanePrim2 Point>
    constexpr auto persp(const Triple<Point> &tri1, const Triple<Point> &tri2) -> bool {
        const auto &[A, B, C] = tri1;
        const auto &[D, E, F] = tri2;
        const auto O = (A * D) * (B * E);
        return incident(O, C * F);
    }

    /**
     * @brief Compute the harmonic conjugate of three collinear points.
     *
     * Given three collinear points A, B, C, returns the fourth point D
     * such that (A, B; C, D) is a harmonic range.
     * @param[in] A First point
     * @param[in] B Second point
     * @param[in] C Third point (must be collinear with A and B)
     * @return constexpr Point The harmonic conjugate
     */
    template <ProjectivePlane2 Point>
    constexpr auto harm_conj(const Point &A, const Point &B, const Point &C) -> Point {
        assert(incident(A * B, C));
        const auto lC = C * (A * B).aux();
        return parametrize(B.dot(lC), A, A.dot(lC), B);
    }

    /**
     * @brief Compute the harmonic conjugate (generic version).
     *
     * Given three collinear points A, B, C, returns the fourth point D
     * such that (A, B; C, D) is a harmonic range. Generic version using
     * auxiliary constructions.
     * @param[in] A First point
     * @param[in] B Second point
     * @param[in] C Third point (must be collinear with A and B)
     * @return constexpr Point The harmonic conjugate
     */
    template <ProjectivePlaneGeneric2 _Point>
    constexpr auto harm_conj(const _Point &A, const _Point &B, const _Point &C) -> _Point {
        assert(incident(A * B, C));
        const auto AB = A * B;
        const auto P = AB.aux();
        const auto R = P.aux2(C);
        const auto S = (A * R) * (B * P);
        const auto Q = (B * R) * (A * P);
        return (Q * S) * AB;
    }

    /**
     * @brief Check if four points form a harmonic range.
     *
     * Four points A, B, C, D form a harmonic range if the cross ratio
     * (A, B; C, D) equals -1.
     * @param[in] A First point
     * @param[in] B Second point
     * @param[in] C Third point
     * @param[in] D Fourth point
     * @return constexpr auto true if harmonic, false otherwise
     */
    template <ProjectivePlane2 Point> constexpr auto is_harmonic(const Point &A, const Point &B,
                                                                 const Point &C,
                                                                 const Point &D) -> bool {
        return harm_conj(A, B, C) == D;
    }

    /**
     * @brief
     *
     * @tparam Point
     * @tparam Line
     */
    template <typename Point, typename Line>
        requires ProjectivePlane<Point, Line>
    class Involution {
        using K = Value_type<Point>;

      private:
        Line _m;
        Point _o;
        K _c;

      public:
        /**
         * @brief Construct a new Involution object
         *
         * @param[in] ln_m
         * @param[in] o
         */
        constexpr Involution(Line ln_m, Point o)  // input mirror and center
            : _m{std::move(ln_m)}, _o{std::move(o)}, _c{_m.dot(_o)} {}

        /**
         * @brief Apply the involution to a point.
         *
         * Returns the image of pt_p under the involution.
         * @param[in] pt_p The point to transform
         * @return Point The transformed point
         */
        constexpr auto operator()(const Point &pt_p) const -> Point {
            return parametrize(this->_c, pt_p, K(-2 * pt_p.dot(this->_m)), this->_o);
        }

        /**
         * @brief Apply the involution to a line.
         *
         * Returns the image of ln_l under the involution.
         * @param[in] ln_l The line to transform
         * @return Line The transformed line
         */
        constexpr auto operator()(const Line &ln_l) const -> Line {
            return parametrize(this->_c, ln_l, K(-2 * ln_l.dot(this->_o)), this->_m);
        }
    };

    /**
     * @brief Generic involution class for projective planes.
     *
     * An involution is a transformation that is its own inverse.
     * @tparam Point The point type
     * @tparam Line The line type
     */
    template <typename Point, typename Line>
        requires ProjectivePlaneGeneric<Point, Line>
    class involution_generic {
      private:
        Line _m;
        Point _o;

      public:
        /**
         * @brief Construct a new generic involution object
         *
         * @param[in] ln_m The mirror line
         * @param[in] o The center point
         */
        constexpr involution_generic(Line ln_m,
                                     Point o)  // input mirror and center
            : _m{std::move(ln_m)}, _o{std::move(o)} {}

        /**
         * @brief Apply the involution to a point.
         *
         * Returns the image of pt_p under the involution.
         * @param[in] pt_p The point to transform
         * @return Point The transformed point
         */
        constexpr auto operator()(const Point &pt_p) const -> Point {
            auto po = pt_p * this->_o;
            auto B = po * this->_m;
            return harm_conj(this->_o, B, pt_p);
        }
    };

    /**
     * @brief Check Pappus Theorem for two sets of collinear points.
     *
     * Asserts that the three intersection points formed by joining
     * corresponding points from two collinear triples are themselves collinear.
     * @param[in] coline1 First triple of collinear points
     * @param[in] coline2 Second triple of collinear points
     */
    template <ProjectivePlanePrim2 Point>
    void check_pappus(const Triple<Point> &coline1, const Triple<Point> &coline2)

    {
        const auto &[A, B, C] = coline1;
        const auto &[D, E, F] = coline2;

        const auto G = (A * E) * (B * D);
        const auto H = (A * F) * (C * D);
        const auto I = (B * F) * (C * E);
        assert(coincident(G, H, I));
    }

    /**
     * @brief Check Desargues' theorem for two triangles.
     *
     * Asserts that two triangles are perspective from a point if and only if
     * they are perspective from a line. This is a fundamental theorem in
     * projective geometry.
     * @param[in] tri1 First triangle
     * @param[in] tri2 Second triangle
     */
    template <ProjectivePlanePrim2 Point>
    void check_desargue(const Triple<Point> &tri1, const Triple<Point> &tri2) {
        const auto trid1 = tri_dual(tri1);
        const auto trid2 = tri_dual(tri2);
        const auto bool1 = persp(tri1, tri2);
        const auto bool2 = persp(trid1, trid2);
        assert((bool1 && bool2) || (!bool1 && !bool2));
    }

}  // namespace fun

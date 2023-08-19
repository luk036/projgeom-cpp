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
     * @param[in] pt_p
     * @param[in] ln_l
     * @return true
     * @return false
     */
    template <typename Point, typename Line>
    requires ProjectivePlane<Point, Line> constexpr auto incident(const Point &pt_p,
                                                                  const Line &ln_l) -> bool {
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
    requires(ProjectivePlanePrim<Line, Args> &&...) constexpr auto coincident(const Line &ln_l,
                                                                              const Args &... pt_r)
        -> bool {
        return (incident(pt_r, ln_l) && ...);
    }

    template <typename Point> using Triple = std::array<Point, 3>;

    /**
     * @brief
     *
     * @param[in] triangle
     * @return auto
     */
    template <ProjectivePlanePrim2 Point> constexpr auto tri_dual(const Triple<Point> &triangle)

    {
        const auto &[a1, a2, a3] = triangle;
        assert(!coincident(a2 * a3, a1));
        return std::array{a2 * a3, a1 * a3, a1 * a2};
    }

    /**
     * @brief
     *
     * @param[in] func
     * @param[in] triangle
     * @return auto
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
     * @brief
     *
     * @tparam Point
     * @param[in] A
     * @param[in] B
     * @param[in] C
     * @return constexpr Point
     */
    template <ProjectivePlane2 Point>
    constexpr auto harm_conj(const Point &A, const Point &B, const Point &C) -> Point {
        assert(incident(A * B, C));
        const auto lC = C * (A * B).aux();
        return parametrize(B.dot(lC), A, A.dot(lC), B);
    }

    /**
     * @brief
     *
     * @tparam Point
     * @param[in] A
     * @param[in] B
     * @param[in] C
     * @return constexpr Point
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
     * @brief
     *
     * @param[in] A
     * @param[in] B
     * @param[in] C
     * @param[in] D
     * @return constexpr auto
     *
     */
    template <ProjectivePlane2 Point>
    constexpr auto is_harmonic(const Point &A, const Point &B, const Point &C, const Point &D)
        -> bool {
        return harm_conj(A, B, C) == D;
    }

    /**
     * @brief
     *
     * @tparam Point
     * @tparam Line
     */
    template <typename Point, typename Line>
    requires ProjectivePlane<Point, Line> class Involution {
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
         * @brief
         *
         * @param[in] pt_p
         * @return Point
         */
        constexpr auto operator()(const Point &pt_p) const -> Point {
            return parametrize(this->_c, pt_p, K(-2 * pt_p.dot(this->_m)), this->_o);
        }

        /**
         * @brief
         *
         * @param[in] pt_p
         * @return Point
         */
        constexpr auto operator()(const Line &ln_l) const -> Line {
            return parametrize(this->_c, ln_l, K(-2 * ln_l.dot(this->_o)), this->_m);
        }
    };

    /**
     * @brief
     *
     * @tparam Point
     * @tparam Line
     */
    template <typename Point, typename Line>
    requires ProjectivePlaneGeneric<Point, Line> class involution_generic {
      private:
        Line _m;
        Point _o;

      public:
        /**
         * @brief Construct a new Involution object
         *
         * @param[in] ln_m
         * @param[in] o
         */
        constexpr involution_generic(Line ln_m,
                                     Point o)  // input mirror and center
            : _m{std::move(ln_m)}, _o{std::move(o)} {}

        /**
         * @brief
         *
         * @param[in] pt_p
         * @return Point
         */
        constexpr auto operator()(const Point &pt_p) const -> Point {
            auto po = pt_p * this->_o;
            auto B = po * this->_m;
            return harm_conj(this->_o, B, pt_p);
        }
    };

    /**
     * @brief Check Pappus Theorem
     *
     * @tparam Point
     * @tparam Line
     * @param[in] co1
     * @param[in] co2
     */
    template <ProjectivePlanePrim2 Point>
    void check_pappus(const Triple<Point> &co1, const Triple<Point> &co2)

    {
        const auto &[A, B, C] = co1;
        const auto &[D, E, F] = co2;

        const auto G = (A * E) * (B * D);
        const auto H = (A * F) * (C * D);
        const auto I = (B * F) * (C * E);
        assert(coincident(G, H, I));
    }

    /**
     * @brief
     *
     * @param[in] tri1
     * @param[in] tri2
     */
    template <ProjectivePlanePrim2 Point>
    void check_desargue(const Triple<Point> &tri1, const Triple<Point> &tri2) {
        const auto trid1 = tri_dual(tri1);
        const auto trid2 = tri_dual(tri2);
        const auto bool1 = persp(tri1, tri2);
        const auto b2 = persp(trid1, trid2);
        assert((bool1 && b2) || (!bool1 && !b2));
    }

}  // namespace fun

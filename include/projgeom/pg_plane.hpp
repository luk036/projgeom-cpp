#pragma once

#include <array>
#include <cassert>

#if __cpp_concepts >= 201907L
#    include "pg_concepts.hpp"
#endif

namespace fun {
    /**
     * @brief Check Projective plane Axiom
     *
     * @tparam Point Point
     * @tparam Line Line
     * @param[in] pt_p
     * @param[in] pt_q
     * @param[in] ln_l
     */
    template <class Point, class Line>
#if __cpp_concepts >= 201907L
    requires ProjPlanePrimDual<Point, Line>
#endif
    inline auto check_axiom(const Point &pt_p, const Point &pt_q, const Line &ln_l) -> bool {
        if (pt_p != pt_p) return false;
        if (pt_p.incident(ln_l) != ln_l.incident(pt_p)) return false;
        if (pt_p.meet(pt_q) != pt_q.meet(pt_p)) return false;
        const auto ln_m = pt_p.meet(pt_q);
        if (!(ln_m.incident(pt_p) && ln_m.incident(pt_q))) return false;
        return true;
    }

    /**
     * @brief Coincident
     *
     * @tparam Point
     * @param[in] pt_p
     * @param[in] pt_q
     * @param[in] pt_r
     * @return true
     * @return false
     */
    template <class Point, class Line = typename Point::Dual>
#if __cpp_concepts >= 201907L
    requires ProjPlanePrimDual<Point, Line>
#endif
    constexpr auto coincident(const Point &pt_p, const Point &pt_q, const Point &pt_r) -> bool {
        return pt_p.meet(pt_q).incident(pt_r);
    }

    /**
     * @brief Check Pappus Theorem
     *
     * @tparam Point Point
     * @param[in] coline1
     * @param[in] coline2
     * @return true
     * @return false
     */
    template <class Point, class Line = typename Point::Dual>
#if __cpp_concepts >= 201907L
    requires ProjPlanePrimDual<Point, Line>
#endif
    constexpr auto check_pappus(const std::array<Point, 3> &coline1,
                                const std::array<Point, 3> &coline2) -> bool {
        const auto &[pt_a, pt_b, pt_c] = coline1;
        const auto &[pt_d, pt_e, pt_f] = coline2;
        const auto pt_g = (pt_a.meet(pt_e)).meet(pt_b.meet(pt_d));
        const auto pt_h = (pt_a.meet(pt_f)).meet(pt_c.meet(pt_d));
        const auto pt_i = (pt_b.meet(pt_f)).meet(pt_c.meet(pt_e));
        return coincident(pt_g, pt_h, pt_i);
    }

    /**
     * @brief Dual of triangle
     *
     * @tparam Point Point
     * @tparam Line Line
     * @param[in] triangle
     * @return std::array<Line, 3>
     */
    template <class Point, class Line = typename Point::Dual>
#if __cpp_concepts >= 201907L
    requires ProjPlanePrimDual<Point, Line>
#endif
    constexpr auto tri_dual(const std::array<Point, 3> &triangle) -> std::array<Line, 3> {
        const auto &[a1, a2, a3] = triangle;
        assert(!coincident(a1, a2, a3));
        return {a2.meet(a3), a1.meet(a3), a1.meet(a2)};
    }

    /**
     * @brief return whether two triangles are perspective
     *
     * @tparam Point Point
     * @param[in] tri1
     * @param[in] tri2
     * @return true
     * @return false
     */
    template <class Point, class Line = typename Point::Dual>
#if __cpp_concepts >= 201907L
    requires ProjPlanePrimDual<Point, Line>
#endif
    constexpr auto persp(const std::array<Point, 3> &tri1, const std::array<Point, 3> &tri2)
        -> bool {
        const auto &[pt_a, pt_b, pt_c] = tri1;
        const auto &[pt_d, pt_e, pt_f] = tri2;
        const auto &o = pt_a.meet(pt_d).meet(pt_b.meet(pt_e));
        return pt_c.meet(pt_f).incident(o);
    }

    /**
     * @brief Check Desargue's Theorem
     *
     * @tparam Point Point
     * @param[in] tri1
     * @param[in] tri2
     * @return true
     * @return false
     */
    template <class Point, class Line = typename Point::Dual>
#if __cpp_concepts >= 201907L
    requires ProjPlanePrimDual<Point, Line>
#endif
    constexpr auto check_desargue(const std::array<Point, 3> &tri1,
                                  const std::array<Point, 3> &tri2) -> bool {
        const auto trid1 = tri_dual(tri1);
        const auto trid2 = tri_dual(tri2);
        const auto bool1 = persp(tri1, tri2);
        const auto bool2 = persp(trid1, trid2);
        return (bool1 && bool2) || (!bool1 && !bool2);
    }

    /**
     * @brief Check Projective plane Axiom 2
     *
     * @tparam Point Point
     * @tparam Line Line
     */

    /**
     * @brief Check Axiom
     *
     * @tparam Value
     * @tparam Point
     * @tparam Point::Dual
     * @param[in] pt_p
     * @param[in] pt_q
     * @param[in] ln_l
     * @param[in] pt_a
     * @param[in] pt_b
     */
    template <typename Value, class Point, class Line>
#if __cpp_concepts >= 201907L
    requires ProjectivePlaneDual<Value, Point, Line>
#endif
    inline auto check_axiom2(const Point &pt_p, const Point &pt_q, const Line &ln_l, const Value &a,
                             const Value &b) -> bool {
        if (pt_p.dot(ln_l) != ln_l.dot(pt_p)) return false;
        if (pt_p.aux().incident(pt_p)) return false;
        const auto ln_m = pt_p.meet(pt_q);
        if (!ln_m.incident(Point::parametrize(a, pt_p, b, pt_q))) return false;
        return true;
    }

    /**
     * @brief harmonic conjugate
     *
     * @tparam Value
     * @tparam Point
     * @param[in] pt_a
     * @param[in] pt_b
     * @param[in] pt_c
     * @return Point
     */
    template <typename Value, class Point, class Line = typename Point::Dual>
#if __cpp_concepts >= 201907L
    requires ProjectivePlaneDual<Value, Point, Line>
#endif
    constexpr auto harm_conj(const Point &pt_a, const Point &pt_b, const Point &pt_c) -> Point {
        assert(coincident(pt_a, pt_b, pt_c));
        const auto ab = pt_a.meet(pt_b);
        const auto lc = ab.aux().meet(pt_c);
        return Point::parametrize(lc.dot(pt_a), pt_a, lc.dot(pt_b), pt_b);
    }

    /**
     * @brief Involution
     *
     * @tparam Value
     * @tparam Point
     * @tparam Point::Dual
     * @param[in] origin
     * @param[in] mirror
     * @param[in] pt_p
     * @return Point
     */
    template <typename Value, class Point, class Line>
#if __cpp_concepts >= 201907L
    requires ProjectivePlaneDual<Value, Point, Line>
#endif
    constexpr auto involution(const Point &origin, const Line &mirror, const Point &pt_p) -> Point {
        const auto po = pt_p.meet(origin);
        const auto pt_b = po.meet(mirror);
        return harm_conj(origin, pt_b, pt_p);
    }

    /*
    axiom(Point pt_p, Point pt_q, Point pt_r, Line ln_l) {
      ln_l == Line{pt_p, pt_q} => I(pt_p, ln_l) and I(pt_q, ln_l);
    }
    */

}  // namespace fun

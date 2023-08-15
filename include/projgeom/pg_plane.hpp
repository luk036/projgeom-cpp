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
     * @tparam L Line
     * @param[in] p
     * @param[in] q
     * @param[in] l
     */
    template <class Point, class L>
#if __cpp_concepts >= 201907L
        requires ProjPlanePrimDual<Point, L>
#endif
    inline auto check_axiom(const Point &p, const Point &q, const L &l) -> bool {
        if (p != p) return false;
        if (p.incident(l) != l.incident(p)) return false;
        if (p.meet(q) != q.meet(p)) return false;
        const auto m = p.meet(q);
        if (!(m.incident(p) && m.incident(q))) return false;
        return true;
    }

    /**
     * @brief Coincident
     *
     * @tparam Point
     * @param[in] p
     * @param[in] q
     * @param[in] r
     * @return true
     * @return false
     */
    template <class Point, class L = typename Point::Dual>
#if __cpp_concepts >= 201907L
        requires ProjPlanePrimDual<Point, L>
#endif
    inline constexpr auto coincident(const Point &p, const Point &q, const Point &r) -> bool {
        return p.meet(q).incident(r);
    }

    /**
     * @brief Check Pappus Theorem
     *
     * @tparam Point Point
     * @param[in] co1
     * @param[in] co2
     * @return true
     * @return false
     */
    template <class Point, class L = typename Point::Dual>
#if __cpp_concepts >= 201907L
        requires ProjPlanePrimDual<Point, L>
#endif
    inline constexpr auto check_pappus(const std::array<Point, 3> &co1,
                                       const std::array<Point, 3> &co2) -> bool {
        const auto &[a, b, c] = co1;
        const auto &[d, e, f] = co2;
        const auto g = (a.meet(e)).meet(b.meet(d));
        const auto h = (a.meet(f)).meet(c.meet(d));
        const auto i = (b.meet(f)).meet(c.meet(e));
        return coincident(g, h, i);
    }

    /**
     * @brief Dual of triangle
     *
     * @tparam Point Point
     * @tparam L Line
     * @param[in] tri
     * @return std::array<L, 3>
     */
    template <class Point, class L = typename Point::Dual>
#if __cpp_concepts >= 201907L
        requires ProjPlanePrimDual<Point, L>
#endif
    inline constexpr auto tri_dual(const std::array<Point, 3> &tri) -> std::array<L, 3> {
        const auto &[a1, a2, a3] = tri;
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
    template <class Point, class L = typename Point::Dual>
#if __cpp_concepts >= 201907L
        requires ProjPlanePrimDual<Point, L>
#endif
    inline constexpr auto persp(const std::array<Point, 3> &tri1, const std::array<Point, 3> &tri2)
        -> bool {
        const auto &[a, b, c] = tri1;
        const auto &[d, e, f] = tri2;
        const auto &o = a.meet(d).meet(b.meet(e));
        return c.meet(f).incident(o);
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
    template <class Point, class L = typename Point::Dual>
#if __cpp_concepts >= 201907L
        requires ProjPlanePrimDual<Point, L>
#endif
    inline constexpr auto check_desargue(const std::array<Point, 3> &tri1,
                                         const std::array<Point, 3> &tri2) -> bool {
        const auto trid1 = tri_dual(tri1);
        const auto trid2 = tri_dual(tri2);
        const auto b1 = persp(tri1, tri2);
        const auto b2 = persp(trid1, trid2);
        return (b1 && b2) || (!b1 && !b2);
    }

    /**
     * @brief Check Projective plane Axiom 2
     *
     * @tparam Point Point
     * @tparam L Line
     */

    /**
     * @brief Check Axiom
     *
     * @tparam V
     * @tparam Point
     * @tparam Point::Dual
     * @param[in] p
     * @param[in] q
     * @param[in] l
     * @param[in] a
     * @param[in] b
     */
    template <typename V, class Point, class L>
#if __cpp_concepts >= 201907L
        requires ProjPlaneDual<V, Point, L>
#endif
    inline auto check_axiom2(const Point &p, const Point &q, const L &l, const V &a, const V &b)
        -> bool {
        if (p.dot(l) != l.dot(p)) return false;
        if (p.aux().incident(p)) return false;
        const auto m = p.meet(q);
        if (!m.incident(Point::plucker(a, p, b, q))) return false;
        return true;
    }

    /**
     * @brief harmonic conjugate
     *
     * @tparam V
     * @tparam Point
     * @param[in] a
     * @param[in] b
     * @param[in] c
     * @return Point
     */
    template <typename V, class Point, class L = typename Point::Dual>
#if __cpp_concepts >= 201907L
        requires ProjPlaneDual<V, Point, L>
#endif
    inline constexpr auto harm_conj(const Point &a, const Point &b, const Point &c) -> Point {
        assert(coincident(a, b, c));
        const auto ab = a.meet(b);
        const auto lc = ab.aux().meet(c);
        return Point::plucker(lc.dot(a), a, lc.dot(b), b);
    }

    /**
     * @brief Involution
     *
     * @tparam V
     * @tparam Point
     * @tparam Point::Dual
     * @param[in] origin
     * @param[in] mirror
     * @param[in] p
     * @return Point
     */
    template <typename V, class Point, class L>
#if __cpp_concepts >= 201907L
        requires ProjPlaneDual<V, Point, L>
#endif
    inline constexpr auto involution(const Point &origin, const L &mirror, const Point &p)
        -> Point {
        const auto po = p.meet(origin);
        const auto b = po.meet(mirror);
        return harm_conj(origin, b, p);
    }

    /*
    axiom(Point p, Point q, Point r, L l) {
      l == L{p, q} => I(p, l) and I(q, l);
    }
    */

}  // namespace fun

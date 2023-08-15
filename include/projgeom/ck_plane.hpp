#pragma once

#include <array>

#if __cpp_concepts >= 201907L
#    include "ck_concepts.hpp"
#endif

namespace fun {
    /**
     * @brief
     *
     * @tparam Point Point
     * @tparam L Line
     */
    template <class L, class Point = typename L::Dual>
#if __cpp_concepts >= 201907L
        requires CKPlanePrimDual<L, Point>
#endif
    constexpr auto is_perpendicular(const L &m1, const L &m2) -> bool {
        return m1.perp().incident(m2);
    }

    /**
     * @brief
     *
     * @tparam Point Point
     * @tparam L Line
     */
    template <class Point, class L>
#if __cpp_concepts >= 201907L
        requires CKPlanePrimDual<Point, L>
#endif
    constexpr auto altitude(const Point &p, const L &m) -> L {
        return m.perp().meet(p);
    }

    /**
     * @brief
     *
     * @param[in] tri
     * @return std::arrary<L, 3>
     */
    template <class Point, class L = typename Point::Dual>
#if __cpp_concepts >= 201907L
        requires CKPlanePrimDual<Point, L>
#endif
    constexpr auto orthocenter(const std::array<Point, 3> &tri) -> Point {
        const auto &[a1, a2, a3] = tri;
        assert(!coincident(a1, a2, a3));
        const auto t1 = altitude(a1, a2.meet(a3));
        const auto t2 = altitude(a2, a3.meet(a1));
        return t1.meet(t2);
        // return {a2.meet(a3), a1.meet(a3), a1.meet(a2)};
    }

    /**
     * @brief
     *
     * @param[in] tri
     * @return std::arrary<L, 3>
     */
    template <class Point, class L>
#if __cpp_concepts >= 201907L
        requires CKPlanePrimDual<Point, L>
#endif
    constexpr auto tri_altitude(const std::array<Point, 3> &tri) -> std::array<L, 3> {
        const auto [l1, l2, l3] = tri_dual(tri);
        const auto &[a1, a2, a3] = tri;
        assert(!coincident(a1, a2, a3));
        auto &&t1 = altitude(a1, l1);
        auto &&t2 = altitude(a2, l2);
        auto &&t3 = altitude(a3, l3);
        return {t1, t2, t3};
    }

    template <typename V, class Point, class L = typename Point::Dual>
#if __cpp_concepts >= 201907L
        requires CKPlaneDual<V, Point, L>
#endif
    constexpr auto reflect(const L &mirror, const Point &p) -> Point {
        return involution(mirror.perp(), mirror, p);
    }

    /*
    axiom(Point p, Point q, Point r, L l) {
      l == L{p, q} => I(p, l) and I(q, l);
    }
    */

}  // namespace fun

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
     * @tparam Line Line
     */
    template <class Line, class Point = typename Line::Dual>
#if __cpp_concepts >= 201907L
        requires CayleyKleinPlanePrimitiveDual<Line, Point>
#endif
    constexpr auto is_perpendicular(const Line &m1, const Line &m2) -> bool {
        return m1.perp().incident(m2);
    }

    /**
     * @brief
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Point, class Line>
#if __cpp_concepts >= 201907L
        requires CayleyKleinPlanePrimitiveDual<Point, Line>
#endif
    constexpr auto altitude(const Point &p, const Line &m) -> Line {
        return m.perp().meet(p);
    }

    /**
     * @brief
     *
     * @param[in] tri
     * @return std::arrary<Line, 3>
     */
    template <class Point, class Line = typename Point::Dual>
#if __cpp_concepts >= 201907L
        requires CayleyKleinPlanePrimitiveDual<Point, Line>
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
     * @return std::arrary<Line, 3>
     */
    template <class Point, class Line>
#if __cpp_concepts >= 201907L
        requires CayleyKleinPlanePrimitiveDual<Point, Line>
#endif
    constexpr auto tri_altitude(const std::array<Point, 3> &tri) -> std::array<Line, 3> {
        const auto [l1, l2, l3] = tri_dual(tri);
        const auto &[a1, a2, a3] = tri;
        assert(!coincident(a1, a2, a3));
        auto &&t1 = altitude(a1, l1);
        auto &&t2 = altitude(a2, l2);
        auto &&t3 = altitude(a3, l3);
        return {t1, t2, t3};
    }

    template <typename Value, class Point, class Line = typename Point::Dual>
#if __cpp_concepts >= 201907L
        requires CayleyKleinPlaneDual<Value, Point, Line>
#endif
    constexpr auto reflect(const Line &mirror, const Point &p) -> Point {
        return involution(mirror.perp(), mirror, p);
    }

    /*
    axiom(Point p, Point q, Point r, Line l) {
      l == Line{p, q} => I(p, l) and I(q, l);
    }
    */

}  // namespace fun

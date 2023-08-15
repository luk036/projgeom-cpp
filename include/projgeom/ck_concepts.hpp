#pragma once

#include <array>

#include "pg_concepts.hpp"

namespace fun {
    /**
     * @brief C-K plane Concept
     *
     * @tparam Point Point
     * @tparam L Line
     */
    template <class Point, class L = typename Point::Dual>
    concept CayleyKleinPlanePrimitive =    //
        ProjectivePlanePrimitive<Point, L>  //
        && requires(const Point &p, const L &l) {
        { p.perp() } -> concepts::convertible_to<L>;
    };

    /**
     * @brief C-K plane Concept (full)
     *
     * @tparam Point Point
     * @tparam L Line
     */
    template <class Point, class L = typename Point::Dual>
    concept CKPlanePrimDual = CayleyKleinPlanePrimitive<Point, L> && CayleyKleinPlanePrimitive<L, Point>;

    /**
     * @brief C-K plane Concept
     *
     * @tparam Point Point
     * @tparam L Line
     */
    template <class V, class Point, class L = typename Point::Dual>
    concept CayleyKleinPlane =       //
        ProjectivePlane<V, Point, L>  //
        && requires(const Point &p, const L &l) {
        { p.perp() } -> concepts::convertible_to<L>;
    };

    /**
     * @brief C-K plane Concept (full)
     *
     * @tparam Point Point
     * @tparam L Line
     */
    template <class V, class Point, class L = typename Point::Dual>
    concept CKPlaneDual = CayleyKleinPlane<V, Point, L> && CayleyKleinPlane<V, L, Point>;

}  // namespace fun

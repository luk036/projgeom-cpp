#pragma once

#include <array>
#include <cassert>

#include "common_concepts.h"

namespace fun {
    /**
     * @brief Projective plane Concept
     *
     * @tparam Point Point
     * @tparam L Line
     */
    template <class Point, class L>
    concept ProjectivePlanePrimitive =        //
        concepts::equality_comparable<Point>  //
        && requires(const Point &p, const Point &q, const L &l) {
               { p.incident(l) } -> concepts::convertible_to<bool>;  // incidence
               { p.meet(q) } -> concepts::convertible_to<L>;         // join or meet
           };

    /**
     * @brief Projective plane Concept (full)
     *
     * @tparam Point Point
     * @tparam L Line
     */
    template <class Point, class L>
    concept ProjPlanePrimDual
        = ProjectivePlanePrimitive<Point, L> && ProjectivePlanePrimitive<L, Point>;

    /**
     * @brief Projective plane Concept
     *
     * @tparam V
     * @tparam Point Point
     * @tparam L Line
     */
    template <typename V, class Point, class L>
    concept ProjectivePlane
        = concepts::equality_comparable<Point> && ProjectivePlanePrimitive<Point, L>  //
          && requires(const Point &p, const Point &q, const L &l, const V &a) {
                 { p.aux() } -> concepts::convertible_to<L>;   // line not incident with p
                 { p.dot(l) } -> concepts::convertible_to<V>;  // for basic measurement
                 { Point::plucker(a, p, a, q) } -> concepts::convertible_to<Point>;
             };

    /**
     * @brief Projective plane dual Concept
     *
     * @tparam V
     * @tparam Point Point
     * @tparam L Line
     */
    template <typename V, class Point, class L>
    concept ProjPlaneDual = ProjectivePlane<V, Point, L> && ProjectivePlane<V, L, Point>;

}  // namespace fun

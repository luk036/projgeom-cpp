#pragma once

#include <array>
#include <cassert>

#include "common_concepts.h"

namespace fun {
    /**
     * @brief Projective plane Concept
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Point, class Line>
    concept ProjectivePlanePrimitive =        //
        concepts::equality_comparable<Point>  //
        && requires(const Point &p, const Point &q, const Line &l) {
               { p.incident(l) } -> concepts::convertible_to<bool>;  // incidence
               { p.meet(q) } -> concepts::convertible_to<Line>;         // join or meet
           };

    /**
     * @brief Projective plane Concept (full)
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Point, class Line>
    concept ProjPlanePrimDual
        = ProjectivePlanePrimitive<Point, Line> && ProjectivePlanePrimitive<Line, Point>;

    /**
     * @brief Projective plane Concept
     *
     * @tparam Value
     * @tparam Point Point
     * @tparam Line Line
     */
    template <typename Value, class Point, class Line>
    concept ProjectivePlane
        = concepts::equality_comparable<Point> && ProjectivePlanePrimitive<Point, Line>  //
          && requires(const Point &p, const Point &q, const Line &l, const Value &a) {
                 { p.aux() } -> concepts::convertible_to<Line>;   // line not incident with p
                 { p.dot(l) } -> concepts::convertible_to<Value>;  // for basic measurement
                 { Point::plucker(a, p, a, q) } -> concepts::convertible_to<Point>;
             };

    /**
     * @brief Projective plane dual Concept
     *
     * @tparam Value
     * @tparam Point Point
     * @tparam Line Line
     */
    template <typename Value, class Point, class Line>
    concept ProjectivePlaneDual = ProjectivePlane<Value, Point, Line> && ProjectivePlane<Value, Line, Point>;

}  // namespace fun

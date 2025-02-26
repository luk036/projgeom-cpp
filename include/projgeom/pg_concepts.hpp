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
        && requires(const Point &pt_p, const Point &pt_q, const Line &ln_l) {
        { pt_p.incident(ln_l) } -> concepts::convertible_to<bool>;  // incidence
        { pt_p.meet(pt_q) } -> concepts::convertible_to<Line>;      // join or meet
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
        && requires(const Point &pt_p, const Point &pt_q, const Line &ln_l, const Value &a) {
        { pt_p.aux() } -> concepts::convertible_to<Line>;       // line not incident with pt_p
        { pt_p.dot(ln_l) } -> concepts::convertible_to<Value>;  // for basic measurement
        { Point::parametrize(a, pt_p, a, pt_q) } -> concepts::convertible_to<Point>;
    };

    /**
     * @brief Projective plane dual Concept
     *
     * @tparam Value
     * @tparam Point Point
     * @tparam Line Line
     */
    template <typename Value, class Point, class Line>
    concept ProjectivePlaneDual
        = ProjectivePlane<Value, Point, Line> && ProjectivePlane<Value, Line, Point>;

}  // namespace fun

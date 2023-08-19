#pragma once

#include <array>

#include "pg_concepts.hpp"

namespace fun {
    /**
     * @brief Cayley-Klein plane Concept
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Point, class Line = typename Point::Dual> concept CayleyKleinPlanePrimitive
        =                                      //
        ProjectivePlanePrimitive<Point, Line>  //
            &&requires(const Point &pt_p, const Line &ln_l) {
        { pt_p.perp() }
        ->concepts::convertible_to<Line>;  // pole or polar
    };

    /**
     * @brief Cayley-Klein plane Concept (full)
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Point, class Line = typename Point::Dual> concept CayleyKleinPlanePrimitiveDual
        = CayleyKleinPlanePrimitive<Point, Line> &&CayleyKleinPlanePrimitive<Line, Point>;

    /**
     * @brief Cayley-Klein plane Concept
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Value, class Point, class Line = typename Point::Dual> concept CayleyKleinPlane
        =                                    //
        ProjectivePlane<Value, Point, Line>  //
            &&requires(const Point &pt_p, const Line &ln_l) {
        { pt_p.perp() }
        ->concepts::convertible_to<Line>;  // pole or polar
    };

    /**
     * @brief Cayley-Klein plane Concept (full)
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Value, class Point, class Line = typename Point::Dual>
    concept CayleyKleinPlaneDual
        = CayleyKleinPlane<Value, Point, Line> &&CayleyKleinPlane<Value, Line, Point>;

}  // namespace fun

/** @file include/proj_plane_concepts.h
 *  This is a C++ Library header.
 */

#pragma once

#include "common_concepts.h"

/**
 * @todo: projectivity >=
 */

namespace fun {

    /**
     * @brief Projective plane Concept (half)
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Point, class Line>
    concept ProjectivePlanePrimH
        = STD_ALT::equality_comparable<Point>
          && requires(const Point &pt_p, const Point &pt_q, const Line &ln_l) {
                 { incident(pt_p, ln_l) } -> STD_ALT::convertible_to<bool>;  // incidence
                 { pt_p *pt_q } -> STD_ALT::convertible_to<Line>;           // join or meet
                 // { pt_p.aux() } -> STD_ALT::convertible_to<Line>; // line not incident
                 // with pt_p { pt_p.aux2(pt_q) } -> STD_ALT::convertible_to<Point>;
                 // // pt_p pt_r on pt_p * pt_q, pt_r != pt_p and pt_r != pt_q
             };

    /**
     * @brief Projective plane Concept (full)
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Point, class Line = typename Point::Dual>
    concept ProjectivePlanePrim
        = ProjectivePlanePrimH<Point, Line> && ProjectivePlanePrimH<Line, Point>;

    /**
     * @brief Shorthand Notation of ProjectivePlane
     *
     * @tparam Point Point
     */
    template <class Point>
    concept ProjectivePlanePrim2
        = ProjectivePlanePrim<std::remove_reference_t<Point>>;  // Make the compiler
                                                                // happy

    /**
     * @brief Projective plane Concept (full)
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Point, class Line = typename Point::Dual>
    concept ProjectivePlaneGenericH
        = ProjectivePlanePrimH<Point, Line>
          && requires(const Point &pt_p, const Point &pt_q) {
                 {
                     pt_p.aux()
                 } -> STD_ALT::convertible_to<Line>;  // line not incident with pt_p
                 {
                     pt_p.aux2(pt_q)
                 } -> STD_ALT::convertible_to<Point>;  // pt_p pt_r on pt_p * pt_q,
                                                       // pt_r != pt_p and pt_r != pt_q
             };

    /**
     * @brief Projective plane Concept (full)
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Point, class Line = typename Point::Dual>
    concept ProjectivePlaneGeneric
        = ProjectivePlaneGenericH<Point, Line> && ProjectivePlaneGenericH<Line, Point>;

    /**
     * @brief Shorthand Notation of ProjectivePlane
     *
     * @tparam Point Point
     */
    template <class Point>
    concept ProjectivePlaneGeneric2
        = ProjectivePlaneGeneric<std::remove_reference_t<Point>>;  // Make the compiler
                                                                   // happy

    /**
     * @brief Projective plane Concept (half)
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Point, class Line>
    concept ProjectivePlaneH
        = STD_ALT::equality_comparable<Point>
          && requires(const Point &pt_p, const Point &pt_q, const Line &ln_l,
                      const Value_type<Point> &pt_a) {
                 typename Value_type<Point>;
                 // { Point(pt_p) } -> Point; // copyable
                 // { incident(pt_p, ln_l) } -> bool; // incidence
                 { pt_p *pt_q } -> STD_ALT::convertible_to<Line>;  // join or meet
                 {
                     pt_p.dot(ln_l)
                 } -> STD_ALT::convertible_to<Value_type<Point>>;  // for measurement
                 {
                     pt_p.aux()
                 } -> STD_ALT::convertible_to<Line>;  // line not incident with pt_p
                 {
                     plucker(pt_a, pt_p, pt_a, pt_q)
                 } -> STD_ALT::convertible_to<Point>;  // module computation
             };

    /**
     * @brief Projective plane Concept (full)
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Point, class Line = typename Point::Dual>
    concept ProjectivePlane = ProjectivePlaneH<Point, Line> && ProjectivePlaneH<Line, Point>;

    /*
    axiom(Point pt_p, Point pt_q, Point pt_r, Line ln_l) {
      ln_l == Line{pt_p, pt_q} => I(pt_p, ln_l) and I(pt_q, ln_l);
    }
    */

    /**
     * @brief Shorthand Notation of ProjectivePlane
     *
     * @tparam Point Point
     */
    template <class Point>
    concept ProjectivePlane2
        = ProjectivePlane<std::remove_reference_t<Point>>;  // Make the compiler happy

    /**
     * @brief Projective plane Concept (half)
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Point, class Line>
    concept ProjectivePlaneCoordH
        = ProjectivePlaneH<Point, Line> && requires(const Point &pt_p, size_t idx) {
              typename Value_type<Point>;

              {
                  pt_p[idx]
              } -> STD_ALT::convertible_to<Value_type<Point>>;  // for coordinate acess
          };

    /**
     * @brief Projective plane Concept (full)
     *
     * @tparam Point Point
     * @tparam Line Line
     */
    template <class Point, class Line = typename Point::Dual>
    concept ProjectivePlaneCoord
        = ProjectivePlaneCoordH<Point, Line> && ProjectivePlaneCoordH<Line, Point>;

    /**
     * @brief Shorthand Notation of ProjectivePlane
     *
     * @tparam Point Point
     */
    template <class Point>
    concept ProjectivePlaneCoord2
        = ProjectivePlaneCoord<std::remove_reference_t<Point>>;  // Make the compiler
                                                                 // happy

}  // namespace fun

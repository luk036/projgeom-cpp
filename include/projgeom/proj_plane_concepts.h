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
concept ProjectivePlanePrimH = STD_ALT::equality_comparable<Point> &&
    requires(const Point &point_p, const Point &point_q, const Line &line_l) {
    { incident(point_p, line_l) } -> STD_ALT::convertible_to<bool>; // incidence
    { point_p *point_q } -> STD_ALT::convertible_to<Line>; // join or meet
    // { point_p.aux() } -> STD_ALT::convertible_to<Line>; // line not incident
    // with point_p { point_p.aux2(point_q) } -> STD_ALT::convertible_to<Point>;
    // // point_p r on point_p * point_q, r != point_p and r != point_q
};

/**
 * @brief Projective plane Concept (full)
 *
 * @tparam Point Point
 * @tparam Line Line
 */
template <class Point, class Line = typename Point::Dual>
concept ProjectivePlanePrim =
    ProjectivePlanePrimH<Point, Line> && ProjectivePlanePrimH<Line, Point>;

/**
 * @brief Shorthand Notation of ProjectivePlane
 *
 * @tparam Point Point
 */
template <class Point>
concept ProjectivePlanePrim2 =
    ProjectivePlanePrim<std::remove_reference_t<Point>>; // Make the compiler
                                                         // happy

/**
 * @brief Projective plane Concept (full)
 *
 * @tparam Point Point
 * @tparam Line Line
 */
template <class Point, class Line = typename Point::Dual>
concept ProjectivePlaneGenericH = ProjectivePlanePrimH<Point, Line> &&
    requires(const Point &point_p, const Point &point_q) {
    {
        point_p.aux()
        } -> STD_ALT::convertible_to<Line>; // line not incident with point_p
    {
        point_p.aux2(point_q)
        } -> STD_ALT::convertible_to<Point>; // point_p r on point_p * point_q,
                                             // r != point_p and r != point_q
};

/**
 * @brief Projective plane Concept (full)
 *
 * @tparam Point Point
 * @tparam Line Line
 */
template <class Point, class Line = typename Point::Dual>
concept ProjectivePlaneGeneric = ProjectivePlaneGenericH<Point, Line> &&
    ProjectivePlaneGenericH<Line, Point>;

/**
 * @brief Shorthand Notation of ProjectivePlane
 *
 * @tparam Point Point
 */
template <class Point>
concept ProjectivePlaneGeneric2 =
    ProjectivePlaneGeneric<std::remove_reference_t<Point>>; // Make the compiler
                                                            // happy

/**
 * @brief Projective plane Concept (half)
 *
 * @tparam Point Point
 * @tparam Line Line
 */
template <class Point, class Line>
concept ProjectivePlaneH = STD_ALT::equality_comparable<Point> &&
    requires(const Point &point_p, const Point &point_q, const Line &line_l,
             const Value_type<Point> &a) {
    typename Value_type<Point>;
    // { Point(point_p) } -> Point; // copyable
    // { incident(point_p, line_l) } -> bool; // incidence
    { point_p *point_q } -> STD_ALT::convertible_to<Line>; // join or meet
    {
        point_p.dot(line_l)
        } -> STD_ALT::convertible_to<Value_type<Point>>; // for measurement
    {
        point_p.aux()
        } -> STD_ALT::convertible_to<Line>; // line not incident with point_p
    {
        plucker(a, point_p, a, point_q)
        } -> STD_ALT::convertible_to<Point>; // module computation
};

/**
 * @brief Projective plane Concept (full)
 *
 * @tparam Point Point
 * @tparam Line Line
 */
template <class Point, class Line = typename Point::Dual>
concept ProjectivePlane =
    ProjectivePlaneH<Point, Line> && ProjectivePlaneH<Line, Point>;

/*
axiom(Point point_p, Point point_q, Point r, Line line_l) {
  line_l == Line{point_p, point_q} => I(point_p, line_l) and I(point_q, line_l);
}
*/

/**
 * @brief Shorthand Notation of ProjectivePlane
 *
 * @tparam Point Point
 */
template <class Point>
concept ProjectivePlane2 =
    ProjectivePlane<std::remove_reference_t<Point>>; // Make the compiler happy

/**
 * @brief Projective plane Concept (half)
 *
 * @tparam Point Point
 * @tparam Line Line
 */
template <class Point, class Line>
concept ProjectivePlaneCoordH = ProjectivePlaneH<Point, Line> &&
    requires(const Point &point_p, size_t idx) {
    typename Value_type<Point>;

    {
        point_p[idx]
        } -> STD_ALT::convertible_to<Value_type<Point>>; // for coordinate acess
};

/**
 * @brief Projective plane Concept (full)
 *
 * @tparam Point Point
 * @tparam Line Line
 */
template <class Point, class Line = typename Point::Dual>
concept ProjectivePlaneCoord =
    ProjectivePlaneCoordH<Point, Line> && ProjectivePlaneCoordH<Line, Point>;

/**
 * @brief Shorthand Notation of ProjectivePlane
 *
 * @tparam Point Point
 */
template <class Point>
concept ProjectivePlaneCoord2 =
    ProjectivePlaneCoord<std::remove_reference_t<Point>>; // Make the compiler
                                                          // happy

} // namespace fun

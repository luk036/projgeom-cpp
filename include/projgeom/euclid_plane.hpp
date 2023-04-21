#pragma once

#include <type_traits>

#include "pg_common.hpp" // import cross2, dot1
#include "proj_plane.hpp" // import pg_point, Involution, tri_func, quad_func, plucker
#include "proj_plane_concepts.h"

namespace fun {

/**
 * @brief
 *
 * @param[in] line_l
 * @return auto
 */
template <ProjectivePlaneCoord2 Line> // // and requires point_p[i]
constexpr auto fB(const Line &line_l) -> typename Line::Dual {
  return {line_l[0], line_l[1], 0};
}

/**
 * @brief
 *
 * @param[in] line_l
 * @param[in] line_m
 * @return true
 * @return false
 */
template <ProjectivePlaneCoord2 Line>
constexpr auto is_perpendicular(const Line &line_l, const Line &line_m)
    -> bool {
  return dot1(line_l, line_m) == 0;
}

/**
 * @brief
 *
 * @param[in] line_l
 * @param[in] line_m
 * @return true
 * @return false
 */
template <ProjectivePlaneCoord2 Line>
constexpr auto is_parallel(const Line &line_l, const Line &line_m) -> bool {
  return cross2(line_l, line_m) == 0;
}

/**
 * @brief
 *
 * @param[in] a
 * @param[in] line_l
 * @return Line
 */
template <typename Point, typename Line>
requires ProjectivePlaneCoord<Point, Line>
constexpr auto altitude(const Point &a, const Line &line_l) -> Line {
  return a * fB(line_l);
}

/**
 * @brief
 *
 * @param[in] tri
 * @return auto
 */
template <ProjectivePlaneCoord2 Point>
constexpr auto tri_altitude(const Triple<Point> &tri) {
  const auto &[a1, a2, a3] = tri;
  return std::array{altitude(a1, a2 * a3), altitude(a2, a3 * a1),
                    altitude(a3, a1 * a2)};
}

/**
 * @brief
 *
 * @param[in] tri
 * @return Point
 */
template <ProjectivePlaneCoord2 Point>
constexpr auto orthocenter(const Triple<Point> &tri) -> Point {
  const auto &[a1, a2, a3] = tri;
  const auto t1 = altitude(a1, a2 * a3);
  const auto t2 = altitude(a2, a1 * a3);
  return t1 * t2;
}

/**
 * @brief
 *
 * @param[in] line_m
 * @return auto
 */
template <ProjectivePlaneCoord2 Line>
constexpr auto reflect(const Line &line_m) {
  return Involution{line_m, fB(line_m)};
}

/**
 * @brief
 *
 * @param[in] a
 * @param[in] b
 * @return Point
 */
template <ProjectivePlaneCoord2 Point>
constexpr auto midpoint(const Point &a, const Point &b) -> Point {
  return plucker(b[2], a, a[2], b);
}

/**
 * @brief
 *
 * @param[in] tri
 * @return auto
 */
template <ProjectivePlaneCoord2 Point>
constexpr auto tri_midpoint(const Triple<Point> &tri) -> Triple<Point> {
  const auto &[a1, a2, a3] = tri;
  return {midpoint(a1, a2), midpoint(a2, a3), midpoint(a1, a3)};
}

/**
 * @brief
 *
 * @param[in] lda1
 * @param[in] mu1
 * @return Point
 */
template <ProjectivePlaneCoord2 Point>
constexpr auto uc_point(const Value_type<Point> &lda1,
                        const Value_type<Point> &mu1) {
  const auto lda2 = lda1 * lda1;
  const auto mu2 = mu1 * mu1;
  return Point{lda2 - mu2, 2 * lda1 * mu1, lda2 + mu2};
}

/**
 * @brief Archimedes's function
 *
 * @tparam _Q
 * @param[in] a
 * @param[in] b
 * @param[in] c
 * @return auto
 */
template <OrderedRing _Q>
constexpr auto Ar(const _Q &a, const _Q &b, const _Q &c) {
  return 4 * a * b - sq(a + b - c);
}

/**
 * @brief Cyclic quadrilateral quadrea theorem
 *
 * @tparam _Q
 * @param[in] a
 * @param[in] b
 * @param[in] c
 * @param[in] d
 * @return auto
 */
template <typename _Q>
constexpr auto cqq(const _Q &a, const _Q &b, const _Q &c, const _Q &d)
    -> std::array<_Q, 2> {
  const auto t1 = 4 * a * b;
  const auto t2 = 4 * c * d;
  auto line_m = (t1 + t2) - sq(a + b - c - d);
  auto point_p = line_m * line_m - 4 * t1 * t2;
  return {std::move(line_m), std::move(point_p)};
}

/**
 * @brief
 *
 * @tparam _Q
 * @param[in] quad
 * @return auto
 */
template <typename T> constexpr auto Ptolemy(const T &quad) -> bool {
  const auto &[Q12, Q23, Q34, Q14, Q13, Q24] = quad;
  using _K = decltype(Q12);
  return Ar(Q12 * Q34, Q23 * Q14, Q13 * Q24) == _K(0);
}

} // namespace fun

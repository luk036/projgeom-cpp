#pragma once

#include "fractions.hpp"
#include "proj_plane.hpp"

/** @file include/proj_plane.hpp
 *  This is a C++ Library header.
 */

/**
 @todo: projectivity >=
**/

namespace fun {

/**
 * @brief
 *
 * @tparam K
 * @param[in] a
 * @param[in] b
 * @param[in] c
 * @param[in] d
 * @return auto
 */
template <Ring K>
constexpr auto ratio_ratio(const K &a, const K &b, const K &c, const K &d) {
  if constexpr (Integral<K>) {
    return Fraction(a, b) / Fraction(c, d);
  } else {
    return (a * d) / (b * c);
  }
}

/**
 * @brief Cross Ratio
 *
 * @tparam Point
 * @tparam Line
 * @param[in] A point_p \in Point
 * @param[in] B point_p \in Point
 * @param[in] line_l line \in Point
 * @param[in] line_m line \in Point
 * @return cross ratio R(A,B;line_l,line_m)
 *
 * @todo rewrite by projecting to the y-axis first [:2]
 */
template <typename Point, typename Line>
requires ProjectivePlane<Point, Line>
constexpr auto x_ratio(const Point &A, const Point &B, const Line &line_l,
                       const Line &line_m) {
  return ratio_ratio(A.dot(line_l), A.dot(line_m), B.dot(line_l),
                     B.dot(line_m));
}

/**
 * @brief
 *
 * @param[in] A
 * @param[in] B
 * @param[in] C
 * @param[in] D
 * @return constexpr auto
 */
template <ProjectivePlaneCoord2 Point>
constexpr auto R(const Point &A, const Point &B, const Point &C, const Point &D)

{
  using K = Value_type<Point>;
  if (cross0(A, B) != K(0)) { // Project points to yz-plane
    return R0(A, B, C, D);
  }
  // Project points to xz-plane
  return R1(A, B, C, D);
}

/**
 * @brief
 *
 * @param[in] A
 * @param[in] B
 * @param[in] C
 * @param[in] D
 * @return constexpr auto
 */
template <ProjectivePlane2 Point>
constexpr auto R(const Point &A, const Point &B, const Point &C,
                 const Point &D) {
  const auto O = (C * D).aux();
  return x_ratio(A, B, O * C, O * D);
}

/**
 * @brief
 *
 * @param[in] A
 * @param[in] B
 * @param[in] C
 * @param[in] D
 * @return constexpr auto
 */
template <ProjectivePlaneCoord2 Point>
constexpr auto R0(const Point &A, const Point &B, const Point &C,
                  const Point &D) {
  return ratio_ratio(cross0(A, C), cross0(A, D), cross0(B, C), cross0(B, D));
}

/**
 * @brief
 *
 * @param[in] A
 * @param[in] B
 * @param[in] C
 * @param[in] D
 * @return constexpr auto
 */
template <ProjectivePlaneCoord2 Point>
constexpr auto R1(const Point &A, const Point &B, const Point &C,
                  const Point &D) {
  return ratio_ratio(cross1(A, C), cross1(A, D), cross1(B, C), cross1(B, D));
}

} // namespace fun

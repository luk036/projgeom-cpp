#pragma once

#include <type_traits> // std::is_base_of_v

#include "pg_common.hpp"
#include "proj_plane.hpp"
#include "proj_plane_concepts.h"
#include "proj_plane_measure.hpp"

namespace fun {

/**
 * @brief
 *
 * @tparam _Point
 * @tparam _Line
 * @tparam Derived
 */
template <typename _Point, typename _Line,
          template <typename Point, typename Line> class Derived>
requires ProjectivePlanePrim<_Point, _Line> // c++20 concept
struct ck {
  using point_t = _Point;
  using line_t = _Line;

  using cDer = const Derived<_Point, _Line>;
  cDer &self = *static_cast<cDer *>(this);

  /**
   * @brief construct ck object
   *
   */
  constexpr ck() noexcept {
    static_assert(
        std::is_base_of_v<ck<_Point, _Line, Derived>, Derived<_Point, _Line>>);
  }

  /**
   * @brief is perpendicular
   *
   * @tparam Line
   * @param[in] line_l
   * @param[in] line_m
   * @return true
   * @return false
   */
  constexpr auto is_perpendicular(const _Line &line_l,
                                  const _Line &line_m) const -> bool {
    return incident(line_m, self.perp(line_l));
  }

  /**
   * @brief altitude
   *
   * @param[in] point_p
   * @param[in] line_l
   * @return Line
   */
  template <typename Point, typename Line = typename Point::Dual>
  requires ProjectivePlanePrim<Point, Line> // c++20 concept
  constexpr auto altitude(const Point &point_p, const Line &line_l) const
      -> Line {
    return point_p * self.perp(line_l);
  }

  /**
   * @brief altitudes of triangle
   *
   * @param[in] tri
   * @return std::tuple
   */
  template <ProjectivePlanePrim2 Point>
  constexpr auto tri_altitude(const Triple<Point> &tri) const {
    const auto [l1, l2, l3] = tri_dual(tri);
    const auto &[a1, a2, a3] = tri;

    return std::array{this->altitude(a1, l1), this->altitude(a2, l2),
                      this->altitude(a3, l3)};
  }

  /**
   * @brief ortho-center
   *
   * @param[in] tri
   * @return Point
   */
  template <ProjectivePlanePrim2 Point>
  constexpr auto orthocenter(const Triple<Point> &tri) const -> Point {
    const auto &[a1, a2, a3] = tri;

    const auto t1 = this->altitude(a1, a2 * a3);
    const auto t2 = this->altitude(a2, a1 * a3);
    return t1 * t2;
  }

  /**
   * @brief reflect
   *
   * @param[in] line_m
   * @return auto
   */
  template <ProjectivePlane Point> auto reflect(const Point &line_m) const {
    return Involution{self.perp(line_m), Point{line_m}};
  }

  /**
   * @brief measure of triangle
   *
   * @param[in] tri
   * @return constexpr auto
   */
  template <ProjectivePlane Point>
  constexpr auto tri_measure(const Triple<Point> &tri) const {
    const auto &[a1, a2, a3] = tri;

    return std::array{self.measure(a2, a3), self.measure(a1, a3),
                      self.measure(a1, a2)};
  }

  /**
   * @brief quadrance between two points
   *
   * @param[in] point_p
   * @param[in] point_q
   * @return constexpr auto
   */
  constexpr auto quadrance(const _Point &point_p, const _Point &point_q) const {
    return self.measure(point_p, point_q);
  }

  /**
   * @brief spread between two lines
   *
   * @param[in] line_l
   * @param[in] line_m
   * @return constexpr auto
   */
  constexpr auto spread(const _Line &line_l, const _Line &line_m) const {
    return self.measure(line_l, line_m);
  }

  /**
   * @brief quadrances of triangle
   *
   * @param[in] triangle
   * @return constexpr auto
   */
  constexpr auto tri_quadrance(const Triple<_Point> &triangle) const {
    return this->tri_measure(triangle);
  }

  /**
   * @brief spreads of triangle
   *
   * @param[in] trilateral
   * @return constexpr auto
   */
  constexpr auto tri_spread(const Triple<_Line> &trilateral) const {
    return this->tri_measure(trilateral);
  }
};

/**
 * @brief check sine law
 *
 * @tparam Q_t
 * @tparam S_t
 * @param[in] Q
 * @param[in] S
 * @return true
 * @return false
 */
template <OrderedRing Q_t>
constexpr auto check_sine_law(const Triple<Q_t> &Q, const Triple<Q_t> &S)
    -> bool {
  const auto &[q1, q2, q3] = Q;
  const auto &[s1, s2, s3] = S;
  return (s1 * q2 == s2 * q1) && (s2 * q3 == s3 * q2);
}

/**
 * @brief Elliptic Plane
 *
 * @tparam Point
 * @tparam Point::Dual
 */
template <typename Point, typename Line = typename Point::Dual>
requires ProjectivePlanePrim<Point, Line> // c++20 concept
struct ellck : ck<Point, Line, ellck> {
  /**
   * @brief perp (polar) of point_p
   *
   * @param[in] v
   * @return Line
   */
  constexpr auto perp(const Point &v) const -> Line { return Line(v); }

  /**
   * @brief perp (pole) of line
   *
   * @param[in] v
   * @return Point
   */
  constexpr auto perp(const Line &v) const -> Point { return Point(v); }

  /**
   * @brief measure between two objects
   *
   * @tparam _Point
   * @param[in] a1
   * @param[in] a2
   * @return constexpr auto
   */
  template <ProjectivePlane2 _Point>
  constexpr auto measure(const _Point &a1, const _Point &a2) const {
    return 1 - x_ratio(a1, a2, this->perp(a2), this->perp(a1));
  }
};

/**
 * @brief Hyperbolic Plane
 *
 * @tparam Point
 * @tparam Point::Dual
 */
template <typename Point, typename Line = typename Point::Dual>
requires ProjectivePlanePrim<Point, Line> // c++20 concept
struct hyck : ck<Point, Line, hyck> {
  /**
   * @brief perp (polar) of point_p
   *
   * @param[in] v
   * @return Line
   */
  constexpr auto perp(const Point &v) const -> Line {
    return Line(v[0], v[1], -v[2]);
  }

  /**
   * @brief perp (pole) of line
   *
   * @param[in] v
   * @return Point
   */
  constexpr auto perp(const Line &v) const -> Point {
    return Point(v[0], v[1], -v[2]);
  }

  /**
   * @brief measure between two objects
   *
   * @tparam _Point
   * @param[in] a1
   * @param[in] a2
   * @return constexpr auto
   */
  template <ProjectivePlane2 _Point>
  constexpr auto measure(const _Point &a1, const _Point &a2) const {
    return 1 - x_ratio(a1, a2, this->perp(a2), this->perp(a1));
  }
};

/**
 * @brief
 *
 * @tparam K
 * @param[in] Q
 * @return constexpr auto
 */
template <OrderedRing K> constexpr auto check_cross_TQF(const Triple<K> &Q) {
  const auto &[q1, q2, q3] = Q;
  return sq(q1 + q2 + q3) - 2 * (q1 * q1 + q2 * q2 + q3 * q3) -
         4 * q1 * q2 * q3;
}

/**
 * @brief
 *
 * @tparam K
 * @param[in] S
 * @param[in] q3
 * @return constexpr auto
 */
template <OrderedRing K>
constexpr auto check_cross_law(const Triple<K> &S, const K &q3) {
  const auto &[s1, s2, s3] = S;
  return sq(s1 * s2 * q3 - (s1 + s2 + s3) + 2) -
         4 * (1 - s1) * (1 - s2) * (1 - s3);
}

} // namespace fun

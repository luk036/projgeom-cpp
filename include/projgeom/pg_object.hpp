// The template and inlines for the -*- C++ -*- pg object classes.
// Initially implemented by Wai-Shing Luk <luk036@gmail.com>
//

/** @file include/pg_object.hpp
 *  This is a C++ Library header.
 */

#pragma once

#include "pg_common.hpp"

namespace fun {

/**
 * @brief Projective object
 *
 * @tparam _K Type of object elements
 * @tparam _dual
 */
template <Ring _K, typename _dual> class pg_object : public std::array<_K, 3> {
  /// Value typedef.
  using _Base = std::array<_K, 3>;
  using _Self = pg_object<_K, _dual>;

public:
  using value_type = _K;
  using Dual = _dual;

  // pg_object(_Self &&) = default;

  /**
   * @brief Construct a new pg object object
   *
   * @param[in] a array of coordinates
   */
  constexpr explicit pg_object(const _Base &a) : _Base{a} {}

  /**
   * @brief Construct a new pg object
   *
   */
  explicit pg_object(const _Self &) = default;

  /**
   * @brief
   *
   * @return _Self&
   */
  auto operator=(const _Self &) -> _Self & = delete;

  /**
   * @brief Construct a new pg object
   *
   */
  pg_object(_Self &&) noexcept = default;

  /**
   * @brief
   *
   * @return _Self&
   */
  auto operator=(_Self &&) noexcept -> _Self & = default;

  // Operators:

  /**
   * @brief Equal to
   *
   * @param[in] rhs
   * @return true if this object is equivalent to the rhs
   * @return false otherwise
   */
  friend constexpr auto operator==(const _Self &lhs, const _Self &rhs) -> bool {
    if (&lhs == &rhs) {
      return true;
    }
    return cross(lhs, rhs) == _Base{_K(0), _K(0), _K(0)};
  }

  /**
   * @brief Not equal to
   *
   * @param[in] rhs
   * @return true if this object is not equivalent to the rhs
   * @return false otherwise
   */
  friend constexpr auto operator!=(const _Self &lhs, const _Self &rhs) -> bool {
    return !(lhs == rhs);
  }

  /**
   * @brief Equal to
   *
   * @param[in] rhs
   * @return true if this object is equivalent to the rhs
   * @return false otherwise
   */
  [[nodiscard]] constexpr auto is_NaN() const -> bool {
    const _Base &base = *this;
    return base == _Base{_K(0), _K(0), _K(0)};
  }

  /**
   * @brief the dot product
   *
   * @param[in] line_l
   * @return _K
   */
  [[nodiscard]] constexpr auto dot(const Dual &line_l) const -> _K {
    return fun::dot_c(*this, line_l);
  }

  /**
   * @brief Generate a new line not incident with point_p
   *
   * @return Dual
   */
  [[nodiscard]] constexpr auto aux() const -> Dual { return Dual(*this); }

  /**
   * @brief Join or meet
   *
   * @param[in] rhs
   * @return true if this point_p is equivalent to the rhs
   * @return false otherwise
   */
  friend constexpr auto operator*(const _Self &lhs, const _Self &rhs) -> Dual {
    return Dual(cross(lhs, rhs));
  }
};

/**
 * @brief
 *
 * @tparam Point
 * @tparam Value_type<Point>
 * @param[in] lda1
 * @param[in] point_p
 * @param[in] mu1
 * @param[in] point_q
 * @return Point
 */
template <typename Point, Ring _K = Value_type<Point>>
inline constexpr auto plucker(const _K &ld1, const Point &point_p,
                              const _K &mu1, const Point &point_q) -> Point {
  return Point{plucker_c(ld1, point_p, mu1, point_q)};
}

/**
 * @brief
 *
 * @tparam _K
 * @tparam _dual
 * @tparam _Stream
 * @param[in] os
 * @param[in] point_p
 * @return _Stream&
 */
template <Ring _K, typename _dual, class _Stream>
auto operator<<(_Stream &os, const pg_object<_K, _dual> &point_p) -> _Stream & {
  os << '(' << point_p[0] << ':' << point_p[1] << ':' << point_p[2] << ')';
  return os;
}

} // namespace fun

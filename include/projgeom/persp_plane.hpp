#pragma once

#include "ck_plane.hpp"
#include "fractions.hpp"
// #include "pg_common.hpp"
#include "proj_plane.hpp" // import pg_point, Involution, tri_func,

namespace fun {

/**
 * @brief
 *
 * @tparam Point
 * @tparam Point::Dual
 */
template <typename Point, typename Line = typename Point::Dual>
requires ProjectivePlanePrim<Point, Line> // c++20 concept
class persp_euclid_plane : public ck<Point, Line, persp_euclid_plane> {
  using K = Value_type<Point>;

private:
  Point _Ire;
  Point _Iim;
  Line _l_infty;

public:
  /**
   * @brief Construct a new persp euclid plane object
   *
   * @param[in] Ire
   * @param[in] Iim
   * @param[in] l_infty
   */
  constexpr persp_euclid_plane(Point Ire, Point Iim, Line l_infty)
      : _Ire{std::move(Ire)}, _Iim{std::move(Iim)}, _l_infty{
                                                        std::move(l_infty)} {}

  // /**
  //  * @brief Construct a new persp euclid plane object
  //  *
  //  * @param[in] Ire
  //  * @param[in] Iim
  //  * @param[in] l_infty
  //  */
  // constexpr persp_euclid_plane(const Point &Ire, const Point &Iim, const Line
  // &l_infty)
  //     : _Ire{Ire}, _Iim{Iim}, _l_infty{l_infty} {}

  // /**
  //  * @brief
  //  *
  //  * @param[in] x
  //  * @return const Line&
  //  */
  // constexpr const Line &perp(const Point &x) const { return _l_infty; }

  /**
   * @brief
   *
   * @return const Line&
   */
  [[nodiscard]] constexpr auto l_infty() const -> const Line & {
    return this->_l_infty;
  }

  /**
   * @brief
   *
   * @param[in] x
   * @return Point
   */
  [[nodiscard]] constexpr auto perp(const Line &v) const -> Point {
    const auto alpha = v.dot(this->_Ire);
    const auto beta = v.dot(this->_Iim);
    return plucker(alpha, this->_Ire, beta, this->_Iim);
  }

  /**
   * @brief
   *
   * @param[in] line_l
   * @param[in] line_m
   * @return true
   * @return false
   */
  [[nodiscard]] constexpr auto is_parallel(const Line &line_l,
                                           const Line &line_m) const -> bool {
    return incident(this->_l_infty, line_l * line_m);
  }

  /**
   * @brief
   *
   * @param[in] a
   * @param[in] b
   * @return Point
   */
  [[nodiscard]] constexpr auto midpoint(const Point &a, const Point &b) const
      -> Point {
    const auto alpha = a.dot(this->_l_infty);
    const auto beta = b.dot(this->_l_infty);
    return plucker(alpha, a, beta, b);
  }

  /**
   * @brief
   *
   * @param[in] tri
   * @return auto
   */
  [[nodiscard]] constexpr auto tri_midpoint(const Triple<Point> &tri) const {
    const auto &[a1, a2, a3] = tri;

    return Triple<Point>{this->midpoint(a1, a2), this->midpoint(a2, a3),
                         this->midpoint(a1, a3)};
  }

  /**
   * @brief
   *
   * @param[in] x
   * @return K
   */
  [[nodiscard]] constexpr auto omega(const Point &x) const -> K {
    return sq(x.dot(this->_l_infty));
  }

  /**
   * @brief
   *
   * @param[in] x
   * @return K
   */
  [[nodiscard]] constexpr auto omega(const Line &x) const -> K {
    return sq(x.dot(this->_Ire)) + sq(x.dot(this->_Iim));
  }

  /**
   * @brief
   *
   * @param[in] a1
   * @param[in] a2
   * @return auto
   */
  template <ProjectivePlane2 _Point>
  [[nodiscard]] constexpr auto measure(const _Point &a1,
                                       const _Point &a2) const {
    const auto omg = K(this->omega(a1 * a2));
    const auto den = K(this->omega(a1) * this->omega(a2));
    if constexpr (Integral<K>) {
      return Fraction<K>(omg, den);
    } else {
      return omg / den;
    }
  }

  // /**
  //  * @brief
  //  *
  //  * @param[in] l1
  //  * @param[in] l2
  //  * @return auto
  //  */
  // constexpr auto cross_s(const Line &l1, const Line &l2) const {
  //     return 1 - this->spread(l1, l2); // ???
  // }
};

} // namespace fun

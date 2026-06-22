/** @file pg_common.hpp
 *  This is a C++ Library header.
 */

#pragma once

#include <array>
#include <tuple>

#include "common_concepts.h"

namespace fun {

    /**
     * @brief 1st term of Cross product (yz-plane projection)
     *
     * @f[
     *     \mathrm{cross}_0(v,w) = v_y w_z - w_y v_z
     * @f]
     * @tparam _K
     * @param[in] v
     * @param[in] w
     * @return 1st term of Cross product
     */
    template <Ring _K> auto cross0(const std::array<_K, 3>& v, const std::array<_K, 3>& w) -> _K {
        return v[1] * w[2] - w[1] * v[2];
    }

    /**
     * @brief 2nd term of Cross product (xz-plane projection)
     *
     * @f[
     *     \mathrm{cross}_1(v,w) = v_x w_z - w_x v_z
     * @f]
     * @tparam _K
     * @param[in] v
     * @param[in] w
     * @return 2nd term of Cross product
     */
    template <Ring _K> auto cross1(const std::array<_K, 3>& v, const std::array<_K, 3>& w) -> _K {
        return v[0] * w[2] - w[0] * v[2];
    }

    /**
     * @brief 3rd term of Cross product (xy-plane projection)
     *
     * @f[
     *     \mathrm{cross}_2(v,w) = v_x w_y - w_x v_y
     * @f]
     * @tparam _K
     * @param[in] v
     * @param[in] w
     * @return 3rd term of Cross product
     */
    template <Ring _K> auto cross2(const std::array<_K, 3>& v, const std::array<_K, 3>& w) -> _K {
        return v[0] * w[1] - w[0] * v[1];
    }

    /**
     * @brief Cross product in homogeneous 3D coordinates
     *
     * @f[
     *     v \times w = \begin{pmatrix}
     *         v_y w_z - w_y v_z \\
     *         w_x v_z - v_x w_z \\
     *         v_x w_y - w_x v_y
     *     \end{pmatrix}
     * @f]
     * @tparam _K
     * @param[in] v
     * @param[in] w
     * @return Cross product
     */
    template <typename Point>
        requires Ring<Value_type<Point>>
    auto cross(const Point& v, const Point& w) -> std::array<Value_type<Point>, 3> {
        return {cross0(v, w), -cross1(v, w), cross2(v, w)};
    }

    /**
     * @brief Dot product (full 3-component)
     *
     * @f[
     *     v \cdot w = v_x w_x + v_y w_y + v_z w_z
     * @f]
     * @tparam _K
     * @param[in] v
     * @param[in] w
     * @return auto
     */
    template <Ring _K> auto dot_c(const std::array<_K, 3>& v, const std::array<_K, 3>& w) -> _K {
        const auto& [x1, y1, z1] = v;
        const auto& [x2, y2, z2] = w;
        return x1 * x2 + y1 * y2 + z1 * z2;
    }

    /**
     * @brief generic Plücker linear combination
     *
     * @f[
     *     \lambda v_1 + \mu v_2
     * @f]
     * @tparam _T scalar type
     * @tparam _K coordinate type
     * @param[in] lambda scalar for v1
     * @param[in] v1 first vector
     * @param[in] mu scalar for v2
     * @param[in] v2 second vector
     * @return lambda*v1 + mu*v2
     */
    template <Ring _T, Ring _K> auto plucker_c(const _T& lambda, const std::array<_K, 3>& v1,
                                               const _T& mu, const std::array<_K, 3>& v2)
        -> std::array<_K, 3> {
        const auto& [x1, y1, z1] = v1;
        const auto& [x2, y2, z2] = v2;
        return {lambda * x1 + mu * x2, lambda * y1 + mu * y2, lambda * z1 + mu * z2};
    }

    /**
     * @brief Dot product of the (x,y)-components of two vectors (affine part)
     *
     * @f[
     *     \mathrm{dot}_1(v,w) = v_x w_x + v_y w_y
     * @f]
     * @tparam _K
     * @param[in] v
     * @param[in] w
     * @return auto
     */
    template <Ring _K> auto dot1(const std::array<_K, 3>& v, const std::array<_K, 3>& w) -> _K {
        return v[0] * w[0] + v[1] * w[1];
    }

    /**
     * @brief dot product of the (0,2)-component of two vectors
     *
     * @tparam _K
     * @param[in] v
     * @param[in] w
     * @return auto
     */
    template <Ring _K> auto dot2(const std::array<_K, 3>& v, const std::array<_K, 3>& w) -> _K {
        return v[0] * w[0] + v[2] * w[2];
    }

    /**
     * @brief Square function
     *
     * @f[
     *     \mathrm{sq}(a) = a^2
     * @f]
     * @tparam T data type
     * @param[in] a input value
     * @return a^2
     */
    template <typename T> constexpr auto sq(const T& a) { return a * a; }

    /**
     * @brief Homogeneous parametrization of point or line (free function)
     *
     * Returns lambda * p + mu * q in homogeneous coordinates.
     * This free function is required by the ProjectivePlaneH concept
     * and is used by Involution and other algorithms in proj_plane.hpp.
     * @tparam Value The scalar type (coordinate type)
     * @tparam Point The point or line type
     * @param[in] lambda First scalar coefficient
     * @param[in] pt_p First point/line
     * @param[in] mu Second scalar coefficient
     * @param[in] pt_q Second point/line
     * @return Point The parametrized point/line
     */
    template <typename Value, typename Point>
        requires Ring<Value>
    constexpr auto parametrize(const Value& lambda, const Point& pt_p, const Value& mu,
                               const Point& pt_q) -> Point {
        return Point::parametrize(lambda, pt_p, mu, pt_q);
    }

}  // namespace fun

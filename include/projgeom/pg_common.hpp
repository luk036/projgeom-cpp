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
    template <Ring _K> auto cross0(const std::array<_K, 3>& v_a, const std::array<_K, 3>& v_b)
        -> _K {
        return v_a[1] * v_b[2] - v_b[1] * v_a[2];
    }

    /**
     * @brief 2nd term of Cross product (xz-plane projection)
     *
     * @f[
     *     \mathrm{cross}_1(v_a,v_b) = v_{a,x} v_{b,z} - v_{b,x} v_{a,z}
     * @f]
     * @tparam _K
     * @param[in] v_a
     * @param[in] v_b
     * @return 2nd term of Cross product
     */
    template <Ring _K> auto cross1(const std::array<_K, 3>& v_a, const std::array<_K, 3>& v_b)
        -> _K {
        return v_a[0] * v_b[2] - v_b[0] * v_a[2];
    }

    /**
     * @brief 3rd term of Cross product (xy-plane projection)
     *
     * @f[
     *     \mathrm{cross}_2(v_a,v_b) = v_{a,x} v_{b,y} - v_{b,x} v_{a,y}
     * @f]
     * @tparam _K
     * @param[in] v_a
     * @param[in] v_b
     * @return 3rd term of Cross product
     */
    template <Ring _K> auto cross2(const std::array<_K, 3>& v_a, const std::array<_K, 3>& v_b)
        -> _K {
        return v_a[0] * v_b[1] - v_b[0] * v_a[1];
    }

    /**
     * @brief Cross product in homogeneous 3D coordinates
     *
     * @f[
     *     v_a \times v_b = \begin{pmatrix}
     *         v_{a,y} v_{b,z} - v_{b,y} v_{a,z} \\
     *         v_{b,x} v_{a,z} - v_{a,x} v_{b,z} \\
     *         v_{a,x} v_{b,y} - v_{b,x} v_{a,y}
     *     \end{pmatrix}
     * @f]
     * @tparam _K
     * @param[in] v_a
     * @param[in] v_b
     * @return Cross product
     */
    template <typename Point>
        requires Ring<Value_type<Point>>
    auto cross(const Point& v_a, const Point& v_b) -> std::array<Value_type<Point>, 3> {
        return {cross0(v_a, v_b), -cross1(v_a, v_b), cross2(v_a, v_b)};
    }

    /**
     * @brief Dot product (full 3-component)
     *
     * @f[
     *     v_a \cdot v_b = v_{a,x} v_{b,x} + v_{a,y} v_{b,y} + v_{a,z} v_{b,z}
     * @f]
     * @tparam _K
     * @param[in] v_a
     * @param[in] v_b
     * @return auto
     */
    template <Ring _K> auto dot_c(const std::array<_K, 3>& v_a, const std::array<_K, 3>& v_b)
        -> _K {
        const auto& [x1, y1, z1] = v_a;
        const auto& [x2, y2, z2] = v_b;
        return x1 * x2 + y1 * y2 + z1 * z2;
    }

    /**
     * @brief generic Plücker linear combination
     *
     * @f[
     *     \lambda v_a + \mu v_b
     * @f]
     * @tparam _T scalar type
     * @tparam _K coordinate type
     * @param[in] lambda_val scalar for v_a
     * @param[in] v_a first vector
     * @param[in] mu_val scalar for v_b
     * @param[in] v_b second vector
     * @return lambda_val*v_a + mu_val*v_b
     */
    template <Ring _T, Ring _K> auto plucker_c(const _T& lambda_val, const std::array<_K, 3>& v_a,
                                               const _T& mu_val, const std::array<_K, 3>& v_b)
        -> std::array<_K, 3> {
        const auto& [x1, y1, z1] = v_a;
        const auto& [x2, y2, z2] = v_b;
        return {lambda_val * x1 + mu_val * x2, lambda_val * y1 + mu_val * y2,
                lambda_val * z1 + mu_val * z2};
    }

    /**
     * @brief Dot product of the (x,y)-components of two vectors (affine part)
     *
     * @f[
     *     \mathrm{dot}_1(v_a,v_b) = v_{a,x} v_{b,x} + v_{a,y} v_{b,y}
     * @f]
     * @tparam _K
     * @param[in] v_a
     * @param[in] v_b
     * @return auto
     */
    template <Ring _K> auto dot1(const std::array<_K, 3>& v_a, const std::array<_K, 3>& v_b) -> _K {
        return v_a[0] * v_b[0] + v_a[1] * v_b[1];
    }

    /**
     * @brief Dot product of the (0,2)-component of two vectors
     *
     * @f[
     *     \mathrm{dot}_2(v_a,v_b) = v_{a,x} v_{b,x} + v_{a,z} v_{b,z}
     * @f]
     * @tparam _K
     * @param[in] v_a
     * @param[in] v_b
     * @return auto
     */
    template <Ring _K> auto dot2(const std::array<_K, 3>& v_a, const std::array<_K, 3>& v_b) -> _K {
        return v_a[0] * v_b[0] + v_a[2] * v_b[2];
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
     * @f[
     *     \lambda p + \mu q
     * @f]
     * @param[in] pt_q Second point/line
     * @return Point The parametrized point/line
     */
    template <typename Value, typename Point>
        requires Ring<Value>
    constexpr auto parametrize(const Value& lambda_val, const Point& pt_p, const Value& mu_val,
                               const Point& pt_q) -> Point {
        return Point::parametrize(lambda_val, pt_p, mu_val, pt_q);
    }

}  // namespace fun

/** @file include/pg_common.hpp
 *  This is a C++ Library header.
 */

#pragma once

#include <array>
#include <tuple>

#include "common_concepts.h"

namespace fun {

    /**
     * @brief 1st term of Cross product
     *
     * @tparam _K
     * @param[in] v
     * @param[in] w
     * @return 1st term of Cross product
     */
    template <Ring _K> auto cross0(const std::array<_K, 3> &v, const std::array<_K, 3> &w) -> _K {
        return v[1] * w[2] - w[1] * v[2];
    }

    /**
     * @brief 2nd term of Cross product
     *
     * @tparam _K
     * @param[in] v
     * @param[in] w
     * @return 2nd term of Cross product
     */
    template <Ring _K> auto cross1(const std::array<_K, 3> &v, const std::array<_K, 3> &w) -> _K {
        return v[0] * w[2] - w[0] * v[2];
    }

    /**
     * @brief 3rd term of Cross product
     *
     * @tparam _K
     * @param[in] v
     * @param[in] w
     * @return 3rd term of Cross product
     */
    template <Ring _K> auto cross2(const std::array<_K, 3> &v, const std::array<_K, 3> &w) -> _K {
        return v[0] * w[1] - w[0] * v[1];
    }

    /**
     * @brief Cross product
     *
     * @tparam _K
     * @param[in] v
     * @param[in] w
     * @return Cross product
     */
    template <typename Point>
        requires Ring<Value_type<Point>>
    auto cross(const Point &v, const Point &w) -> std::array<Value_type<Point>, 3> {
        return {cross0(v, w), -cross1(v, w), cross2(v, w)};
    }

    /**
     * @brief Dot product
     *
     * @tparam _K
     * @param[in] v
     * @param[in] w
     * @return auto
     */
    template <Ring _K> auto dot_c(const std::array<_K, 3> &v, const std::array<_K, 3> &w) -> _K {
        const auto &[x1, y1, z1] = v;
        const auto &[x2, y2, z2] = w;
        return x1 * x2 + y1 * y2 + z1 * z2;
    }

    /**
     * @brief generic Plucker function
     *
     * @tparam _K data type
     * @param[in] lambda lamda
     * @param[in] v
     * @param[in] mu
     * @param[in] w
     * @return lamda*v + mu*w
     */
    template <Ring _T, Ring _K> auto plucker_c(const _T &lambda, const std::array<_K, 3> &v1,
                                               const _T &mu,
                                               const std::array<_K, 3> &v2) -> std::array<_K, 3> {
        const auto &[x1, y1, z1] = v1;
        const auto &[x2, y2, z2] = v2;
        return {lambda * x1 + mu * x2, lambda * y1 + mu * y2, lambda * z1 + mu * z2};
    }

    /**
     * @brief dot product of the (0,1)-component of two vectors
     *
     * @tparam _K
     * @param[in] v
     * @param[in] w
     * @return auto
     */
    template <Ring _K> auto dot1(const std::array<_K, 3> &v, const std::array<_K, 3> &w) -> _K {
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
    template <Ring _K> auto dot2(const std::array<_K, 3> &v, const std::array<_K, 3> &w) -> _K {
        return v[0] * w[0] + v[2] * w[2];
    }

    /**
     * @brief Square function
     *
     * @tparam T data type
     * @param[in] a input value
     * @return a^2
     */
    template <typename T> constexpr auto sq(const T &a) { return a * a; }

}  // namespace fun

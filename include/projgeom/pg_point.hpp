// The template and inlines for the -*- C++ -*- pg pt_p classes.
// Initially implemented by Wai-Shing Luk <luk036@gmail.com>
//

/** @file include/pg_point.hpp
 *  This is a C++ Library header.
 */

#pragma once

#include "pg_common.hpp"
#include "pg_object.hpp"

namespace fun {

    // Forward declarations.
    template <Ring _K> struct pg_line;

    template <Ring _K> struct pg_point : pg_object<_K, pg_line<_K>> {
        /// Value typedef.
        using _Base = pg_object<_K, pg_line<_K>>;
        using _Base2 = std::array<_K, 3>;
        // using value_type = _K;

        /**
         * @brief Construct a new pg pt_p object
         *
         */
        explicit pg_point(const pg_point<_K> &) = default;

        /**
         * @brief Construct a new pg pt_p object
         *
         */
        pg_point(pg_point<_K> &&) noexcept = default;

        /**
         * @brief
         *
         * @return pg_point<_K>&
         */
        auto operator=(const pg_point<_K> &) -> pg_point<_K> & = delete;

        /**
         * @brief
         *
         * @return pg_point<_K>&
         */
        auto operator=(pg_point<_K> &&) noexcept -> pg_point<_K> & = default;

        /**
         * @brief Construct a new pg object object
         *
         * @param[in] a array of coordinates
         */
        constexpr explicit pg_point(const _Base2 &a) : _Base{a} {}

        /**
         * @brief Construct a new pg_object object
         *
         * @param[in] x
         * @param[in] y
         * @param[in] z
         */
        constexpr pg_point(const _K &x, const _K &y, const _K &z) : _Base{_Base2{x, y, z}} {}
    };

    /**
     * @brief Return join of two points.
     *
     * @param[in] pt_p
     * @param[in] pt_q
     * @return pg_line<_K>
     */
    template <Ring _K>
    constexpr auto join(const pg_point<_K> &pt_p, const pg_point<_K> &pt_q) -> pg_line<_K> {
        return pt_p * pt_q;
    }

}  // namespace fun

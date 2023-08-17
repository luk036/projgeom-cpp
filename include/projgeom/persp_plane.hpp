#pragma once

#include "ck_plane.hpp"
#include "fractions.hpp"
// #include "pg_common.hpp"
#include "proj_plane.hpp"  // import pg_point, Involution, tri_func,

namespace fun {

    /**
     * @brief
     *
     * @tparam Point
     * @tparam Point::Dual
     */
    template <typename Point, typename Line = typename Point::Dual>
        requires ProjectivePlanePrim<Point, Line>  // c++20 concept
    class persp_euclid_plane : public ck<Point, Line, persp_euclid_plane> {
        using K = Value_type<Point>;

      private:
        Point _I_re;
        Point _I_im;
        Line _l_inf;

      public:
        /**
         * @brief Construct a new persp euclid plane object
         *
         * @param[in] I_re
         * @param[in] I_im
         * @param[in] l_inf
         */
        constexpr persp_euclid_plane(Point I_re, Point I_im, Line l_inf)
            : _I_re{std::move(I_re)}, _I_im{std::move(I_im)}, _l_inf{std::move(l_inf)} {}

        // /**
        //  * @brief Construct a new persp euclid plane object
        //  *
        //  * @param[in] I_re
        //  * @param[in] I_im
        //  * @param[in] l_inf
        //  */
        // constexpr persp_euclid_plane(const Point &I_re, const Point &I_im, const
        // Line &l_inf)
        //     : _I_re{I_re}, _I_im{I_im}, _l_inf{l_inf} {}

        // /**
        //  * @brief
        //  *
        //  * @param[in] x
        //  * @return const Line&
        //  */
        // constexpr const Line &perp(const Point &x) const { return _l_inf; }

        /**
         * @brief
         *
         * @return const Line&
         */
        [[nodiscard]] constexpr auto l_inf() const -> const Line & { return this->_l_inf; }

        /**
         * @brief
         *
         * @param[in] x
         * @return Point
         */
        [[nodiscard]] constexpr auto perp(const Line &v) const -> Point {
            const auto alpha = v.dot(this->_I_re);
            const auto beta = v.dot(this->_I_im);
            return parametrize(alpha, this->_I_re, beta, this->_I_im);
        }

        /**
         * @brief
         *
         * @param[in] ln_l
         * @param[in] ln_m
         * @return true
         * @return false
         */
        [[nodiscard]] constexpr auto is_parallel(const Line &ln_l, const Line &ln_m) const
            -> bool {
            return incident(this->_l_inf, ln_l * ln_m);
        }

        /**
         * @brief
         *
         * @param[in] pt_a
         * @param[in] pt_b
         * @return Point
         */
        [[nodiscard]] constexpr auto midpoint(const Point &pt_a, const Point &pt_b) const -> Point {
            const auto alpha = a.dot(this->_l_inf);
            const auto beta = pt_b.dot(this->_l_inf);
            return parametrize(alpha, pt_a, beta, pt_b);
        }

        /**
         * @brief
         *
         * @param[in] triangle
         * @return auto
         */
        [[nodiscard]] constexpr auto tri_midpoint(const Triple<Point> &triangle) const {
            const auto &[a1, a2, a3] = triangle;

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
            return sq(x.dot(this->_l_inf));
        }

        /**
         * @brief
         *
         * @param[in] x
         * @return K
         */
        [[nodiscard]] constexpr auto omega(const Line &x) const -> K {
            return sq(x.dot(this->_I_re)) + sq(x.dot(this->_I_im));
        }

        /**
         * @brief
         *
         * @param[in] a1
         * @param[in] a2
         * @return auto
         */
        template <ProjectivePlane2 _Point>
        [[nodiscard]] constexpr auto measure(const _Point &a1, const _Point &a2) const {
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

}  // namespace fun

#pragma once

#include "ck_plane.hpp"
#include "fractions.hpp"
// #include "pg_common.hpp"
#include "proj_plane.hpp"  // import pg_point, Involution, tri_func,

namespace fun {

    /**
     * @brief Perspective-Euclidean plane class.
     *
     * A Cayley-Klein plane that combines projective geometry with Euclidean metrics.
     * @tparam Point The point type
     * @tparam Line The line type (dual of point)
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
         * @brief Get the line at infinity.
         *
         * @return const Line& The line at infinity
         */
        [[nodiscard]] constexpr auto l_inf() const -> const Line & { return this->_l_inf; }

        /**
         * @brief Compute the pole of a line.
         *
         * @param[in] v A line
         * @return Point The pole (polar) of the line
         */
        [[nodiscard]] constexpr auto perp(const Line &v) const -> Point {
            const auto alpha = v.dot(this->_I_re);
            const auto beta = v.dot(this->_I_im);
            return parametrize(alpha, this->_I_re, beta, this->_I_im);
        }

        /**
         * @brief Check if two lines are parallel.
         *
         * @param[in] ln_l First line
         * @param[in] ln_m Second line
         * @return true if lines are parallel, false otherwise
         */
        [[nodiscard]] constexpr auto is_parallel(const Line &ln_l, const Line &ln_m) const -> bool {
            return incident(this->_l_inf, ln_l * ln_m);
        }

        /**
         * @brief Compute the midpoint of two points.
         *
         * @param[in] pt_a First point
         * @param[in] pt_b Second point
         * @return Point The midpoint
         */
        [[nodiscard]] constexpr auto midpoint(const Point &pt_a, const Point &pt_b) const -> Point {
            const auto alpha = a.dot(this->_l_inf);
            const auto beta = pt_b.dot(this->_l_inf);
            return parametrize(alpha, pt_a, beta, pt_b);
        }

        /**
         * @brief Compute the midpoints of all three sides of a triangle.
         *
         * @param[in] triangle Array of three points
         * @return Triple<Point> Array of three midpoints
         */
        [[nodiscard]] constexpr auto tri_midpoint(const Triple<Point> &triangle) const {
            const auto &[a1, a2, a3] = triangle;

            return Triple<Point>{this->midpoint(a1, a2), this->midpoint(a2, a3),
                                 this->midpoint(a1, a3)};
        }

        /**
         * @brief Compute the omega value for a point.
         *
         * The omega function measures how far a point is from the line at infinity.
         * @param[in] x A point
         * @return K The omega value
         */
        [[nodiscard]] constexpr auto omega(const Point &x) const -> K {
            return sq(x.dot(this->_l_inf));
        }

        /**
         * @brief Compute the omega value for a line.
         *
         * The omega function measures how far a line is from the ideal points.
         * @param[in] x A line
         * @return K The omega value
         */
        [[nodiscard]] constexpr auto omega(const Line &x) const -> K {
            return sq(x.dot(this->_I_re)) + sq(x.dot(this->_I_im));
        }

        /**
         * @brief Compute the cross-ratio measure between two elements.
         *
         * Returns the ratio of omega values for measuring projective relationships.
         * @param[in] a1 First point or line
         * @param[in] a2 Second point or line
         * @return auto The measure value
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

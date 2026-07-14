/** @file conic.hpp
 *  @brief Conic sections in homogeneous coordinates.
 */

#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

#include "fractions.hpp"
#include "pg_object.hpp"

namespace fun {

    /**
     * @brief Enumeration of conic types based on the discriminant.
     */
    enum class ConicType { Ellipse, Parabola, Hyperbola };

    /**
     * @brief A conic section represented by a symmetric 3x3 matrix.
     *
     * A point \f$\mathbf{x}=(x:y:z)\f$ lies on the conic iff
     * \f$\mathbf{x}^T Q \mathbf{x}=0\f$.
     */
    class Conic {
      public:
        using Mat3x3 = std::array<std::array<Fraction, 3>, 3>;

        /**
         * @brief Construct a new Conic from a symmetric matrix.
         * @param[in] matrix  The 3x3 symmetric matrix Q.
         */
        constexpr explicit Conic(Mat3x3 matrix) : matrix_{std::move(matrix)} {}

        /**
         * @brief Create a circle with centre (cx, cy) and squared radius r².
         *
         *  \f[ (x-c_x)^2 + (y-c_y)^2 = r^2 \f]
         *
         * @param[in] center_x  X coordinate of centre.
         * @param[in] center_y  Y coordinate of centre.
         * @param[in] radius_sq Squared radius.
         * @return constexpr Conic
         */
        static constexpr auto circle(std::int64_t center_x, std::int64_t center_y,
                                     std::int64_t radius_sq) -> Conic {
            const Fraction cx{center_x, 1};
            const Fraction cy{center_y, 1};
            const Fraction r2{radius_sq, 1};
            const Fraction zero{0, 1};
            const Fraction one{1, 1};
            return Conic{Mat3x3{{
                {{one, zero, -cx}},
                {{zero, one, -cy}},
                {{-cx, -cy, cx * cx + cy * cy - r2}},
            }}};
        }

        /**
         * @brief Create a unit circle centred at the origin.
         *
         *  \f[ x^2 + y^2 = 1 \f]
         * @return constexpr Conic
         */
        static constexpr auto unit_circle() -> Conic { return circle(0, 0, 1); }

        /**
         * @brief Create a parabola \f$y = a x^2\f$.
         * @param[in] a  Coefficient.
         * @return constexpr Conic
         */
        static constexpr auto parabola(const Fraction& a) -> Conic {
            const Fraction zero{0, 1};
            const Fraction half{1, 2};
            const auto na = -a;
            return Conic{Mat3x3{{
                {{na, zero, zero}},
                {{zero, zero, half}},
                {{zero, half, zero}},
            }}};
        }

        /**
         * @brief Check whether a point lies on the conic.
         *
         *  \f[ \mathbf{x}^T Q \mathbf{x} = 0 \f]
         * @param[in] pt  The point to test.
         * @return true if the point lies on the conic.
         */
        constexpr auto contains(const PgPoint& point) const -> bool {
            const Fraction x{point.coord[0], 1};
            const Fraction y{point.coord[1], 1};
            const Fraction z{point.coord[2], 1};

            const auto& m = matrix_;
            const auto val = x * (m[0][0] * x + m[0][1] * y + m[0][2] * z)
                            + y * (m[1][0] * x + m[1][1] * y + m[1][2] * z)
                            + z * (m[2][0] * x + m[2][1] * y + m[2][2] * z);
            return val == Fraction{0, 1};
        }

        /**
         * @brief Polar line of a point with respect to the conic.
         *
         *  \f[ \mathbf{l} = Q \mathbf{x} \f]
         * @param[in] point  The point.
         * @return PgLine
         */
        constexpr auto polar(const PgPoint& point) const -> PgLine {
            const Fraction x{point.coord[0], 1};
            const Fraction y{point.coord[1], 1};
            const Fraction z{point.coord[2], 1};

            const auto& m = matrix_;
            const auto a = m[0][0] * x + m[0][1] * y + m[0][2] * z;
            const auto b = m[1][0] * x + m[1][1] * y + m[1][2] * z;
            const auto c = m[2][0] * x + m[2][1] * y + m[2][2] * z;

            return PgLine{{a.numer() / a.denom(), b.numer() / b.denom(), c.numer() / c.denom()}};
        }

        /**
         * @brief Tangent line at a point on the conic.
         *
         * At a point \f$\mathbf{p}\f$ on the conic, the tangent is the polar:
         * \f[ \mathbf{t} = Q \mathbf{p} \f]
         * @param[in] point  A point on the conic.
         * @return PgLine
         */
        constexpr auto tangent(const PgPoint& point) const -> PgLine { return polar(point); }

        /**
         * @brief Pole of a line with respect to the conic (placeholder).
         *
         *  \f[ \mathbf{x} = Q^{-1} \mathbf{l} \f]
         *  Currently returns a point with the same coordinates as the line.
         * @param[in] line  The line.
         * @return PgPoint
         */
        constexpr auto pole(const PgLine& line) const -> PgPoint {
            return PgPoint{{line.coord[0], line.coord[1], line.coord[2]}};
        }

        /**
         * @brief Intersection of a line with the conic (placeholder).
         *
         *  Currently returns an empty vector.
         * @param[in] line  The line.
         * @return std::vector<PgPoint>  (0, 1, or 2 points).
         */
        [[nodiscard]] auto intersect(const PgLine& line) const -> std::vector<PgPoint> {
            (void)line;
            return {};
        }

        /**
         * @brief Discriminant of the conic (det of the 2x2 upper-left block).
         *
         *  \f[ \Delta = a e - b d \f]
         *   >0 → ellipse, =0 → parabola, <0 → hyperbola.
         * @return Fraction
         */
        constexpr auto discriminant() const -> Fraction {
            return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
        }

        /**
         * @brief Determine the conic type from the discriminant.
         * @return ConicType
         */
        constexpr auto conic_type() const -> ConicType {
            const auto d = discriminant();
            if (d > Fraction{0, 1}) return ConicType::Ellipse;
            if (d == Fraction{0, 1}) return ConicType::Parabola;
            return ConicType::Hyperbola;
        }

        /** @brief Access the matrix. */
        constexpr auto matrix() const -> const Mat3x3& { return matrix_; }

        constexpr auto operator==(const Conic& other) const -> bool {
            return matrix_ == other.matrix_;
        }

      private:
        Mat3x3 matrix_;
    };

}  // namespace fun

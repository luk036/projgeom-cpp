/** @file transform.hpp
 *  @brief Projective transformations (translation, rotation, scaling, shear).
 */

#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>

#include "fractions.hpp"
#include "pg_object.hpp"

namespace fun {

    /**
     * @brief A 3×3 projective transformation matrix.
     *
     * Operates on homogeneous coordinates \f$(x:y:z)\f$.
     * Supports translation, rotation, scaling, shear, composition, and inverse.
     */
    class Transform {
      public:
        using Mat3x3 = std::array<std::array<Fraction, 3>, 3>;

        /**
         * @brief Construct a new Transform from a matrix.
         * @param[in] matrix  The 3×3 matrix.
         */
        constexpr explicit Transform(Mat3x3 matrix) : matrix_{std::move(matrix)} {}

        // ---- factory methods ------------------------------------------------

        /**
         * @brief Identity transformation.
         *
         *  \f[ I = \begin{bmatrix}1&0&0\\0&1&0\\0&0&1\end{bmatrix} \f]
         * @return constexpr Transform
         */
        static constexpr auto identity() -> Transform {
            const Fraction Z{0, 1}, O{1, 1};
            return Transform{Mat3x3{{{{O, Z, Z}}, {{Z, O, Z}}, {{Z, Z, O}}}}};
        }

        /**
         * @brief Translation by \f$(t_x, t_y)\f$.
         *
         *  \f[
         *     T(t_x,t_y) = \begin{bmatrix}1&0&t_x\\0&1&t_y\\0&0&1\end{bmatrix}
         *  \f]
         * @param[in] tx  X translation.
         * @param[in] ty  Y translation.
         * @return constexpr Transform
         */
        static constexpr auto translation(std::int64_t tx, std::int64_t ty) -> Transform {
            const Fraction Z{0, 1}, O{1, 1};
            return Transform{
                Mat3x3{{{{O, Z, Fraction{tx, 1}}}, {{Z, O, Fraction{ty, 1}}}, {{Z, Z, O}}}}};
        }

        /**
         * @brief Rotation by angle \f$\theta\f$.
         *
         *  \f[
         *     R(\theta) = \begin{bmatrix}
         *         \cos\theta & -\sin\theta & 0 \\
         *         \sin\theta &  \cos\theta & 0 \\
         *         0 & 0 & 1
         *     \end{bmatrix}
         *  \f]
         * @param[in] cos_a  Cosine of the angle.
         * @param[in] sin_a  Sine of the angle.
         * @return constexpr Transform
         */
        static constexpr auto rotation(const Fraction& angle_cos, const Fraction& angle_sin)
            -> Transform {
            const Fraction Z{0, 1};
            return Transform{Mat3x3{{{{angle_cos, -angle_sin, Z}},
                                     {{angle_sin, angle_cos, Z}},
                                     {{Z, Z, Fraction{1, 1}}}}}};
        }

        /**
         * @brief Scaling by \f$(s_x, s_y)\f$.
         *
         *  \f[
         *     S(s_x,s_y) = \begin{bmatrix}
         *         s_x & 0   & 0 \\
         *         0   & s_y & 0 \\
         *         0   & 0   & 1
         *     \end{bmatrix}
         *  \f]
         * @param[in] sx  X scale factor.
         * @param[in] sy  Y scale factor.
         * @return constexpr Transform
         */
        static constexpr auto scaling(const Fraction& sx, const Fraction& sy) -> Transform {
            const Fraction Z{0, 1};
            return Transform{Mat3x3{{{{sx, Z, Z}}, {{Z, sy, Z}}, {{Z, Z, Fraction{1, 1}}}}}};
        }

        /**
         * @brief Shear transformation.
         *
         *  \f[
         *     H(sh_x,sh_y) = \begin{bmatrix}
         *         1 & sh_x & 0 \\
         *         sh_y & 1 & 0 \\
         *         0 & 0 & 1
         *     \end{bmatrix}
         *  \f]
         * @param[in] shx  X shear factor.
         * @param[in] shy  Y shear factor.
         * @return constexpr Transform
         */
        static constexpr auto shear(const Fraction& shx, const Fraction& shy) -> Transform {
            const Fraction Z{0, 1}, O{1, 1};
            return Transform{Mat3x3{{{{O, shx, Z}}, {{shy, O, Z}}, {{Z, Z, O}}}}};
        }

        // ---- operations -----------------------------------------------------

        /**
         * @brief Compose this transformation with another.
         *
         *  \f[ (M_1 M_2)_{ij} = \sum_k (M_1)_{ik} (M_2)_{kj} \f]
         * @param[in] other  The transform to apply after this one.
         * @return Transform
         */
        constexpr auto compose(const Transform& other) const -> Transform {
            Mat3x3 result{};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    Fraction sum{0, 1};
                    for (int k = 0; k < 3; ++k) {
                        sum = sum + matrix_[i][k] * other.matrix_[k][j];
                    }
                    result[i][j] = sum;
                }
            }
            return Transform{result};
        }

        /**
         * @brief Apply the transformation to a point.
         *
         *  \f[ p' = M p \f]
         * @param[in] pt  The point.
         * @return PgPoint
         */
        constexpr auto apply_point(const PgPoint& point) const -> PgPoint {
            const Fraction x{point.coord[0], 1};
            const Fraction y{point.coord[1], 1};
            const Fraction z{point.coord[2], 1};

            const auto& m = matrix_;
            const auto xn = m[0][0] * x + m[0][1] * y + m[0][2] * z;
            const auto yn = m[1][0] * x + m[1][1] * y + m[1][2] * z;
            const auto zn = m[2][0] * x + m[2][1] * y + m[2][2] * z;

            return PgPoint{
                {xn.numer() / xn.denom(), yn.numer() / yn.denom(), zn.numer() / zn.denom()}};
        }

        /**
         * @brief Apply the transformation to a line (via inverse transpose).
         *
         *  \f[ l' = M^{-T} l \f]
         * @param[in] line  The line.
         * @return PgLine
         */
        constexpr auto apply_line(const PgLine& line) const -> PgLine {
            const auto inv = inverse();
            const Fraction x{line.coord[0], 1};
            const Fraction y{line.coord[1], 1};
            const Fraction z{line.coord[2], 1};

            const auto& m = inv.matrix_;
            const auto xn = m[0][0] * x + m[1][0] * y + m[2][0] * z;
            const auto yn = m[0][1] * x + m[1][1] * y + m[2][1] * z;
            const auto zn = m[0][2] * x + m[1][2] * y + m[2][2] * z;

            return PgLine{
                {xn.numer() / xn.denom(), yn.numer() / yn.denom(), zn.numer() / zn.denom()}};
        }

        /**
         * @brief Compute the inverse transformation.
         *
         *  \f[ M^{-1} = \frac{\operatorname{adj}(M)}{\det(M)} \f]
         * @return Transform
         * @throws std::domain_error if the matrix is singular.
         */
        constexpr auto inverse() const -> Transform {
            const auto& m = matrix_;
            const auto& a = m[0][0];
            const auto& b = m[0][1];
            const auto& c = m[0][2];
            const auto& d = m[1][0];
            const auto& e = m[1][1];
            const auto& f = m[1][2];
            const auto& g = m[2][0];
            const auto& h = m[2][1];
            const auto& i_ = m[2][2];

            const auto det = a * (e * i_ - f * h) - b * (d * i_ - f * g) + c * (d * h - e * g);
            if (det == Fraction{0, 1}) {
                throw std::domain_error{"Cannot invert singular transformation matrix"};
            }
            const auto inv_det = Fraction{1, 1} / det;

            return Transform {
                Mat3x3{{{
                    {inv_det * (e * i_ - f * h), inv_det * (c * h - b * i_),
                     inv_det * (b * f - c * e)},
                    {inv_det * (f * g - d * i_), inv_det * (a * i_ - c * g),
                     inv_det * (c * d - a * f)},
                    {inv_det * (d * h - e * g), inv_det * (b * g - a * h),
                     inv_det * (a * e - b * d)},
                }}};
            }

            /** @brief Access the matrix. */
            constexpr auto matrix() const -> const Mat3x3& { return matrix_; }

            constexpr auto operator==(const Transform& other) const->bool {
                return matrix_ == other.matrix_;
            }

            constexpr auto operator!=(const Transform& other) const->bool {
                return !(*this == other);
            }

          private:
            Mat3x3 matrix_;
        };

        // ---- convenience free functions -----------------------------------------

        /**
         * @brief Rotate a point around the origin.
         * @param[in] pt       The point.
         * @param[in] cos_a    Cosine of the rotation angle.
         * @param[in] sin_a    Sine of the rotation angle.
         * @return PgPoint
         */
        inline constexpr auto rotate_point(const PgPoint& point, const Fraction& angle_cos,
                                           const Fraction& angle_sin) -> PgPoint {
            return Transform::rotation(angle_cos, angle_sin).apply_point(point);
        }

        /**
         * @brief Translate a point by (tx, ty).
         * @param[in] pt  The point.
         * @param[in] tx  X translation.
         * @param[in] ty  Y translation.
         * @return PgPoint
         */
        inline constexpr auto translate_point(const PgPoint& point, std::int64_t tx,
                                              std::int64_t ty) -> PgPoint {
            return Transform::translation(tx, ty).apply_point(point);
        }

        /**
         * @brief Scale a point by (sx, sy).
         * @param[in] pt  The point.
         * @param[in] sx  X scale factor.
         * @param[in] sy  Y scale factor.
         * @return PgPoint
         */
        inline constexpr auto scale_point(const PgPoint& point, const Fraction& sx,
                                          const Fraction& sy) -> PgPoint {
            return Transform::scaling(sx, sy).apply_point(point);
        }

    }  // namespace fun

/** @file transform.hpp
 *  @brief Projective transformations (translation, rotation, scaling, shear).
 */

#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>

#include "fractions.hpp"
#include "matrix3.hpp"
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
        using Mat3x3 = std::array<std::array<Frac, 3>, 3>;

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
            const Frac Z{0, 1}, O{1, 1};
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
            const Frac Z{0, 1}, O{1, 1};
            return Transform{Mat3x3{{{{O, Z, Frac{tx, 1}}}, {{Z, O, Frac{ty, 1}}}, {{Z, Z, O}}}}};
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
        static constexpr auto rotation(const Frac& angle_cos, const Frac& angle_sin) -> Transform {
            const Frac Z{0, 1};
            return Transform{Mat3x3{
                {{{angle_cos, -angle_sin, Z}}, {{angle_sin, angle_cos, Z}}, {{Z, Z, Frac{1, 1}}}}}};
        }

        /**
         * @brief Rotation from the tangent of its half-angle.
         *
         * For a rational \f$t = \tan(\theta/2)\f$ this yields an exact rotation
         * through \f$\theta\f$ with rational entries:
         * \f[ \cos\theta = \frac{1 - t^2}{1 + t^2}, \qquad
         *     \sin\theta = \frac{2 t}{1 + t^2}. \f]
         * @param[in] t  Tangent of half the rotation angle.
         * @return constexpr Transform
         */
        static constexpr auto rotation_from_tangent(const Frac& t) -> Transform {
            const Frac one{1, 1};
            const auto denom = one + t * t;
            return rotation((one - t * t) / denom, (Frac{2, 1} * t) / denom);
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
        static constexpr auto scaling(const Frac& sx, const Frac& sy) -> Transform {
            const Frac Z{0, 1};
            return Transform{Mat3x3{{{{sx, Z, Z}}, {{Z, sy, Z}}, {{Z, Z, Frac{1, 1}}}}}};
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
        static constexpr auto shear(const Frac& shx, const Frac& shy) -> Transform {
            const Frac Z{0, 1}, O{1, 1};
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
                    Frac sum{0, 1};
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
            const Frac x{point.coord[0], 1};
            const Frac y{point.coord[1], 1};
            const Frac z{point.coord[2], 1};

            const auto& m = matrix_;
            const auto xn = m[0][0] * x + m[0][1] * y + m[0][2] * z;
            const auto yn = m[1][0] * x + m[1][1] * y + m[1][2] * z;
            const auto zn = m[2][0] * x + m[2][1] * y + m[2][2] * z;

            return PgPoint{rationalize(Vec3{{xn, yn, zn}})};
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
            const Frac x{line.coord[0], 1};
            const Frac y{line.coord[1], 1};
            const Frac z{line.coord[2], 1};

            const auto& m = inv.matrix_;
            const auto xn = m[0][0] * x + m[1][0] * y + m[2][0] * z;
            const auto yn = m[0][1] * x + m[1][1] * y + m[2][1] * z;
            const auto zn = m[0][2] * x + m[1][2] * y + m[2][2] * z;

            return PgLine{rationalize(Vec3{{xn, yn, zn}})};
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
            if (det == Frac{0, 1}) {
                throw std::domain_error{"Cannot invert singular transformation matrix"};
            }
            const auto inv_det = Frac{1, 1} / det;

            Mat3x3 result{};
            result[0][0] = inv_det * (e * i_ - f * h);
            result[0][1] = inv_det * (c * h - b * i_);
            result[0][2] = inv_det * (b * f - c * e);
            result[1][0] = inv_det * (f * g - d * i_);
            result[1][1] = inv_det * (a * i_ - c * g);
            result[1][2] = inv_det * (c * d - a * f);
            result[2][0] = inv_det * (d * h - e * g);
            result[2][1] = inv_det * (b * g - a * h);
            result[2][2] = inv_det * (a * e - b * d);
            return Transform{result};
        }

        /** @brief Access the matrix. */
        constexpr auto matrix() const -> const Mat3x3& { return matrix_; }

        constexpr auto operator==(const Transform& other) const -> bool {
            return matrix_ == other.matrix_;
        }

        constexpr auto operator!=(const Transform& other) const -> bool {
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
    inline constexpr auto rotate_point(const PgPoint& point, const Frac& angle_cos,
                                       const Frac& angle_sin) -> PgPoint {
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
    inline constexpr auto scale_point(const PgPoint& point, const Frac& sx,
                                      const Frac& sy) -> PgPoint {
        return Transform::scaling(sx, sy).apply_point(point);
    }

    /**
     * @brief Projective transformation mapping four points to four points.
     *
     * The correspondences are normalized against their unit point so that
     * the result satisfies \f$M p_i \sim q_i\f$. The computation is exact
     * over the rationals.
     * @param[in] src  Four source points, no three collinear.
     * @param[in] dst  Four destination points, no three collinear.
     * @return Transform
     * @throws std::domain_error if either set of points is degenerate.
     */
    [[nodiscard]] inline auto projective_transform(const std::array<PgPoint, 4>& src,
                                                   const std::array<PgPoint, 4>& dst) -> Transform {
        const auto support = [](const std::array<PgPoint, 4>& pts) -> Mat3 {
            Mat3 m{};
            for (std::size_t i = 0; i < 3; ++i) {
                for (std::size_t j = 0; j < 3; ++j) {
                    m[i][j] = Frac{pts[j].coord[i], 1};
                }
            }
            return m;
        };
        const auto as_vec = [](const PgPoint& pt) -> Vec3 {
            return Vec3{{Frac{pt.coord[0], 1}, Frac{pt.coord[1], 1}, Frac{pt.coord[2], 1}}};
        };

        const auto src_inverse = inverse3(support(src));
        const auto dst_inverse = inverse3(support(dst));
        const auto alpha = matvec3(src_inverse, as_vec(src[3]));
        const auto beta = matvec3(dst_inverse, as_vec(dst[3]));

        Mat3 diagonal{};
        for (std::size_t i = 0; i < 3; ++i) {
            if (alpha[i] == Frac{0, 1} || beta[i] == Frac{0, 1}) {
                throw std::domain_error{"correspondences are not in general position"};
            }
            diagonal[i][i] = beta[i] / alpha[i];
        }
        return Transform{matmul3(matmul3(support(dst), diagonal), src_inverse)};
    }

}  // namespace fun

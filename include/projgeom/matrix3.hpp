/** @file matrix3.hpp
 *  @brief Exact 3x3 linear-algebra helpers over the rationals.
 */

#pragma once

#include <array>
#include <cstddef>
#include <stdexcept>

#include "fractions.hpp"

namespace fun {

    /** @brief A 3x3 matrix of rational numbers. */
    using Mat3 = std::array<std::array<Frac, 3>, 3>;

    /** @brief A 3-vector of rational numbers. */
    using Vec3 = std::array<Frac, 3>;

    /**
     * @brief Determinant of a 3x3 matrix.
     *
     * @f[ \det(A) = a(ei-fh) - b(di-fg) + c(dh-eg) @f]
     * @param[in] m  The matrix.
     * @return Frac  The determinant.
     */
    [[nodiscard]] constexpr auto det3(const Mat3& m) -> Frac {
        const auto& [a, b, c] = m[0];
        const auto& [d, e, f] = m[1];
        const auto& [g, h, i] = m[2];
        return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
    }

    /**
     * @brief Adjugate (classical adjoint) of a 3x3 matrix.
     *
     * Satisfies \f$m \cdot \operatorname{adj}(m) = \det(m)\,I\f$, so it is the
     * numerator of the inverse and is well defined even for singular matrices.
     *
     * @param[in] m  The matrix.
     * @return Mat3  The adjugate.
     */
    [[nodiscard]] constexpr auto adjugate3(const Mat3& m) -> Mat3 {
        const auto& [a, b, c] = m[0];
        const auto& [d, e, f] = m[1];
        const auto& [g, h, i] = m[2];
        return Mat3{{
            {{e * i - f * h, c * h - b * i, b * f - c * e}},
            {{f * g - d * i, a * i - c * g, c * d - a * f}},
            {{d * h - e * g, b * g - a * h, a * e - b * d}},
        }};
    }

    /**
     * @brief Inverse of a 3x3 matrix (adjugate divided by the determinant).
     *
     * @param[in] m  The matrix.
     * @return Mat3  The inverse.
     * @throws std::domain_error if the matrix is singular.
     */
    [[nodiscard]] constexpr auto inverse3(const Mat3& m) -> Mat3 {
        const auto det = det3(m);
        if (det == Frac{0, 1}) {
            throw std::domain_error{"Cannot invert singular matrix"};
        }
        const auto inv_det = Frac{1, 1} / det;
        const auto adj = adjugate3(m);
        Mat3 result{};
        for (std::size_t i = 0; i < 3; ++i) {
            for (std::size_t j = 0; j < 3; ++j) {
                result[i][j] = adj[i][j] * inv_det;
            }
        }
        return result;
    }

    /**
     * @brief Product of two 3x3 matrices.
     *
     * @param[in] a  Left matrix.
     * @param[in] b  Right matrix.
     * @return Mat3  \f$a b\f$.
     */
    [[nodiscard]] constexpr auto matmul3(const Mat3& a, const Mat3& b) -> Mat3 {
        Mat3 result{};
        for (std::size_t i = 0; i < 3; ++i) {
            for (std::size_t j = 0; j < 3; ++j) {
                result[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j];
            }
        }
        return result;
    }

    /**
     * @brief Product of a 3x3 matrix and a 3-vector.
     *
     * @param[in] m  The matrix.
     * @param[in] v  The vector.
     * @return Vec3  \f$m v\f$.
     */
    [[nodiscard]] constexpr auto matvec3(const Mat3& m, const Vec3& v) -> Vec3 {
        return Vec3{{m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2],
                     m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2],
                     m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2]}};
    }

}  // namespace fun

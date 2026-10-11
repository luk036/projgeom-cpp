/** @file conic.hpp
 *  @brief Conic sections in homogeneous coordinates.
 */

#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <variant>
#include <vector>

#include "fractions.hpp"
#include "matrix3.hpp"
#include "pg_object.hpp"

namespace fun {

    namespace detail {

        /**
         * @brief Integer square root (floor) by binary search, without floating point.
         *
         * @param[in] n  A non-negative integer.
         * @return std::int64_t  \f$\lfloor\sqrt{n}\rfloor\f$, or -1 if n < 0.
         */
        [[nodiscard]] constexpr auto isqrt(std::int64_t n) -> std::int64_t {
            if (n < 0) {
                return -1;
            }
            if (n < 2) {
                return n;
            }
            std::int64_t lo = 1;
            std::int64_t hi = n / 2 + 1;
            while (lo <= hi) {
                const std::int64_t mid = lo + (hi - lo) / 2;
                const std::int64_t sq = mid * mid;
                if (sq == n) {
                    return mid;
                }
                if (sq < n) {
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }
            return hi;
        }

        /**
         * @brief Whether a rational 3-vector is the zero vector.
         *
         * @param[in] v  The vector.
         * @return true if all components are zero.
         */
        [[nodiscard]] constexpr auto is_zero(const Vec3& v) -> bool {
            return v[0] == Frac{0, 1} && v[1] == Frac{0, 1} && v[2] == Frac{0, 1};
        }

        /**
         * @brief Cross product of two rational 3-vectors.
         *
         * @param[in] a  First vector.
         * @param[in] b  Second vector.
         * @return Vec3  \f$a \times b\f$.
         */
        [[nodiscard]] constexpr auto cross3(const Vec3& a, const Vec3& b) -> Vec3 {
            return Vec3{
                {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]}};
        }

        /**
         * @brief Quadratic form \f$x^T Q x\f$.
         */
        [[nodiscard]] constexpr auto quadratic_form(const Mat3& q, const Vec3& x) -> Frac {
            return x[0] * (q[0][0] * x[0] + q[0][1] * x[1] + q[0][2] * x[2])
                   + x[1] * (q[1][0] * x[0] + q[1][1] * x[1] + q[1][2] * x[2])
                   + x[2] * (q[2][0] * x[0] + q[2][1] * x[1] + q[2][2] * x[2]);
        }

        /**
         * @brief Bilinear form \f$x^T Q y\f$.
         */
        [[nodiscard]] constexpr auto bilinear_form(const Mat3& q, const Vec3& x,
                                                   const Vec3& y) -> Frac {
            return x[0] * (q[0][0] * y[0] + q[0][1] * y[1] + q[0][2] * y[2])
                   + x[1] * (q[1][0] * y[0] + q[1][1] * y[1] + q[1][2] * y[2])
                   + x[2] * (q[2][0] * y[0] + q[2][1] * y[1] + q[2][2] * y[2]);
        }

        /**
         * @brief Exact rational square root, when one exists.
         *
         * @param[in] value  A rational number.
         * @return std::optional<Frac>  \f$\sqrt{\text{value}}\f$ if it is a rational
         *     square, otherwise @c std::nullopt.
         */
        [[nodiscard]] constexpr auto rational_sqrt(const Frac& value) -> std::optional<Frac> {
            if (value < Frac{0, 1}) {
                return std::nullopt;
            }
            const auto n = isqrt(value.num());
            const auto d = isqrt(value.den());
            if (n * n == value.num() && d * d == value.den()) {
                return Frac{n, d};
            }
            return std::nullopt;
        }

    }  // namespace detail

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
        using Mat3x3 = std::array<std::array<Frac, 3>, 3>;

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
            const Frac cx{center_x, 1};
            const Frac cy{center_y, 1};
            const Frac r2{radius_sq, 1};
            const Frac zero{0, 1};
            const Frac one{1, 1};
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
        static constexpr auto parabola(const Frac& a) -> Conic {
            const Frac zero{0, 1};
            const Frac half{1, 2};
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
            const Frac x{point.coord[0], 1};
            const Frac y{point.coord[1], 1};
            const Frac z{point.coord[2], 1};

            const auto& m = matrix_;
            const auto val = x * (m[0][0] * x + m[0][1] * y + m[0][2] * z)
                             + y * (m[1][0] * x + m[1][1] * y + m[1][2] * z)
                             + z * (m[2][0] * x + m[2][1] * y + m[2][2] * z);
            return val == Frac{0, 1};
        }

        /**
         * @brief Polar line of a point with respect to the conic.
         *
         *  \f[ \mathbf{l} = Q \mathbf{x} \f]
         * @param[in] point  The point.
         * @return PgLine
         */
        constexpr auto polar(const PgPoint& point) const -> PgLine {
            const Frac x{point.coord[0], 1};
            const Frac y{point.coord[1], 1};
            const Frac z{point.coord[2], 1};

            const auto& m = matrix_;
            const auto a = m[0][0] * x + m[0][1] * y + m[0][2] * z;
            const auto b = m[1][0] * x + m[1][1] * y + m[1][2] * z;
            const auto c = m[2][0] * x + m[2][1] * y + m[2][2] * z;

            return PgLine{rationalize(Vec3{{a, b, c}})};
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
         * @brief Pole of a line with respect to the conic.
         *
         *  \f[ \mathbf{x} = Q^{-1} \mathbf{l}
         *      = \frac{\operatorname{adj}(Q)\, \mathbf{l}}{\det Q} \f]
         *  The scalar \f$1/\det Q\f$ is dropped because the result is a
         *  homogeneous point, so the computation stays exact over the rationals.
         * @param[in] line  The line.
         * @return PgPoint
         * @throws std::domain_error if the conic is degenerate (\f$\det Q = 0\f$).
         */
        constexpr auto pole(const PgLine& line) const -> PgPoint {
            if (det3(matrix_) == Frac{0, 1}) {
                throw std::domain_error{"Cannot compute the pole of a degenerate conic"};
            }
            const auto adj = adjugate3(matrix_);
            const auto& l = line.coord;
            return PgPoint{rationalize(Vec3{{
                adj[0][0] * Frac{l[0], 1} + adj[0][1] * Frac{l[1], 1} + adj[0][2] * Frac{l[2], 1},
                adj[1][0] * Frac{l[0], 1} + adj[1][1] * Frac{l[1], 1} + adj[1][2] * Frac{l[2], 1},
                adj[2][0] * Frac{l[0], 1} + adj[2][1] * Frac{l[1], 1} + adj[2][2] * Frac{l[2], 1},
            }})};
        }

        /**
         * @brief Intersection of a line with the conic.
         *
         * The line is parametrized as \f$\mathbf{x}(t) = u + t\,v\f$ and
         * substituted into \f$\mathbf{x}^T Q \mathbf{x} = 0\f$, giving a
         * quadratic in \f$t\f$. When its discriminant is a perfect square the
         * roots are rational and the intersection points are returned exactly.
         * @param[in] line  The line.
         * @return std::vector<PgPoint>  The intersection points (empty if the line
         *     does not meet the conic in real points).
         * @throws std::domain_error if the intersection is irrational, or the line
         *     lies on the conic.
         */
        [[nodiscard]] auto intersect(const PgLine& line) const -> std::vector<PgPoint> {
            const Vec3 line_coord{
                {Frac{line.coord[0], 1}, Frac{line.coord[1], 1}, Frac{line.coord[2], 1}}};

            std::vector<Vec3> vectors;
            for (std::size_t j = 0; j < 3; ++j) {
                Vec3 axis{{Frac{0, 1}, Frac{0, 1}, Frac{0, 1}}};
                axis[j] = Frac{1, 1};
                const auto candidate = detail::cross3(line_coord, axis);
                if (!detail::is_zero(candidate)) {
                    vectors.push_back(candidate);
                }
            }
            const Vec3 origin = vectors[0];
            Vec3 direction{};
            for (std::size_t i = 1; i < vectors.size(); ++i) {
                if (!detail::is_zero(detail::cross3(origin, vectors[i]))) {
                    direction = vectors[i];
                    break;
                }
            }

            const auto quad = detail::quadratic_form(matrix_, direction);
            const auto lin = Frac{2, 1} * detail::bilinear_form(matrix_, origin, direction);
            const auto constant = detail::quadratic_form(matrix_, origin);

            const auto point_at = [&](const Frac& t) -> PgPoint {
                return PgPoint{
                    rationalize(Vec3{{origin[0] + t * direction[0], origin[1] + t * direction[1],
                                      origin[2] + t * direction[2]}})};
            };

            if (quad == Frac{0, 1}) {
                if (lin == Frac{0, 1}) {
                    throw std::domain_error{"line lies on the conic"};
                }
                return {point_at(-constant / lin)};
            }

            const auto disc = lin * lin - Frac{4, 1} * quad * constant;
            if (disc < Frac{0, 1}) {
                return {};
            }
            const auto root = detail::rational_sqrt(disc);
            if (!root.has_value()) {
                throw std::domain_error{"intersection requires a quadratic surd"};
            }
            if (root.value() == Frac{0, 1}) {
                return {point_at(-lin / (Frac{2, 1} * quad))};
            }
            return {point_at((-lin + root.value()) / (Frac{2, 1} * quad)),
                    point_at((-lin - root.value()) / (Frac{2, 1} * quad))};
        }

        /**
         * @brief Discriminant of the conic (det of the 2x2 upper-left block).
         *
         *  \f[ \Delta = a e - b d \f]
         *   >0 → ellipse, =0 → parabola, <0 → hyperbola.
         * @return Frac
         */
        constexpr auto discriminant() const -> Frac {
            return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
        }

        /**
         * @brief Determine the conic type from the discriminant.
         * @return ConicType
         */
        constexpr auto conic_type() const -> ConicType {
            const auto d = discriminant();
            if (d > Frac{0, 1}) return ConicType::Ellipse;
            if (d == Frac{0, 1}) return ConicType::Parabola;
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

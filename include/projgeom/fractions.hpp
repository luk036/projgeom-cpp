// -*- coding: utf-16 -*-
#pragma once

/*! @file include/fractions.hpp
 *  This is a C++ Library header.
 */

// #include <boost/operators.hpp>
// #include <cmath>
#include <numeric>
#include <type_traits>
#include <utility>

#include "common_concepts.h"

namespace fun {

    /**
     * @brief absolute (for unsigned)
     *
     * @tparam T
     * @param[in] a
     * @return T
     */
    template <typename T>
    requires std::is_unsigned_v<T>
    inline constexpr auto abs(const T& a) -> T { return a; }

    /**
     * @brief absolute
     *
     * @tparam T
     */
    template <typename T>
    requires(!std::is_unsigned_v<T> && ordered_ring<T>) inline constexpr auto abs(const T& a) -> T {
        return (a < T(0)) ? -a : a;
    }

    /*!
     * @brief Greatest common divider
     *
     * @tparam _Mn
     * @param[in] __m
     * @param[in] __n
     * @return _Mn
     */
    template <Integral _Mn> inline constexpr auto gcd_recur(_Mn __m, _Mn __n) -> _Mn {
        if (__n == 0) {
            return abs(__m);
        }
        return gcd_recur(__n, __m % __n);
    }

    /*!
     * @brief Greatest common divider
     *
     * @tparam _Mn
     * @param[in] __m
     * @param[in] __n
     * @return _Mn
     */
    template <Integral _Mn> inline constexpr auto gcd(_Mn __m, _Mn __n) -> _Mn {
        if (__m == 0) {
            return abs(__n);
        }
        return gcd_recur(__m, __n);
    }

    /*!
     * @brief Least common multiple
     *
     * @tparam _Mn
     * @param[in] __m
     * @param[in] __n
     * @return _Mn
     */
    template <Integral _Mn> inline constexpr auto lcm(_Mn __m, _Mn __n) -> _Mn {
        if (__m == 0 || __n == 0) {
            return 0;
        }
        return (abs(__m) / gcd(__m, __n)) * abs(__n);
    }

    template <Integral Z> struct Fraction {
        Z _num;
        Z _den;

        /*!
         * @brief Construct a new Fraction object
         *
         * @param[in] num
         * @param[in] den
         */
        constexpr Fraction(Z num, Z den) : _num{std::move(num)}, _den{std::move(den)} {
            this->normalize();
        }

        constexpr void normalize() {
            if (this->_den < Z(0)) {
                this->_num = -this->_num;
                this->_den = -this->_den;
            }
            const Z common = gcd(this->_num, this->_den);
            if (common == Z(1) || common == Z(0)) {
                return;
            }
            this->_num /= common;
            this->_den /= common;
        }

        /*!
         * @brief Construct a new Fraction object
         *
         * @param[in] num
         */
        constexpr explicit Fraction(Z&& num) : _num{std::move(num)}, _den(Z(1)) {}

        /*!
         * @brief Construct a new Fraction object
         *
         * @param[in] num
         */
        constexpr explicit Fraction(const Z& num) : _num{num}, _den(1) {}

        /*!
         * @brief Construct a new Fraction object
         *
         * @param[in] num
         */
        constexpr Fraction() : _num(0), _den(1) {}

        /*!
         * @brief
         *
         * @return const Z&
         */
        [[nodiscard]] constexpr auto num() const noexcept -> const Z& { return _num; }

        /*!
         * @brief
         *
         * @return const Z&
         */
        [[nodiscard]] constexpr auto den() const noexcept -> const Z& { return _den; }

        /** @name Comparison operators
         *  ==, !=, <, >, <=, >= etc.
         */

        constexpr auto cross(const Fraction& rhs) const -> Z {
            return this->_num * rhs._den - this->_den * rhs._num;
        }

        ///@{

        /**
         * @brief
         *
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator==(const Z& rhs) const -> bool {
            if (this->_den == Z(1) || rhs == Z(0)) {
                return this->_num == rhs;
            }
            return this->_num == this->_den * rhs;
        }

        /**
         * @brief
         *
         * @param[in] rhs
         * @return true
         * @return false
         */
        friend constexpr auto operator==(const Z& lhs, const Fraction& rhs) -> bool {
            return rhs == lhs;
        }

        /**
         * @brief
         *
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator==(const Fraction& rhs) const -> bool {
            if (this->_den == rhs._den) {
                return this->_num == rhs._num;
            }
            const auto f1 = Fraction{this->_num, rhs._num};  // perform gcd()
            const auto f2 = Fraction{this->_den, rhs._den};  // perform gcd()
            return f1.cross(f2) == Z(0);
        }

        /**
         * @brief
         *
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator<(const Z& rhs) const -> bool {
            if (this->_den == Z(1) || rhs == Z(0)) {
                return this->_num < rhs;
            }
            return this->_num < this->_den * rhs;
        }

        /**
         * @brief
         *
         * @param[in] rhs
         * @return true
         * @return false
         */
        friend constexpr auto operator<(const Z& lhs, const Fraction& rhs) -> bool {
            if (rhs._den == Z(1) || lhs == Z(0)) {
                return lhs < rhs._num;
            }
            return lhs * rhs._den < rhs._num;
        }

        /**
         * @brief
         *
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator<(const Fraction& rhs) const -> bool {
            if (this->_den == rhs._den) {
                return this->_num < rhs._num;
            }
            const auto common = gcd(this->_den, rhs._den);
            if (common == Z(0)) {
                return rhs._den * this->_num < this->_den * rhs._num;
            }
            const auto l = this->_den / common;
            const auto r = rhs._den / common;
            return r * this->_num < l * rhs._num;
        }

        /**
         * @brief
         *
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator!=(const Fraction& rhs) const -> bool { return !(*this == rhs); }

        /**
         * @brief Greater than
         *
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator>(const Fraction& rhs) const -> bool { return rhs < *this; }

        /**
         * @brief
         *
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator>=(const Fraction& rhs) const -> bool { return !(*this < rhs); }

        /**
         * @brief
         *
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator<=(const Fraction& rhs) const -> bool { return !(rhs < *this); }

        /**
         * @brief
         *
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator>(const Z& rhs) const -> bool { return rhs < *this; }

        /**
         * @brief
         *
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator<=(const Z& rhs) const -> bool { return !(rhs < *this); }

        /**
         * @brief
         *
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator>=(const Z& rhs) const -> bool { return !(*this < rhs); }

        /**
         * @brief
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return true
         * @return false
         */
        friend constexpr auto operator>(const Z& lhs, const Fraction& rhs) -> bool {
            return rhs < lhs;
        }

        /**
         * @brief
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return true
         * @return false
         */
        friend constexpr auto operator<=(const Z& lhs, const Fraction& rhs) -> bool {
            return !(rhs < lhs);
        }

        /**
         * @brief
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return true
         * @return false
         */
        friend constexpr auto operator>=(const Z& lhs, const Fraction& rhs) -> bool {
            return !(lhs < rhs);
        }

        ///@}

        /*!
         * @brief
         *
         */
        constexpr void reciprocal() noexcept(std::is_nothrow_swappable_v<Z>) {
            std::swap(this->_num, this->_den);
            if (this->_den < Z(0)) {
                this->_num = -this->_num;
                this->_den = -this->_den;
            }
        }

        constexpr auto operator*=(const Fraction& rhs) -> Fraction& {
            const auto f1 = Fraction{this->_num, rhs._den};  // perform gcd()
            const auto f2 = Fraction{rhs._num, this->_den};  // perform gcd()
            this->_num = f1._num * f2._num;
            this->_den = f1._den * f2._den;
            return *this;
        }

        friend constexpr auto operator*(Fraction lhs, const Fraction& rhs) -> Fraction {
            return std::move(lhs) *= rhs;
        }

        constexpr auto operator*=(const Z& rhs) -> Fraction& {
            const auto f2 = Fraction{rhs, this->_den};  // perform gcd()
            this->_num *= f2._num;
            this->_den = f2._den;
            return *this;
        }

        friend constexpr auto operator*(Fraction lhs, const Z& rhs) -> Fraction {
            return std::move(lhs) *= rhs;
        }

        friend constexpr auto operator*(const Z& lhs, Fraction rhs) -> Fraction {
            return std::move(rhs) *= lhs;
        }

        constexpr auto operator/=(Fraction rhs) -> Fraction& {
            rhs.reciprocal();
            return *this *= rhs;
        }

        friend constexpr auto operator/(Fraction lhs, const Fraction& rhs) -> Fraction {
            return std::move(lhs) /= rhs;
        }

        constexpr auto operator/=(const Z& rhs) -> Fraction& {
            const auto f1 = Fraction{this->_num, rhs};  // perform gcd()
            this->_num = f1._num;
            this->_den *= f1._den;
            return *this;
        }

        friend constexpr auto operator/(Fraction lhs, const Z& rhs) -> Fraction {
            return lhs /= rhs;
        }

        friend constexpr auto operator/(const Z& lhs, Fraction rhs) -> Fraction {
            rhs.reciprocal();
            return rhs *= lhs;
        }

        /*!
         * @brief
         *
         * @return Fraction
         */
        constexpr auto operator-() const -> Fraction {
            auto res = Fraction(*this);
            res._num = -res._num;
            return res;
        }

        constexpr auto operator+(const Fraction& rhs) const -> Fraction {
            if (this->_den == rhs._den) {
                return Fraction(this->_num + rhs._num, this->_den);
            }
            const auto common = gcd(this->_den, rhs._den);
            if (common == Z(0)) {
                return Fraction(rhs._den * this->_num + this->_den * rhs._num, Z(0));
            }
            const auto l = this->_den / common;
            const auto r = rhs._den / common;
            auto d = this->_den * r;
            auto n = r * this->_num + l * rhs._num;
            return Fraction(std::move(n), std::move(d));
        }

        /*!
         * @brief
         *
         * @param[in] frac
         * @return Fraction
         */
        constexpr auto operator-(const Fraction& frac) const -> Fraction { return *this + (-frac); }

        /*!
         * @brief
         *
         * @param[in] i
         * @return Fraction
         */
        constexpr auto operator+(const Z& i) const -> Fraction {
            auto n = this->_num + this->_den * i;
            return Fraction(std::move(n), this->_den);
        }

        /*!
         * @brief
         *
         * @param[in] i
         * @return Fraction
         */
        constexpr auto operator-(const Z& i) const -> Fraction { return *this + (-i); }

        /*!
         * @brief
         *
         * @param[in] frac
         * @return Fraction
         */
        constexpr auto operator+=(const Fraction& frac) -> Fraction& {
            return *this = *this + frac;
        }

        /*!
         * @brief
         *
         * @param[in] frac
         * @return Fraction
         */
        constexpr auto operator-=(const Fraction& frac) -> Fraction& {
            return *this = *this - frac;
        }


        /*!
         * @brief
         *
         * @param[in] i
         * @return Fraction
         */
        constexpr auto operator+=(const Z& i) -> Fraction& { return *this = *this + i; }

        /*!
         * @brief
         *
         * @param[in] i
         * @return Fraction
         */
        constexpr auto operator-=(const Z& i) -> Fraction& { return *this = *this - i; }

    };

    /*!
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction<Z>
     */
    template <typename Z> constexpr auto operator+(const Z& c, const Fraction<Z>& frac)
        -> Fraction<Z> {
        return frac + c;
    }

    /*!
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction<Z>
     */
    template <typename Z> constexpr auto operator-(const Z& c, const Fraction<Z>& frac)
        -> Fraction<Z> {
        return c + (-frac);
    }

    // /*!
    //  * @brief
    //  *
    //  * @param[in] c
    //  * @param[in] frac
    //  * @return Fraction<Z>
    //  */
    // template <typename Z>
    // constexpr Fraction<Z> operator*(const Z& c, const Fraction<Z>& frac)
    // {
    //     return frac * c;
    // }

    /*!
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction<Z>
     */
    template <typename Z> constexpr auto operator+(int&& c, const Fraction<Z>& frac)
        -> Fraction<Z> {
        return frac + Z(c);
    }

    /*!
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction<Z>
     */
    template <typename Z> constexpr auto operator-(int&& c, const Fraction<Z>& frac)
        -> Fraction<Z> {
        return (-frac) + Z(c);
    }

    /*!
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction<Z>
     */
    template <typename Z> constexpr auto operator*(int&& c, const Fraction<Z>& frac)
        -> Fraction<Z> {
        return frac * Z(c);
    }

    /*!
     * @brief
     *
     * @tparam _Stream
     * @tparam Z
     * @param[in] os
     * @param[in] frac
     * @return _Stream&
     */
    template <typename _Stream, typename Z> auto operator<<(_Stream& os, const Fraction<Z>& frac)
        -> _Stream& {
        os << "(" << frac.num() << "/" << frac.den() << ")";
        return os;
    }

    // For template deduction
    // Integral{Z} Fraction(const Z &, const Z &) noexcept -> Fraction<Z>;

}  // namespace fun

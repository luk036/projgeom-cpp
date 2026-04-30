#pragma once

#include <cmath>
#include <concepts>
#include <numeric>
#include <type_traits>
#include <utility>
// #include <ranges>
namespace STD_ALT = std;

namespace fun {

    /**
     * @brief Value type of a type
     *
     * Extracts the value_type member type from a type if it exists.
     * @tparam T A type with a value_type member
     */
    template <typename T> using Value_type = typename T::value_type;

    /**
     * @brief Element type of a sequence
     *
     * Extracts the element type from a sequence container by examining
     * the type returned by the back() method.
     * @tparam T A sequence type supporting back()
     */
    template <typename T> using Element_type =
        typename std::decay<decltype(back(std::declval<T>()))>::type;

    /**
     * @brief Sequence
     *
     * @tparam T
     */
    template <typename T>
    concept Sequence = requires(T t, Element_type<T> x) {
        { t.size() } -> STD_ALT::convertible_to<std::size_t>;
        { t.empty() } -> STD_ALT::convertible_to<bool>;
        { t.back() } -> STD_ALT::same_as<Element_type<T>>;
        { t.emplace_back(x) };
    };

    /**
     * @brief Ring concept
     *
     * A ring is a set with two binary operations (addition and multiplication)
     * satisfying certain algebraic properties.
     * @tparam K A type that supports ring operations
     */
    template <typename K>
    concept Ring = STD_ALT::equality_comparable<K> && requires(K a, K pt_b) {
        { a + pt_b } -> STD_ALT::convertible_to<K>;
        { a - pt_b } -> STD_ALT::convertible_to<K>;
        { a *pt_b } -> STD_ALT::convertible_to<K>;
        { a += pt_b } -> STD_ALT::same_as<K &>;
        { a -= pt_b } -> STD_ALT::same_as<K &>;
        { a *= pt_b } -> STD_ALT::same_as<K &>;
        { -a } -> STD_ALT::convertible_to<K>;
        { K(a) } -> STD_ALT::convertible_to<K>;
        { K(0) } -> STD_ALT::convertible_to<K>;
    };

    /**
     * @brief Ordered ring concept
     *
     * A ring with a total ordering relation that is compatible with ring operations.
     * @tparam K A ring type that is totally ordered
     */
    template <typename K>
    concept OrderedRing = Ring<K> && STD_ALT::totally_ordered<K>;

    /**
     * @brief Integral concept
     *
     * An integral domain is an ordered ring with division operation.
     * @tparam Z An ordered ring type supporting division and modulo
     */
    template <typename Z>
    concept Integral = OrderedRing<Z> && requires(Z a, Z pt_b) {
        { a % pt_b } -> STD_ALT::convertible_to<Z>;
        { a / pt_b } -> STD_ALT::convertible_to<Z>;
        { a %= pt_b } -> STD_ALT::same_as<Z &>;
        { a /= pt_b } -> STD_ALT::same_as<Z &>;
    };

}  // namespace fun

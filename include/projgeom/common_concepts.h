#pragma once

#include <cmath>
#include <concepts>
#include <numeric>
#include <type_traits>
#include <utility>
// #include <ranges>
namespace STD_ALT = std;

namespace fun {

    template <typename T> using Value_type = typename T::value_type;

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
        {t.push_back(x)};
    };

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

    template <typename K>
    concept OrderedRing = Ring<K> && STD_ALT::totally_ordered<K>;

    template <typename Z>
    concept Integral = OrderedRing<Z> && requires(Z a, Z pt_b) {
        { a % pt_b } -> STD_ALT::convertible_to<Z>;
        { a / pt_b } -> STD_ALT::convertible_to<Z>;
        { a %= pt_b } -> STD_ALT::same_as<Z &>;
        { a /= pt_b } -> STD_ALT::same_as<Z &>;
    };

}  // namespace fun

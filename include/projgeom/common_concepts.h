#pragma once

#include <cmath>
#include <numeric>
#include <type_traits>
#include <utility>

#if __has_include(<concepts>)
#    include <concepts>
namespace std_alt = std;
#elif __has_include(<concepts/concepts.hpp>)
#    include <concepts/concepts.hpp>
#    include <range/v3/utility/concepts.hpp>
namespace std_alt = concepts;
#endif

namespace fun {

    template <typename T> using Value_type = typename T::value_type;

    template <typename T> using Element_type =
        typename std::decay<decltype(back(std::declval<T>()))>::type;

    // /**
    //  * @brief Sequence
    //  *
    //  * @tparam T
    //  */
    // template <typename T>
    // concept Sequence = requires(T t, Element_type<T> x) {
    //     { t.size() } -> std_alt::convertible_to<size_t>;
    //     { t.empty() } -> std_alt::convertible_to<bool>;
    //     { t.back() } -> std_alt::same_as<Element_type<T> >;
    //     {t.push_back(x)};
    // };

    template <typename K>
    concept ring = std_alt::equality_comparable<K> && requires(K a, K b) {
        { a + b } -> std_alt::convertible_to<K>;
        { a - b } -> std_alt::convertible_to<K>;
        { a* b } -> std_alt::convertible_to<K>;
        { a += b } -> std_alt::same_as<K&>;
        { a -= b } -> std_alt::same_as<K&>;
        { a *= b } -> std_alt::same_as<K&>;
        { -a } -> std_alt::convertible_to<K>;
        { K(a) } -> std_alt::convertible_to<K>;
        { K(0) } -> std_alt::convertible_to<K>;
    };

    template <typename K>
    concept ordered_ring = ring<K> && std_alt::totally_ordered<K>;

    template <typename Z>
    concept Integral = ordered_ring<Z> && requires(Z a, Z b) {
        { a % b } -> std_alt::convertible_to<Z>;
        { a / b } -> std_alt::convertible_to<Z>;
        { a %= b } -> std_alt::same_as<Z&>;
        { a /= b } -> std_alt::same_as<Z&>;
    };

}  // namespace fun

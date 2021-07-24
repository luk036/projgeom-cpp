#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <compare>

inline constexpr auto operator<=>(
    const boost::multiprecision::cpp_int& x,
    const boost::multiprecision::cpp_int& y) -> std::strong_ordering {
    if (x < y) return std::strong_ordering::less;
    if (x > y) return std::strong_ordering::greater;
    return std::strong_ordering::equivalent; 
}

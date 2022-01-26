#pragma once

namespace cexpr {
namespace math {

/**
 * pow
 *
 * Raises base to power.
 */
template<int base, int power>
struct pow {
    constexpr static int value = base * pow<base, power-1>::value;
};

template<int base>
struct pow<base, 1> {
    constexpr static int value = base;
};


/**
 * abs
 */
template<std::int64_t val, typename = void>
struct abs;

template<std::int64_t val>
struct abs<val, std::enable_if_t<val < 0>> {
    constexpr static std::int64_t value = -val;
};

template<std::int64_t val>
struct abs<val, std::enable_if_t<val >= 0>> {
    constexpr static std::int64_t value = val;
};


/**
 * math
 */
template<auto lhs, auto rhs, typename = void>
struct max_pred;

template<auto lhs, auto rhs>
struct max_pred<lhs, rhs, std::enable_if_t<lhs <= rhs>> {
    constexpr static decltype(rhs) value = rhs;
};

template<auto lhs, auto rhs>
struct max_pred<lhs, rhs, std::enable_if_t<(lhs > rhs)>> {
    constexpr static decltype(lhs) value = lhs;
};

template<auto lhs, auto rhs>
using max = max_pred<lhs, rhs>;

/**
 * min
 */
template<auto lhs, auto rhs, typename = void>
struct min_pred;

template<auto lhs, auto rhs>
struct min_pred<lhs, rhs, std::enable_if_t<lhs >= rhs>> {
    constexpr static decltype(rhs) value = rhs;
};

template<auto lhs, auto rhs>
struct min_pred<lhs, rhs, std::enable_if_t<(lhs < rhs)>> {
    constexpr static decltype(lhs) value = lhs;
};

template<auto lhs, auto rhs>
using min = min_pred<lhs, rhs>;

}; // namespace math
}; // namespace cexpr

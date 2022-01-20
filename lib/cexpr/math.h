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

}; // namespace math
}; // namespace cexpr

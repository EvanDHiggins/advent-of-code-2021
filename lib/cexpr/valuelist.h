#pragma once

namespace cexpr {
namespace valuelist {

template<auto... ints>
struct IntList;

/**
 * nth
 *
 * Returns the nth element of intlist.
 */
template<int n, typename intlist>
struct nth;

template<auto head, auto... rest>
struct nth<0, IntList<head, rest...>> {
    constexpr static int value = head;
};

template<auto n, auto head, auto... rest>
struct nth<n, IntList<head, rest...>> {
    constexpr static decltype(n) value = nth<n - 1, IntList<rest...>>::value;
};

/**
 * prepend
 *
 * prepends val to intlist.
 */
template<auto val, typename intlist>
struct prepend;

template<auto val, auto... items>
struct prepend<val, IntList<items...>> {
    using type = IntList<val, items...>;
};

template<auto val>
struct prepend<val, cexpr::List<>> {
    using type = IntList<val>;
};

template<auto val, typename intlist>
using prepend_t = typename prepend<val, intlist>::type;


/**
 * ifmap
 *
 * Maps func over intlist.
 */
template<template<auto> typename func, typename intlist>
struct ifmap;

template<template<auto> typename func>
struct ifmap<func, IntList<>> {
    using type = IntList<>;
};

template<template<auto> typename func, int head, int... rest>
struct ifmap<func, IntList<head, rest...>> {
    using type = prepend_t<
        func<head>::value,
        typename ifmap<func, IntList<rest...>>::type
    >;
};

/**
 * zip
 *
 * Zips lhs and rhs with func.
 */
template<template<int, int> typename func, typename lhs, typename rhs>
struct zip;

template<
    template<int, int> typename func,
    int lhead, int... lrest,
    int rhead, int... rrest>
struct zip<
    func, IntList<lhead, lrest...>, IntList<rhead, rrest...>> {
    using type = prepend_t<
        func<lhead, rhead>::value,
        typename zip<func, IntList<lrest...>, IntList<rrest...>>::type
    >;
};
template<template<int, int> typename func>
struct zip<func, IntList<>, IntList<>> {
    using type = IntList<>;
};


}; // namespace valuelist
}; // namespace cexpr

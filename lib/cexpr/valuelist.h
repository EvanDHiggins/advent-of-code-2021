#pragma once

#include "lib/cexpr/math.h"

namespace cexpr {
namespace valuelist {

template<auto... ints>
struct ValueList;

template<char... cs>
using String = ValueList<cs...>;

/**
 * nth
 *
 * Returns the nth element of intlist.
 */
template<int n, typename intlist>
struct nth;

template<auto head, auto... rest>
struct nth<0, ValueList<head, rest...>> {
    constexpr static int value = head;
};

template<auto n, auto head, auto... rest>
struct nth<n, ValueList<head, rest...>> {
    constexpr static decltype(n) value = nth<n - 1, ValueList<rest...>>::value;
};

/**
 * prepend
 *
 * prepends val to intlist.
 */
template<auto val, typename intlist>
struct prepend;

template<auto val, auto... items>
struct prepend<val, ValueList<items...>> {
    using type = ValueList<val, items...>;
};

template<auto val>
struct prepend<val, ValueList<>> {
    using type = ValueList<val>;
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
struct ifmap<func, ValueList<>> {
    using type = ValueList<>;
};

template<template<auto> typename func, auto head, auto... rest>
struct ifmap<func, ValueList<head, rest...>> {
    using type = prepend_t<
        func<head>::value,
        typename ifmap<func, ValueList<rest...>>::type
    >;
};

/**
 * zip
 *
 * Zips lhs and rhs with func.
 */
template<template<auto, auto> typename func, typename lhs, typename rhs>
struct zip;

template<
    template<auto, auto> typename func,
    auto lhead, auto... lrest,
    auto rhead, auto... rrest>
struct zip<
    func, ValueList<lhead, lrest...>, ValueList<rhead, rrest...>> {
    using type = prepend_t<
        func<lhead, rhead>::value,
        typename zip<func, ValueList<lrest...>, ValueList<rrest...>>::type
    >;
};
template<template<auto, auto> typename func>
struct zip<func, ValueList<>, ValueList<>> {
    using type = ValueList<>;
};

/**
 * is_empty
 *
 * Assigns ::value to the result.
 */
template<typename str>
struct is_empty {
    constexpr static bool value = false;
};

template<>
struct is_empty<ValueList<>> {
    constexpr static bool value = true;
};

/**
 * append
 *
 * Appends c to str.
 */
template<auto c, typename str>
struct append;

template<auto to_append, auto... cs>
struct append<to_append, ValueList<cs...>> {
    using type = ValueList<cs..., to_append>;
};

template<auto c, typename str>
using append_t = typename append<c, str>::type;

/**
 * to_int
 *
 * Converts string to integer.
 */
constexpr int value_to_int(char c) {
    return c - '0';
};

constexpr int value_to_int(int c) {
    return c;
};

template<int radix, typename lst>
struct to_int;

template<int radix, auto x, auto... xs>
struct to_int<radix, ValueList<x, xs...>> {
    constexpr static int value =
        (value_to_int(x) * cexpr::math::pow<radix, sizeof...(xs)>::value)
        + to_int<radix, ValueList<xs...>>::value;
};

template<int radix, auto x>
struct to_int<radix, ValueList<x>> {
    constexpr static int value = value_to_int(x);
};

/**
 * concat
 */
template<typename lst, typename... lsts>
struct concat;

template<auto... lst1_elems, auto... lst2_elems, typename... rest>
struct concat<ValueList<lst1_elems...>, ValueList<lst2_elems...>, rest...> {
    using type =
        typename concat<
            ValueList<lst1_elems..., lst2_elems...>, rest...>::type;
};

template<auto... lst1_elems, auto... lst2_elems>
struct concat<ValueList<lst1_elems...>, ValueList<lst2_elems...>> {
    using type = ValueList<lst1_elems..., lst2_elems...>;
};

/**
 * reverse
 *
 * Reversed the characters in lst.
 */
template<typename lst>
struct reverse;

template<typename lst1, typename lst>
struct reverse_helper;

template<auto... reversed, auto x, auto... xs>
struct reverse_helper<
    ValueList<reversed...>,
    ValueList<x, xs...>>
    : reverse_helper<ValueList<x, reversed...>, ValueList<xs...>>
{};

template<typename reversed>
struct reverse_helper<reversed, ValueList<>> {
    using type = reversed;
};

template<typename lst>
struct reverse {
    using type = typename reverse_helper<ValueList<>, lst>::type;
};

}; // namespace valuelist
}; // namespace cexpr

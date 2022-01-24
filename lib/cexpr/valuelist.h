#pragma once

#include "lib/cexpr/math.h"
#include "lib/cexpr/list.h"

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
 * take
 *
 * Returns a list of the first n elements of lst.
 */
template<int n, typename lst>
struct take;

template<int n, auto elem, auto... elems>
struct take<n, ValueList<elem, elems...>> {
    using type = prepend_t<
        elem,
        take<n - 1, ValueList<elems...>>
    >;
};

template<int n>
struct take<n, ValueList<>> {
    using type = ValueList<>;
};

template<typename lst>
struct take<0, lst> {
    using type = ValueList<>;
};

template<int n, typename lst>
using take_t = typename take<n, lst>::type;

/**
 * drop
 *
 * Returns a list of the first n elements of lst.
 */
template<int n, typename lst>
struct drop;

template<int n, auto elem, auto... elems>
struct drop<n, ValueList<elem, elems...>> {
    using type = typename drop<n - 1, ValueList<elems...>>::type;
};

template<int n>
struct drop<n, ValueList<>> {
    using type = ValueList<>;
};

template<typename lst>
struct drop<0, lst> {
    using type = lst;
};

template<int n, typename lst>
using drop_t = typename drop<n, lst>::type;

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


/**
 * split
 */
constexpr bool is_whitespace(char c) {
    return c == '\n' || c == ' ' || c == '\t';
}

template<
    char on,
    typename tokens,
    typename str_buffer,
    typename rest,
    typename = void>
struct split_helper;

// Guards against putting lots of empty strings into our tokens list.
template<char on, typename tokens, char c, char... rest>
struct split_helper<
    on, tokens, String<>, String<c, rest...>,
    std::enable_if_t<c == on>>
  : split_helper<on, tokens, String<>, String<rest...>>
{};

template<char on, typename tokens, char c>
struct split_helper<
    on, tokens, String<>, String<c>, std::enable_if_t<c == on>>
  : split_helper<on, tokens, String<>, String<>>
{};

template<char on, typename tokens, char c>
struct split_helper<
    on, tokens, String<>, String<c>,
    std::enable_if_t<c != on>>
  : split_helper<
        on,
        cexpr::list::append_t<String<c>, tokens>,
        String<>, String<>>
{};

template<char on, typename tokens, typename str>
struct split_helper<
    on, tokens, str, String<>,
    std::enable_if_t<!cexpr::valuelist::is_empty<str>::value>>
  : split_helper<
        on,
        cexpr::list::append_t<str, tokens>,
        String<>, String<>>
{};

template<char on, typename tokens, typename str, char c, char... rest>
struct split_helper<
    on, tokens, str, String<c, rest...>, std::enable_if_t<c == on>> 
 : split_helper<
    on,
    cexpr::list::append_t<str, tokens>,
    String<>, String<rest...>>
{};

template<char on, typename tokens, typename str, char c>
struct split_helper<
    on, tokens, str, String<c>, std::enable_if_t<c == on>> 
  : split_helper<
        on, cexpr::list::append_t<str, tokens>, String<>, String<>>
{};

template<char on, typename tokens>
struct split_helper<on, tokens, String<>, String<>> {
    using type = tokens;
};

template<char on, typename tokens, typename str_buffer, char c, char... rest>
struct split_helper<
    on, tokens, str_buffer, String<c, rest...>,
    std::enable_if_t<c != on>>
  : split_helper<
        on, tokens, cexpr::valuelist::append_t<c, str_buffer>, String<rest...>>
{};

template<char on, typename str>
struct split : split_helper<on, List<>, String<>, str> {};

}; // namespace valuelist
}; // namespace cexpr

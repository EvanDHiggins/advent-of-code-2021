#pragma once

#include "lib/cexpr/math.h"
#include "lib/cexpr/list.h"

namespace cexpr {
namespace valuelist {

template<auto... ints>
struct ValueList;

template<char... cs>
using String = ValueList<cs...>;

template<typename t>
struct test;

/**
 * head
 */
template<typename valuelist>
struct head;

template<auto _head, auto... rest>
struct head<ValueList<_head, rest...>> {
    constexpr static decltype(_head) value = _head;
};

/**
 * nth
 *
 * Returns the nth element of intlist.
 */
template<int n, typename valuelist>
struct nth;

template<auto head, auto... rest>
struct nth<0, ValueList<head, rest...>> {
    constexpr static decltype(head) value = head;
};

template<auto n, auto head, auto... rest>
struct nth<n, ValueList<head, rest...>> {
    constexpr static decltype(head) value =
        nth<n - 1, ValueList<rest...>>::value;
};

/**
 * tail
 */
template<typename valuelist>
struct tail;

template<auto head, auto... _tail>
struct tail<ValueList<head, _tail...>> {
    using type = ValueList<_tail...>;
};

template<typename valuelist>
using tail_t = typename tail<valuelist>::type;


template<auto val1, auto val2, typename valuelist>
struct prepend_multiple;

template<auto val1, auto val2, auto... elements>
struct prepend_multiple<val1, val2, ValueList<elements...>> {
    using type = ValueList<val1, val2, elements...>;
};

template<auto val1, auto val2>
struct prepend_multiple<val1, val2, ValueList<>> {
    using type = ValueList<val1, val2>;
};


template<auto val1, auto val2, typename valuelist>
using prepend_multiple_t = typename prepend_multiple<
    val1, val2, valuelist>::type;

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
 * set_nth
 */
template<int n, auto val, typename valuelist, typename = void>
struct set_nth;

template<int n, auto val, auto head, auto... rest>
struct set_nth<
    n, val, ValueList<head, rest...>,
    std::enable_if_t<n != 0>> {
    using type = prepend_t<
            head,
            typename set_nth<n - 1, val, ValueList<rest...>>::type
        >;
};

template<auto val, auto head, auto... values>
struct set_nth<0, val, ValueList<head, values...>> {
    using type = ValueList<val, values...>;
};

template<int n, auto val, typename valuelist>
using set_nth_t = typename set_nth<n, val, valuelist>::type;


template<std::uint64_t... values>
using LongList = ValueList<values...>;

/**
 * sum
 */
template<typename intlist, typename intwidth = std::int64_t>
struct sum;

template<auto acc, typename intlist>
struct sum_aux;

template<auto acc, auto head, auto... rest>
struct sum_aux<acc, ValueList<head, rest...>> {
    constexpr static decltype(acc) value =
        sum_aux<acc + head, LongList<rest...>>::value;
};

template<auto acc>
struct sum_aux<acc, ValueList<>> {
    constexpr static decltype(acc) value = acc;
};


template<typename intlist, typename intwidth>
struct sum : sum_aux<(intwidth)0, intlist> {};

/**
 * length
 */
template<typename lst>
struct length;

template<auto... elements>
struct length<ValueList<elements...>> {
    constexpr static std::size_t value = sizeof...(elements);
};

/**
 * fmap
 *
 * Maps func over intlist.
 */
template<template<auto> typename func, typename valuelist>
struct fmap;

template<template<auto> typename func>
struct fmap<func, ValueList<>> {
    using type = ValueList<>;
};

template<template<auto> typename func, auto head, auto... rest>
struct fmap<func, ValueList<head, rest...>> {
    using type = prepend_t<
        func<head>::value,
        typename fmap<func, ValueList<rest...>>::type
    >;
};

// Old calls use "ifmap" because that's what I called it when this was just
// an IntList. Using decl lets me update the name without renaming it
// everywhere.
template<template<auto> typename func, typename valuelist>
using ifmap = fmap<func, valuelist>;

template<template<auto> typename func, typename valuelist>
using fmap_t = typename fmap<func, valuelist>::type;

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

template<int radix, typename lst, typename intwidth = std::int64_t>
struct to_int;

template<int radix, auto x, auto... xs, typename intwidth>
struct to_int<radix, ValueList<x, xs...>, intwidth> {
    constexpr static intwidth value =
        (value_to_int(x) * cexpr::math::pow<radix, sizeof...(xs)>::value)
        + to_int<radix, ValueList<xs...>>::value;
};

template<int radix, auto x, typename intwidth>
struct to_int<radix, ValueList<x>, intwidth> {
    constexpr static intwidth value = value_to_int(x);
};

template<typename str>
using to_base_ten = cexpr::valuelist::to_int<10, str>;

template<typename str>
using to_base_two = cexpr::valuelist::to_int<2, str>;

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
template<int n, typename lst, typename = void>
struct take;

template<int n, auto elem, auto... elems>
struct take<n, ValueList<elem, elems...>, std::enable_if_t<n != 0>> {
    using type = prepend_t<
        elem,
        typename take<n - 1, ValueList<elems...>>::type
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

template<char on, typename str>
using split_t = typename split<on, str>::type;

/**
 * fmap_to_value
 *
 * Maps a template which defines ::value over a List of typenames. Defines
 * a typedef, type, which is a ValueList of the mapped values.
 */
template<template<typename> typename func, typename list>
struct fmap_to_value;

template<template<typename> typename func, typename elem, typename... elems>
struct fmap_to_value<func, List<elem, elems...>> {
    using type = prepend_t<
        func<elem>::value,
        typename fmap_to_value<func, List<elems...>>::type>;
};

template<template<typename> typename func>
struct fmap_to_value<func, List<>> {
    using type = ValueList<>;
};

template<template<typename> typename func, typename lst>
using fmap_to_value_t = typename fmap_to_value<func, lst>::type;

/**
 * fmap_to_list
 *
 * The inverted form of fmap_to_value. Maps a func from 'value -> type' over
 * a ValueList and returns a List.
 */
template<template<auto> typename func, typename list>
struct fmap_to_list;

template<template<auto> typename func, auto elem, auto... elems>
struct fmap_to_list<func, ValueList<elem, elems...>> {
    using type = cexpr::list::prepend_t<
        typename func<elem>::type,
        typename fmap_to_list<func, ValueList<elems...>>::type>;
};

template<template<auto> typename func>
struct fmap_to_list<func, ValueList<>> {
    using type = List<>;
};

template<template<auto> typename func, typename lst>
using fmap_to_list_t = typename fmap_to_list<func, lst>::type;

/**
 * box_value
 */
template<auto v>
struct box_value : std::integral_constant<decltype(v), v> {};

/**
 * unbox_value
 */
template<typename b>
struct unbox_value {
    constexpr static decltype(b::value) value = b::value;
};

/**
 * gen
 */
template<std::uint64_t count, auto v>
struct gen {
    using type = prepend_t<
            v,
            typename gen<count - 1, v>::type
        >;
};

template<auto v>
struct gen<0, v> {
    using type = ValueList<v>;
};

template<std::uint64_t count, auto v>
using gen_t = typename gen<count, v>::type;

/**
 * max
 */
template<typename valuelist, template<auto, auto> typename max_func = cexpr::math::max>
struct max;

template<template<auto, auto> typename max_func, auto max, typename valuelist>
struct max_aux;

template<template<auto, auto> typename max_func, auto max>
struct max_aux<max_func, max, ValueList<>> {
    constexpr static decltype(max) value = max;
};

template<
    template<auto, auto> typename max_func, auto max, auto head, auto...rest>
struct max_aux<max_func, max, ValueList<head, rest...>> {
    constexpr static decltype(max) value =
        max_aux<
            max_func, max_func<max, head>::value, ValueList<rest...>>::value;
};

template<
    typename valuelist,
    template<auto, auto> typename max_func>
struct max : max_aux<max_func, head<valuelist>::value, tail_t<valuelist>> {};

/**
 * build_freq_map
 *
 * Returns a ValueList<uint64_t>, freq, with size elements where:
 *      freq[i] = #occurrences of i in valuelist
 */
template<auto size, typename valuelist>
struct build_freq_map;

template<typename freq, typename valuelist>
struct build_freq_map_aux;

template<auto size, typename valuelist>
struct build_freq_map
    : build_freq_map_aux<gen_t<size, (decltype(size))0>, valuelist> {};

template<typename freq, auto val, auto... vals>
struct build_freq_map_aux<freq, ValueList<val, vals...>> {
    using type = typename build_freq_map_aux<
            valuelist::set_nth_t<
                val, valuelist::nth<val, freq>::value + 1, freq>,
            ValueList<vals...>
        >::type;
};

/**
 * build_freq_map_from_sorted
 */
template<auto size, typename sortedlist>
struct build_freq_map_from_sorted;

template<typename counts, std::int64_t count, auto curr, auto last, typename sortedlist, typename = void>
struct build_freq_map_from_sorted_aux;

template<auto size, typename sortedlist>
struct build_freq_map_from_sorted
    : build_freq_map_from_sorted_aux<ValueList<>, 0, 0, size, sortedlist> {};

template<typename counts, std::int64_t count, auto curr, auto last, auto head, auto... rest>
struct build_freq_map_from_sorted_aux<
    counts, count, curr, last, ValueList<head, rest...>, std::enable_if_t<head == curr>> {
        using type = typename build_freq_map_from_sorted_aux<
                counts,
                count + 1,
                curr,
                last,
                ValueList<rest...>
            >::type;
};

template<typename counts, std::int64_t count, auto curr, auto last, typename sortedlist>
struct build_freq_map_from_sorted_aux<
    counts, count, curr, last, sortedlist, std::enable_if_t<(curr > last)>> {
        using type = counts;
};

template<auto... counts, std::int64_t count, auto curr, auto last>
struct build_freq_map_from_sorted_aux<
    ValueList<counts...>, count, curr, last, ValueList<>, std::enable_if_t<(curr <= last)>> {
        using type = typename build_freq_map_from_sorted_aux<
            ValueList<counts..., count>,
            0,
            curr + 1,
            last,
            ValueList<>
        >::type;
};

template<auto... counts, std::int64_t count, auto curr, auto last, auto head, auto... rest>
struct build_freq_map_from_sorted_aux<
    ValueList<counts...>, count, curr, last, ValueList<head, rest...>, std::enable_if_t<curr != head>> {
        using type = typename build_freq_map_from_sorted_aux<
                ValueList<counts..., count>,
                0,
                curr + 1,
                last,
                ValueList<head, rest...>
            >::type;
};



template<typename freq>
struct build_freq_map_aux<freq, ValueList<>> {
    using type = freq;
};

template<auto size, typename valuelist>
using build_freq_map_t = typename build_freq_map<size, valuelist>::type;

}; // namespace valuelist
}; // namespace cexpr

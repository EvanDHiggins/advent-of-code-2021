#pragma once
#include <utility>

namespace cexpr {

/** The almighty list type. */
template<typename... T>
struct List;

// =====================================================
// Returns the first element of list.
template<typename list>
struct Head;

template<typename x, typename... xs>
struct Head<List<x, xs...>> {
    using type = x;
};

// =====================================================
// Returns the nth element of list.
template<int n, typename list>
struct Nth;

template<int n, typename x, typename... xs>
struct Nth<n, List<x, xs...>> {
    using type = typename Nth <n - 1, List<xs...>>::type;
};

template<typename x, typename... xs>
struct Nth<0, List<x, xs...>> {
    using type = x;
};

namespace list {

/**
 * head
 */
template<typename lst>
struct head;

template<typename x, typename... xs>
struct head<List<x, xs...>> {
    using type = x;
};

template<typename lst>
using head_t = typename head<lst>::type;


/**
 * nth
 */
template<int n, typename list>
struct nth;

template<int n, typename x, typename... xs>
struct nth<n, List<x, xs...>> {
    using type = typename nth <n - 1, List<xs...>>::type;
};

template<typename x, typename... xs>
struct nth<0, List<x, xs...>> {
    using type = x;
};

template<int n, typename lst>
using nth_t = typename nth<n, lst>::type;

/**
 * append
 * 
 * Appends val to list.
 */
template<typename val, typename list>
struct append;

template<typename val, typename... items>
struct append<val, List<items...>> {
    using type = List<items..., val>;
};

template<typename val>
struct append<val, List<>> {
    using type = List<val>;
};

/**
 * prepend
 *
 * Prepends val to list.
 */
template<typename val, typename list>
struct prepend;

template<typename val, typename... items>
struct prepend<val, List<items...>> {
    using type = List<val, items...>;
};

template<typename val>
struct prepend<val, List<>> {
    using type = List<val>;
};

template<typename val, typename list>
using prepend_t = typename prepend<val, list>::type;

template<typename lst>
struct length;

template<typename... elems>
struct length<List<elems...>> {
    constexpr static int value = sizeof...(elems);
};

template<typename val, typename list>
using append_t = typename append<val, list>::type;

/**
 * set_nth
 */
template<int n, typename val, typename lst>
struct set_nth;

template<int n, typename val, typename x, typename... xs>
struct set_nth<n, val, List<x, xs...>> {
    using type = prepend_t<
        x,
        typename set_nth<n-1, val, List<xs...>>::type>;
};

template<typename val, typename x, typename... xs>
struct set_nth<0, val, List<x, xs...>> {
    using type = List<val, xs...>;
};

template<int n, typename val, typename lst>
using set_nth_t = typename set_nth<n, val, lst>::type;

/**
 * fmap
 */
template<template<typename> typename func, typename list>
struct fmap;

template<template<typename> typename func, typename elem, typename... elems>
struct fmap<func, List<elem, elems...>> {
    using type = prepend_t<
        typename func<elem>::type,
        typename fmap<func, List<elems...>>::type>;
};

template<template<typename> typename func>
struct fmap<func, List<>> {
    using type = List<>;
};

template<template<typename> typename func, typename lst>
using fmap_t = typename fmap<func, lst>::type;

/**
 * filter
 */
template<template<typename> typename func, typename list, typename = void>
struct filter;

template<template<typename> typename func>
struct filter<func, List<>> {
    using type = List<>;
};

template<template<typename> typename func, typename first, typename... rest>
struct filter<
    func, List<first, rest...>, std::enable_if_t<func<first>::value>>
{
    using type = prepend_t<
            first,
            typename filter<func, List<rest...>>::type
        >;
};

template<template<typename> typename func, typename first, typename... rest>
struct filter<
    func, List<first, rest...>, std::enable_if_t<!func<first>::value>>
  : filter<func, List<rest...>>
{};

template<template<typename> typename func, typename list>
using filter_t = typename filter<func, list>::type;

/**
 * take
 */
template<int n, typename lst, typename = void>
struct take;

template<int n, typename first, typename... rest>
struct take<
    n, List<first, rest...>,
    std::enable_if_t<n != 0>> {
    using type = prepend_t<
            first,
            typename take<n - 1, List<rest...>>::type
        >;
};

template<typename lst>
struct take<0, lst> {
    using type = List<>;
};

template<int n>
struct take<n, List<>, std::enable_if<n != 0>> {
    using type = List<>;
};

template<int n, typename lst>
using take_t = typename take<n, lst>::type;


/**
 * drop
 *
 * Returns a list of the first n elements of lst.
 */
template<int n, typename lst, typename = void>
struct drop;

template<int n, typename elem, typename... elems>
struct drop<
    n, List<elem, elems...>,
    std::enable_if_t<n != 0>> {
    using type = typename drop<n - 1, List<elems...>>::type;
};

template<int n>
struct drop<n, List<>, std::enable_if_t<n != 0>> {
    using type = List<>;
};

template<typename lst>
struct drop<0, lst> {
    using type = lst;
};

template<int n, typename lst>
using drop_t = typename drop<n, lst>::type;

/**
 * lfold
 *
 * lfold :: (a -> b -> a) -> a -> [b] -> a
 */
template<template<typename, typename> typename func, typename acc, typename lst>
struct lfold;

template<
    template<typename, typename> typename func, typename acc, typename lst>
struct lfold_aux;

template<template<typename, typename> typename func, typename acc, typename first, typename... rest>
struct lfold<func, acc, List<first, rest...>> {
    using type = typename lfold<func, typename func<acc, first>::type, List<rest...>>::type;
};

template<template<typename, typename> typename func, typename acc>
struct lfold<func, acc, List<>> {
    using type = acc;
};

template<template<typename, typename> typename func, typename acc, typename lst>
using lfold_t = typename lfold<func, acc, lst>::type;

template<typename lhs, typename rhs>
struct logical_and : std::integral_constant<bool, lhs::value && rhs::value> {};

template<typename lhs, typename rhs>
struct logical_or : std::integral_constant<bool, lhs::value || rhs::value> {};

/**
 * all
 *
 * Defines value, a bool indicating whether, for each element of lst,
 * func<element>::value == true.
 */
template<template<typename> typename func, typename lst>
struct all {
    using type = lfold_t<
        logical_and, std::integral_constant<bool, true>, fmap_t<func, lst>>;
};

template<template<typename> typename func, typename lst>
using all_t = typename all<func, lst>::type;

/**
 * all
 *
 * Defines value, a bool indicating whether, for each element of lst,
 * func<element>::value == true.
 */
template<template<typename> typename func, typename lst>
struct any {
    using type = lfold_t<
        logical_or, std::integral_constant<bool, false>, fmap_t<func, lst>>;
};

template<template<typename> typename func, typename lst>
using any_t = typename any<func, lst>::type;


}; // namespace list

}; // cexpr

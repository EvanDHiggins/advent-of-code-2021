#pragma once

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

}; // namespace list

}; // cexpr

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

/**
 * filter
 */
template<template<typename> typename func, typename list, typename = void>
struct filter;

template<template<typename> typename func>
struct filter<func, List<>> {
    using type = List<>;
};

template<template<typename> typename func, typename head, typename... rest>
struct filter<
    func, List<head, rest...>, std::enable_if_t<func<head>::value>>
{
    using type = prepend_t<
            head,
            typename filter<func, List<rest...>>::type
        >;
};

template<template<typename> typename func, typename head, typename... rest>
struct filter<
    func, List<head, rest...>, std::enable_if_t<!func<head>::value>>
  : filter<func, List<rest...>>
{};

/**
 * take
 */
template<int n, typename lst, typename = void>
struct take;

template<int n, typename head, typename... rest>
struct take<
    n, List<head, rest...>,
    std::enable_if_t<n != 0>> {
    using type = prepend_t<
            head,
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


}; // namespace list

}; // cexpr

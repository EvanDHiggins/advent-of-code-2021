#pragma once
#include <utility>
#include "lib/cexpr/string.h"
#include "lib/cexpr/list.h"
#include "lib/cexpr/math.h"
#include "lib/cexpr/array.h"


// Assumes c represents an integer ascii character.
constexpr int char_to_int(char c) {
    return c - '0';
}

template<int x, int y>
struct sum {
    constexpr static int value = x + y;
};


template<signed int... ints>
struct IntList;

template<int val, typename intlist>
struct prepend;

template<int val, int... items>
struct prepend<val, IntList<items...>> {
    using type = IntList<val, items...>;
};

template<int val>
struct prepend<val, cexpr::List<>> {
    using type = IntList<val>;
};

template<int val, typename intlist>
using prepend_t = typename prepend<val, intlist>::type;


template<template<int> typename func, typename intlist>
struct ifmap;

template<template<int> typename func>
struct ifmap<func, IntList<>> {
    using type = IntList<>;
};

template<template<int> typename func, int head, int... rest>
struct ifmap<func, IntList<head, rest...>> {
    using type = prepend_t<
        func<head>::value,
        typename ifmap<func, IntList<rest...>>::type
    >;
};

template<template<int, int> typename func, typename lhs, typename rhs>
struct zip_int_lists;

template<
    template<int, int> typename func,
    int lhead, int... lrest,
    int rhead, int... rrest>
struct zip_int_lists<
    func, IntList<lhead, lrest...>, IntList<rhead, rrest...>> {
    using type = prepend_t<
        func<lhead, rhead>::value,
        typename zip_int_lists<func, IntList<lrest...>, IntList<rrest...>>::type
    >;
};
template<template<int, int> typename func>
struct zip_int_lists<func, IntList<>, IntList<>> {
    using type = IntList<>;
};

template<int i>
struct negate_zero {
    constexpr static int value = i;
};

template<>
struct negate_zero<0> {
    constexpr static int value = -1;
};

template<typename intlist>
using negate_zeroes = ifmap<negate_zero, intlist>;

template<int i>
struct zero {
    constexpr static int value = 0;
};

template<typename str>
struct str_to_intlist;

template<char... cs>
struct str_to_intlist<cexpr::String<cs...>> {
    using type = IntList<char_to_int(cs)...>;
};

template<typename list>
struct fold_int_lists;

template<typename acc, typename list>
struct fold_int_lists_aux;

template<typename acc, typename head, typename... rest>
struct fold_int_lists_aux<acc, cexpr::List<head, rest...>>
    : fold_int_lists_aux<
        typename zip_int_lists<sum, acc, head>::type, cexpr::List<rest...>>
{};

template<typename acc>
struct fold_int_lists_aux<acc, cexpr::List<>>
{
    using type = acc;
};

template<typename head, typename... rest>
struct fold_int_lists<cexpr::List<head, rest...>>
    : fold_int_lists_aux<
        typename ifmap<zero, head>::type, cexpr::List<head, rest...>>
{};

template<int i, typename = void>
struct compute_gamma_aux;

template<int i>
struct compute_gamma_aux<i, std::enable_if_t<(i > 0)>> {
    constexpr static int value = 1;
};

template<int i>
struct compute_gamma_aux<i, std::enable_if_t<(i < 0)>> {
    constexpr static int value = 0;
};

template<int i>
struct compute_gamma : compute_gamma_aux<i> {};

template<int i, typename = void>
struct compute_epsilon_aux;

template<int i>
struct compute_epsilon_aux<i, std::enable_if_t<(i < 0)>> {
    constexpr static int value = 1;
};

template<int i>
struct compute_epsilon_aux<i, std::enable_if_t<(i > 0)>> {
    constexpr static int value = 0;
};

template<int i>
struct compute_epsilon : compute_epsilon_aux<i> {};


template<int radix, typename intlist>
struct to_int;

template<int radix, int i, int... is>
struct to_int<radix, IntList<i, is...>> {
    constexpr static int value =
        (i * cexpr::math::pow<radix, sizeof...(is)>::value)
        + to_int<radix, IntList<is...>>::value;
};

template<int radix, int i>
struct to_int<radix, IntList<i>> {
    constexpr static int value = i;
};


template<int x, int y, typename = void>
struct comp;

template<int x, int y>
struct comp<x, y, std::enable_if_t<x == y>> {
    using type = std::true_type;
};

template<int x, int y>
struct comp<x, y, std::enable_if_t<x != y>> {};



template<int size, const char (&arr)[size]>
struct solution {
    using lines = typename cexpr::array::readlines<size, arr>::type;

    using intlists = typename cexpr::list::fmap<
        negate_zeroes,
        typename cexpr::list::fmap<str_to_intlist, lines>::type
    >::type;

    using counts = typename fold_int_lists<intlists>::type;
    using gamma_bits = typename ifmap<compute_gamma, counts>::type;
    using epsilon_bits = typename ifmap<compute_epsilon, counts>::type;
    constexpr static int gamma =
        to_int<2, gamma_bits>::value;
    constexpr static int epsilon =
        to_int<2, epsilon_bits>::value;

    constexpr static int part_one_answer = epsilon * gamma;
    constexpr static int part_two_answer = 0;
};












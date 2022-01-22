#pragma once

#include "lib/cexpr/math.h"

namespace cexpr {

template<char... cs>
struct String;

/**
 * reverse_string
 *
 * Reversed the characters in str.
 */
template<typename str>
struct reverse_string;

template<typename str1, typename str2>
struct reverse_string_helper;

template<char... reversed, char c, char... cs>
struct reverse_string_helper<
    String<reversed...>,
    String<c, cs...>>
    : reverse_string_helper<String<c, reversed...>, String<cs...>>
{};

template<typename reversed>
struct reverse_string_helper<reversed, String<>> {
    using type = reversed;
};

template<typename str>
struct reverse_string {
    using type = typename reverse_string_helper<String<>, str>::type;
};



/**
 * to_int
 *
 * Converts string to integer.
 */
template<typename str>
struct to_int;

template<char c, char... cs>
struct to_int<String<c, cs...>> {
    constexpr static int value =
        (((int)(c - '0')) * math::pow<10, (sizeof...(cs))>::value)
        + to_int<String<cs...>>::value;
};

template<char c>
struct to_int<String<c>> {
    constexpr static int value = c - '0';
};



/**
 * append
 *
 * Appends c to str.
 */
template<char c, typename str>
struct append;

template<char to_append, char... cs>
struct append<to_append, String<cs...>> {
    using type = String<cs..., to_append>;
};

namespace str {

template<typename str>
struct is_empty {
    constexpr static bool value = false;
};

template<>
struct is_empty<String<>> {
    constexpr static bool value = true;
};

/**
 * concat
 */
template<typename str1, typename... strs>
struct concat;

template<char... str1_chars, char... str2_chars, typename... rest>
struct concat<String<str1_chars...>, String<str2_chars...>, rest...> {
    using type =
        typename concat<String<str1_chars..., str2_chars...>, rest...>::type;
};

template<char... str1_chars, char... str2_chars>
struct concat<String<str1_chars...>, String<str2_chars...>> {
    using type = String<str1_chars..., str2_chars...>;
};

}; // namespace str

}; // namespace cexpr

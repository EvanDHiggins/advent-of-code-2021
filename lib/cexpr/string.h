#pragma once

namespace cexpr {

template<char... cs>
struct String;

// =====================================================
// Reversed the characters within str.
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

// =====================================================
// Appends c to str.
template<char c, typename str>
struct append_to_string;

template<char to_append, char... cs>
struct append_to_string<to_append, String<cs...>> {
    using type = String<cs..., to_append>;
};

}; // namespace cexpr

#pragma once

#include "lib/cexpr/string.h"
#include "lib/cexpr/list.h"
#include "lib/cexpr/array.h"
#include "2/input.h"


using cexpr::String;
using cexpr::List;



template<
    typename str,
    int idx,
    int size,
    const char (&arr)[size],
    typename = void>
struct readline_helper;

template<
    char... chars, 
    int idx, 
    int size, 
    const char (&arr)[size]
>
struct readline_helper<
    String<chars...>,
    idx,
    size,
    arr,
    typename std::enable_if<arr[idx] != '\n' && arr[idx] != '\0'>::type
>  : readline_helper<
            String<chars..., arr[idx]>,
            idx + 1,
            size,
            arr
        >
{};

template<
    typename str,
    int idx, 
    int size, 
    const char (&arr)[size]
>
struct readline_helper<
    str, idx, size, arr,
    typename std::enable_if<arr[idx] == '\n' || arr[idx] == '\0'>::type
> {
    static constexpr int next_index = idx + 1;
    using type = str;
};

template<int start, int size, const char (&arr)[size]>
struct readline : readline_helper<String<>, start, size, arr> {};

/**
 * readlines
 */
template<int size, const char (&arr)[size]>
struct readlines;

template<
    typename lines,
    int idx,
    int size,
    const char (&arr)[size],
    typename = void
>
struct readlines_helper;

template<
  typename... lines,
  int idx,
  int size,
  const char (&arr)[size]
>
struct readlines_helper<
    List<lines...>, 
    idx,
    size,
    arr,
    typename std::enable_if<arr[idx] != '\0'>::type
> {
    using line = readline<idx, size, arr>;
    using type = typename readlines_helper<
            List<lines..., typename line::type>,
            line::next_index,
            size,
            arr
        >::type;
};

template<typename list, int idx, int size, const char (&arr)[size]>
struct readlines_helper<
    list, idx, size, arr,
    typename std::enable_if<idx >= size>::type> {
        using type = list;
};

template<int size, const char (&arr)[size]>
struct readlines : readlines_helper<List<>, 0, size, arr> {};

// ======================================================

constexpr bool is_whitespace(char c) {
    return c == '\n' || c == ' ' || c == '\t';
}

template<typename tokens, typename str_buffer, typename rest, typename = void>
struct split_helper;

// Guards against putting lots of empty strings into our tokens list.
template<typename tokens, char c, char... rest>
struct split_helper<
    tokens, String<>, String<c, rest...>,
    typename std::enable_if<is_whitespace(c)>::type
>
    : split_helper<tokens, String<>, String<rest...>> {};

template<typename tokens, char c>
struct split_helper<
    tokens, String<>, String<c>,
    typename std::enable_if<is_whitespace(c)>::type
>
    : split_helper<tokens, String<>, String<>> {};

template<typename tokens, typename str, char c, char... rest>
struct split_helper<
    tokens, str, String<c, rest...>,
    typename std::enable_if<is_whitespace(c)>::type
> 
 : split_helper<cexpr::list::append<str, tokens>, String<>, String<rest...>>
{};

template<typename tokens, typename str, char c>
struct split_helper<
    tokens, str, String<c>,
    typename std::enable_if<is_whitespace(c)>::type
> 
    : split_helper<cexpr::list::append<str, tokens>, String<>, String<>>
{};

template<typename tokens>
struct split_helper<tokens, String<>, String<>>
    : tokens {};

template<typename tokens, typename str_buffer, char c, char... rest>
struct split_helper<
    tokens, str_buffer, String<c, rest...>,
    typename std::enable_if<!is_whitespace(c)>::type
> : split_helper<
    tokens, typename cexpr::append<c, str_buffer>::type, String<rest...>>
{};

template<typename str>
struct split : split_helper<List<>, String<>, str> {};

template<typename dir, int magnitude>
struct Direction;


using parsed_input =
    readlines<cexpr::array::length(PROGRAM_INPUT), PROGRAM_INPUT>::type;

constexpr static int part_one_answer = 0;
constexpr static int part_two_answer = 0;

using FORWARD_STR = String<'f', 'o', 'r', 'w', 'a', 'r', 'd'>;
using BACKWARD_STR = String<'b', 'a', 'c', 'k', 'w', 'a', 'r', 'd'>;
using DOWN_STR = String<'d', 'o', 'w', 'n'>;
using UP_STR = String<'u', 'p'>;
using SPACE = String<' '>;

constexpr static char test_data[] =
R"(forward 1
backward 1
forward 2
backward 1
down 10
up 5)"; // answer is 5

using cexpr::array::length;

// ========== Test readline
using first_readline = readline<0, length(test_data), test_data>;
static_assert(
        std::is_same<
            first_readline::type,
            cexpr::str::concat<FORWARD_STR, SPACE, String<'1'>>::type
        >::value, "");
static_assert(first_readline::next_index == 10, "");
using second_readline = readline<
    first_readline::next_index,
    length(test_data),
    test_data>;
static_assert(
        std::is_same<
            second_readline::type,
            cexpr::str::concat<BACKWARD_STR, SPACE, String<'1'>>::type
        >::value, "");

using last_readline = readline<50, length(test_data), test_data>;
static_assert(
        std::is_same<
            last_readline::type,
            cexpr::str::concat<UP_STR, SPACE, String<'5'>>::type
        >::value, "");
// ========== End test readline

constexpr int test() {
    using cexpr::str::concat;
    using cexpr::String;
    static_assert(
        std::is_same<
            readlines<cexpr::array::length(test_data), test_data>::type,
            List<
                concat<FORWARD_STR, SPACE, String<'1'>>::type,
                concat<BACKWARD_STR, SPACE, String<'1'>>::type,
                concat<FORWARD_STR, SPACE, String<'2'>>::type,
                concat<BACKWARD_STR, SPACE, String<'1'>>::type,
                concat<DOWN_STR, SPACE, String<'1', '0'>>::type,
                concat<UP_STR, SPACE, String<'5'>>::type>
                >::value,
                "");
    return 0;
}

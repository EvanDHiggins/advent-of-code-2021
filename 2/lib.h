#pragma once

#include "lib/cexpr/string.h"
#include "lib/cexpr/list.h"
#include "lib/cexpr/array.h"
#include "2/input.h"


using cexpr::String;
using cexpr::List;

using FORWARD = String<'f', 'o', 'r', 'w', 'a', 'r', 'd'>;
using BACKWARD = String<'b', 'a', 'c', 'k', 'w', 'a', 'r', 'd'>;
using DOWN = String<'d', 'o', 'w', 'n'>;
using UP = String<'u', 'p'>;
using SPACE = String<' '>;


// ======================================================

constexpr bool is_whitespace(char c) {
    return c == '\n' || c == ' ' || c == '\t';
}

template<typename tokens, typename str_buffer, typename rest, typename = void>
struct split_helper;

// Guards against putting lots of empty strings into our tokens list.
template<typename tokens, char c, char... rest>
struct split_helper<
    tokens, String<>, String<c, rest...>, std::enable_if_t<is_whitespace(c)>>
  : split_helper<tokens, String<>, String<rest...>>
{};

template<typename tokens, char c>
struct split_helper<
    tokens, String<>, String<c>, std::enable_if_t<is_whitespace(c)>>
  : split_helper<tokens, String<>, String<>>
{};

template<typename tokens, char c>
struct split_helper<
    tokens, String<>, String<c>,
    std::enable_if_t<!is_whitespace(c)>>
  : split_helper<
        cexpr::list::append_t<String<c>, tokens>,
        String<>, String<>>
{};

template<typename tokens, typename str>
struct split_helper<
    tokens, str, String<>,
    std::enable_if_t<!cexpr::str::is_empty<str>::value>>
  : split_helper<
        cexpr::list::append_t<str, tokens>,
        String<>, String<>>
{};

template<typename tokens, typename str, char c, char... rest>
struct split_helper<
    tokens, str, String<c, rest...>, std::enable_if_t<is_whitespace(c)>> 
 : split_helper<
    cexpr::list::append_t<str, tokens>,
    String<>, String<rest...>>
{};

template<typename tokens, typename str, char c>
struct split_helper<
    tokens, str, String<c>, std::enable_if_t<is_whitespace(c)>> 
  : split_helper<
        cexpr::list::append_t<str, tokens>, String<>, String<>>
{};

template<typename tokens>
struct split_helper<tokens, String<>, String<>> {
    using type = tokens;
};

template<typename tokens, typename str_buffer, char c, char... rest>
struct split_helper<
    tokens, str_buffer, String<c, rest...>,
    std::enable_if_t<!is_whitespace(c)>>
  : split_helper<
        tokens, cexpr::str::append_t<c, str_buffer>, String<rest...>>
{};

template<typename str>
struct split : split_helper<List<>, String<>, str> {};

template<typename dir, int magnitude>
struct Direction;

template<typename... tokens>
struct parse_movement;

template<typename dir, typename mag>
struct parse_movement<List<dir, mag>> {
    using type = Direction<dir, cexpr::to_int<mag>::value>;
};

template<typename line>
struct parse_line : parse_movement<typename split<line>::type> {};



template<typename dirs, typename list>
struct parse_directions_helper;

template<typename dirs, typename line, typename... rest>
struct parse_directions_helper<dirs, List<line, rest...>>
 : parse_directions_helper<
    cexpr::list::append_t<typename parse_line<line>::type, dirs>,
    List<rest...>>
{};

template<typename acc, typename line>
struct parse_directions_helper<acc, List<line>>
{
    using type =
        cexpr::list::append_t<typename parse_line<line>::type, acc>;
};

template<typename lines>
struct parse_directions : parse_directions_helper<List<>, lines> {};

template<int pos, int depth, typename... dirs>
struct solution_helper;

template<int pos, int depth, int delta, typename... dirs>
struct solution_helper<pos, depth, Direction<FORWARD, delta>, dirs...> 
  : solution_helper<pos + delta, depth, dirs...> {};

template<int pos, int depth, int delta, typename... dirs>
struct solution_helper<pos, depth, Direction<BACKWARD, delta>, dirs...> 
  : solution_helper<pos - delta, depth, dirs...> {};

template<int pos, int depth, int delta, typename... dirs>
struct solution_helper<pos, depth, Direction<UP, delta>, dirs...> 
  : solution_helper<pos, depth - delta, dirs...> {};

template<int pos, int depth, int delta, typename... dirs>
struct solution_helper<pos, depth, Direction<DOWN, delta>, dirs...> 
  : solution_helper<pos, depth + delta, dirs...> {};

template<int pos, int depth>
struct solution_helper<pos, depth> {
    constexpr static int value = pos * depth;
};

template<typename dir_list>
struct solution;

template<typename... dirs>
struct solution<List<dirs...>> : solution_helper<0, 0, dirs...> {};



template<typename dir_list>
struct solution_part_two;

template<int pos, int depth, int aim, typename... dirs>
struct sol_part_two_helper;

template<int pos, int depth, int aim, int delta, typename... dirs>
struct sol_part_two_helper<pos, depth, aim, Direction<FORWARD, delta>, dirs...>
    : sol_part_two_helper<pos + delta, depth + (aim * delta), aim, dirs...>
{};

template<int pos, int depth, int aim, int delta, typename... dirs>
struct sol_part_two_helper<pos, depth, aim, Direction<BACKWARD, delta>, dirs...>
    : sol_part_two_helper<pos - delta, depth, aim, dirs...>
{};

template<int pos, int depth, int aim, int delta, typename... dirs>
struct sol_part_two_helper<pos, depth, aim, Direction<UP, delta>, dirs...>
    : sol_part_two_helper<pos, depth, aim - delta, dirs...>
{};

template<int pos, int depth, int aim, int delta, typename... dirs>
struct sol_part_two_helper<pos, depth, aim, Direction<DOWN, delta>, dirs...>
    : sol_part_two_helper<pos, depth, aim + delta, dirs...>
{};

template<int _pos, int _depth, int _aim>
struct sol_part_two_helper<_pos, _depth, _aim> {
    constexpr static int depth = _depth;
    constexpr static int pos = _pos;
    constexpr static int aim = _aim;
    constexpr static int value = pos * depth;
};

template<typename... dirs>
struct solution_part_two<List<dirs...>> : sol_part_two_helper<0, 0, 0, dirs...>
{};

using lines =
    cexpr::array::readlines<
        cexpr::array::length(PROGRAM_INPUT), PROGRAM_INPUT>::type;
using directions =
    parse_directions<lines>::type;

constexpr static int part_one_answer = solution<directions>::value;
constexpr static int part_two_answer = solution_part_two<directions>::value;

constexpr static char test_data[] =
R"(forward 1
backward 1
forward 2
backward 1
down 10
up 5)"; // answer is 5

using cexpr::array::length;

// ========== Test readline
using first_readline = cexpr::array::readline<0, length(test_data), test_data>;
static_assert(
        std::is_same<
            first_readline::type,
            cexpr::str::concat<FORWARD, SPACE, String<'1'>>::type
        >::value, "");
static_assert(first_readline::next_index == 10, "");
using second_readline = cexpr::array::readline<
    first_readline::next_index,
    length(test_data),
    test_data>;
static_assert(
        std::is_same<
            second_readline::type,
            cexpr::str::concat<BACKWARD, SPACE, String<'1'>>::type
        >::value, "");

using last_readline = cexpr::array::readline<50, length(test_data), test_data>;
static_assert(
        std::is_same<
            last_readline::type,
            cexpr::str::concat<UP, SPACE, String<'5'>>::type
        >::value, "");
// ========== End test readline

constexpr int test() {
    using cexpr::str::concat;
    using cexpr::String;
    static_assert(
        std::is_same<
            cexpr::array::readlines_t<cexpr::array::length(test_data), test_data>,
            List<
                concat<FORWARD, SPACE, String<'1'>>::type,
                concat<BACKWARD, SPACE, String<'1'>>::type,
                concat<FORWARD, SPACE, String<'2'>>::type,
                concat<BACKWARD, SPACE, String<'1'>>::type,
                concat<DOWN, SPACE, String<'1', '0'>>::type,
                concat<UP, SPACE, String<'5'>>::type>
                >::value,
                "");
    using split_test_1 = String<
        ' ', ' ', 'f', 'o', 'o', ' ', ' ', 'b', ' ', 'a', ' ', ' '>;
    using parsed_directions = parse_directions<
        cexpr::array::readlines_t<
            cexpr::array::length(test_data), test_data>>::type;
    static_assert(
            std::is_same<
                split<split_test_1>::type,
                List<String<'f','o','o'>, String<'b'>, String<'a'>>
            >::value, "");
    static_assert(
            std::is_same<
                parsed_directions,
                List<
                    Direction<FORWARD, 1>,
                    Direction<BACKWARD, 1>,
                    Direction<FORWARD, 2>,
                    Direction<BACKWARD, 1>,
                    Direction<DOWN, 10>,
                    Direction<UP, 5>
                >
            >::value,
            "");
    static_assert(solution<parsed_directions>::value == 5, "");
    return 0;
}

#pragma once

#include "lib/cexpr/string.h"
#include "lib/cexpr/list.h"
#include "lib/cexpr/array.h"


using cexpr::String;
using cexpr::List;

/**
 * readlines
 */
template<int size, const char (&arr)[size]>
struct readlines;

template<
    typename lines,
    typename curr_str,
    int idx,
    int size,
    const char (&arr)[size]
>
struct readlines_helper;
// TODO: Implement this function to read lines into Strings.

template<int size, const char (&arr)[size]>
struct readlines : readlines_helper<List<>, String<>, 0, size, arr> {};


constexpr static int part_one_answer = 0;
constexpr static int part_two_answer = 0;

using FORWARD_STR = String<'f', 'o', 'r', 'w', 'a', 'r', 'd'>;
using BACWARD_STR = String<'b', 'a', 'c', 'k', 'w', 'a', 'r', 'd'>;
using DOWN_STR = String<'d', 'o', 'w', 'n'>;
using UP_STR = String<'u', 'p'>;
using SPACE = String<' '>;

constexpr static char test_data[] =
R"(forward 1
backward 1
forward 2
backward 1
down 10
up 5
)"; // answer is 5

constexpr int test() {
    static_assert(
        std::is_same<
            readlines<cexpr::array::length(test_data), test_data>::type,
            List<
                str::concat<FORWARD_STR, SPACE, String<'1'>::type,
                str::concat<BACKWARD_STR, SPACE, String<'1'>::type,
                str::concat<FORWARD_STR, SPACE, String<'2'>::type,
                str::concat<BACKWARD_STR, SPACE, String<'1'>::type,
                str::concat<DOWN_STR, SPACE, String<'1', '0'>::type,
                str::concat<UP_STR, SPACE, String<'5'>>::type
                >::value,
                "");

    return 0;
}

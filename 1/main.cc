#include <iostream>
#include <utility>
#include "1/__generated_input.h"
#include "lib/cexpr/list.h"
#include "lib/cexpr/array.h"
#include "lib/cexpr/primitives.h"
#include "lib/cexpr/string.h"
#include "lib/cexpr/math.h"

using namespace cexpr;

template<char curr, int curr_idx, int size, const char (&arr)[size]>
struct first_non_whitespace_impl;

template<int curr_idx, int size, const char (&arr)[size]>
struct first_non_whitespace_impl<'\n', curr_idx, size, arr> {
    static constexpr int value =
        first_non_whitespace_impl<
            arr[curr_idx + 1], curr_idx + 1, size, arr>::value;
};

template<char curr, int curr_idx, int size, const char (&arr)[size]>
struct first_non_whitespace_impl {
    static constexpr int value = curr_idx;
};

template<int size, const char (&arr)[size]>
struct first_non_whitespace {
    static constexpr int value =
        first_non_whitespace_impl<arr[0], 0, size, arr>::value;
};



constexpr static char test_data_1[] = "012345678";

int main() {
    static_assert(
            std::is_same<
                append_to_string<'3', String<'1', '2'>>::type,
                String<'1','2','3'>
            >::value,
            "append_to_string is not working.");
    static_assert(
            std::is_same<
                    reverse_string<String<'1', '2', '3'>>::type,
                    String<'3','2','1'>
                >::value,
            "They aren't the same!");
    static_assert(
            std::is_same<
                array::array_substring<
                    2,
                    5,
                    array::length(test_data_1), test_data_1
                >::type,
                String<'2', '3', '4'>
            >::value,
            "array_substring is broken!");
    static_assert(
            math::pow<10, 2>::value == 100,
            "pow is broken!");
    static_assert(
            math::pow<4, 2>::value == 16,
            "pow is broken!");
    static_assert(
            to_int<String<'1', '2', '3'>>::value == 123,
            "to_int is broken!"
        );
    static_assert(
            to_int<String<'1'>>::value == 1,
            "to_int is broken!"
        );
    static_assert(
            to_int<String<'4', '8', '1', '0', '7'>>::value == 48107,
            "to_int is broken!"
        );
}

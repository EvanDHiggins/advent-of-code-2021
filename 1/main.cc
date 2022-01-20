#include <iostream>
#include <utility>
#include "1/__generated_input.h"
#include "lib/cexpr/list.h"
#include "lib/cexpr/array.h"
#include "lib/cexpr/primitives.h"
#include "lib/cexpr/string.h"

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


constexpr static char some_data[] = "012345678";

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
                array_substring<2, 5, array::length(some_data), some_data>::type,
                String<'2', '3', '4'>
            >::value,
            "array_substring is broken!");
}

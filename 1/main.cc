#include <iostream>
#include <utility>
#include "1/__generated_input.h"
#include "lib/cexpr/list.h"
#include "lib/cexpr/array.h"
#include "lib/cexpr/primitives.h"

using namespace cexpr;

template<char... cs>
struct String;

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


int main() {
    std::cout
        << first_non_whitespace<
               array::length(PROGRAM_INPUT), PROGRAM_INPUT>::value
        << std::endl;
    //using value =
        //array::to_typed_char_list<
            //array::length(PROGRAM_INPUT), PROGRAM_INPUT>::type;
    //std::cout << Nth<2, value>::type::as_char << std::endl;
}

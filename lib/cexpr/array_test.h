#pragma once

#include "lib/cexpr/array.h"
#include "lib/cexpr/string.h"

constexpr static char readline_test_data[] = R"(

hello

world

blah

)";
constexpr static char readline_test_data_2[] =
R"(hello

world

blah

)";
constexpr static char readline_test_data_3[] =
R"(

hello
world

blah)";
constexpr static char readline_test_data_4[] =
R"(hello

world

blah)";

using expected = cexpr::List<
    cexpr::String<'h', 'e', 'l', 'l', 'o'>,
    cexpr::String<'w', 'o', 'r', 'l', 'd'>,
    cexpr::String<'b', 'l', 'a', 'h'>
>;

namespace {
    using cexpr::array::length;

    static_assert(
        cexpr::array::first_from_right<
            length(readline_test_data), readline_test_data>::value
        == 19);
    static_assert(
        cexpr::array::first_from_right<
            length(readline_test_data_4), readline_test_data_4>::value
        == 17);

    static_assert(
            cexpr::array::first_from_left<
                length(readline_test_data), readline_test_data>::value
            == 2
            );

        

    static_assert(
        std::is_same<
            cexpr::array::readlines_t<
                length(readline_test_data), readline_test_data>,
            expected
        >::value);
    static_assert(
        std::is_same<
            cexpr::array::readlines_t<
                length(readline_test_data_2), readline_test_data_2>,
            expected
        >::value);
    static_assert(
        std::is_same<
            cexpr::array::readlines_t<
                length(readline_test_data_3), readline_test_data_3>,
            expected
        >::value);
    static_assert(
        std::is_same<
            cexpr::array::readlines_t<
                length(readline_test_data_4), readline_test_data_4>,
            expected
        >::value);
}

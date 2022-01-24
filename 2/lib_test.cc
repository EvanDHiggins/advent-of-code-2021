#include "2/lib.h"

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
            cexpr::valuelist::concat<FORWARD, SPACE, String<'1'>>::type
        >::value, "");
static_assert(first_readline::next_index == 10, "");
using second_readline = cexpr::array::readline<
    first_readline::next_index,
    length(test_data),
    test_data>;
static_assert(
        std::is_same<
            second_readline::type,
            cexpr::valuelist::concat<BACKWARD, SPACE, String<'1'>>::type
        >::value, "");

using last_readline = cexpr::array::readline<50, length(test_data), test_data>;
static_assert(
        std::is_same<
            last_readline::type,
            cexpr::valuelist::concat<UP, SPACE, String<'5'>>::type
        >::value, "");
// ========== End test readline


using cexpr::valuelist::concat;
using cexpr::valuelist::String;
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
            cexpr::valuelist::split<' ', split_test_1>::type,
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

int main() {}

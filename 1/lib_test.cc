#include "1/lib.h"
using cexpr::valuelist::to_int;
using cexpr::valuelist::ValueList;

// ================== Test cases =====================
constexpr static char test_ends_null[] = "\n\n123\n23";
constexpr static char test_data_1[] = "012345678";

using test_int_list = parse_file_to_int_list<
    array::length(test_ends_null), test_ends_null>::type;
static_assert(
        std::is_same<
            test_int_list,
            ValueList<123, 23>
        >::value,
        "");

using array::length;

static_assert(
        std::is_same<
            cexpr::valuelist::append_t<'3', String<'1', '2'>>,
            String<'1','2','3'>
        >::value,
        "append_to_string is not working.");
static_assert(
        std::is_same<
                cexpr::valuelist::reverse<String<'1', '2', '3'>>::type,
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
        to_int<10, String<'1', '2', '3'>>::value == 123,
        "to_int is broken!"
    );
static_assert(
        to_int<10, String<'1'>>::value == 1,
        "to_int is broken!"
    );
static_assert(
        to_int<10, String<'4', '8', '1', '0', '7'>>::value == 48107,
        "to_int is broken!"
    );

int main() {}

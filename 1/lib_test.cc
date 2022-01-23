#include "1/lib.h"

// ================== Test cases =====================
constexpr static char test_ends_null[] = "\n\n123\n23";
constexpr static char test_data_1[] = "012345678";

template<int start, int size, const char (&arr)[size]>
struct test_read_int_helper :
    read_next_int<start, size, arr> {};

constexpr static int from_beginning =
    test_read_int_helper<
        0, array::length(test_ends_null), test_ends_null>::value;

constexpr static int from_one =
    test_read_int_helper<
        1, array::length(test_ends_null), test_ends_null>::value;

constexpr static int from_two =
    test_read_int_helper<
        1, array::length(test_ends_null), test_ends_null>::value;

constexpr static int from_five_null =
    test_read_int_helper<
        5, array::length(test_ends_null), test_ends_null>::value;

static_assert(from_beginning == 123, "");
static_assert(from_one == 123, "");
static_assert(from_two == 123, "");
static_assert(from_five_null == 23, "");

using test_int_list = parse_file_to_int_list<
    array::length(test_ends_null), test_ends_null>::type;
static_assert(
        std::is_same<
            test_int_list,
            IntList<123, 23>
        >::value,
        "");

using array::length;

static_assert(
        std::is_same<
            cexpr::str::append_t<'3', String<'1', '2'>>,
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

int main() {}

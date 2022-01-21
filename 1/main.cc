#include <iostream>
#include <utility>
#include "1/__generated_input.h"
#include "lib/cexpr/list.h"
#include "lib/cexpr/array.h"
#include "lib/cexpr/primitives.h"
#include "lib/cexpr/string.h"
#include "lib/cexpr/math.h"

using namespace cexpr;



/**
 * read_next_int
 * read_next_int_lstrip
 * read_next_int_helper
 *
 * Given an array, its size, and a starting index, read from 'start',
 * ignoring newlines, until you hit a char. Then read in all subsequent chars
 * and convert them to an integer.
 *
 * Preconditions:
 *      Assumes that at least one non-'\n', non-'\0' character exists to the
 *      right of arr[start].
 *
 * Returns:
 *      ::value = The parsed char
 *      ::last_seen_idx = The last index which _was not_ included in value.
 *              Subsequent calls to this function should be provided this
 *              value as their starting index to continue reading chars.
 */
template<int start, int size, const char (&arr)[size]>
struct read_next_int;

template<int idx, int size, const char (&arr)[size], typename = void>
struct read_next_int_lstrip;

template<typename str, int idx, int size, const char (&arr)[size], typename = void>
struct read_next_int_helper :
    read_next_int_helper<
        typename append_to_string<arr[idx], str>::type, idx+1, size, arr>
{};

template<typename str, int idx, int size, const char (&arr)[size]>
struct read_next_int_helper<
    str, idx, size, arr,
    typename std::enable_if<arr[idx] == '\n' || arr[idx] == '\0'>::type
> {
    constexpr static int value = to_int<str>::value;
    constexpr static int last_seen_idx = idx;
};

template<int idx, int size, const char (&arr)[size]>
struct read_next_int_lstrip<
    idx, size, arr, typename std::enable_if<arr[idx] == '\n'>::type> :
        read_next_int_lstrip<idx+1, size, arr>
{};


template<int idx, int size, const char (&arr)[size]>
struct read_next_int_lstrip<
    idx, size, arr, typename std::enable_if<arr[idx] != '\n'>::type>
: read_next_int_helper<String<>, idx, size, arr> {};


template<int start, int size, const char (&arr)[size]>
struct read_next_int : read_next_int_lstrip<start, size, arr> {};


/**
 * find_numeric_range_from
 * find_numeric_range_from_helper
 */
template<int start, int end, int size, const char (&arr)[size], typename = void>
struct find_numeric_range_from_helper;

template<int start, int end, int size, const char (&arr)[size]>
struct find_numeric_range_from_helper<start, end, size, arr,
    typename std::enable_if<arr[end] != '\n' && arr[end] != '\0'>::type>
    : find_numeric_range_from_helper<start, end+1, size, arr>
{};

template<int start, int end, int size, const char (&arr)[size]>
struct find_numeric_range_from_helper<start, end, size, arr,
    typename std::enable_if<arr[end] == '\n' || arr[end] == '\0'>::type>
{
    constexpr static int left = start;
    constexpr static int right = end;
};

template<int start, int size, const char (&arr)[size]>
struct find_numeric_range_from;

template<int start, int size, const char (&arr)[size]>
struct find_numeric_range_from
 : find_numeric_range_from_helper<start, start + 1, size, arr>
{};


template<int... ints>
struct IntList;

/**
 * parse_file_to_int_list
 * parse_file_to_int_list_helper
 *
 * Assumes arr is a char array of digits seperated by spaces, with no trailing
 * newline. Parses it as a list of integers.
 */
template<typename int_list, int idx, int size, const char (&arr)[size], typename = void>
struct parse_file_to_int_list_helper;

template<typename int_list, int idx, int size, const char (&arr)[size]>
struct parse_file_to_int_list_helper<
    int_list, idx, size, arr, typename std::enable_if<arr[idx] == '\0' || arr[idx+1] == '\0'>::type>
{
    using type = int_list;
};
template<int... ints, int idx, int size, const char (&arr)[size]>
struct parse_file_to_int_list_helper<
    IntList<ints...>, idx, size, arr, typename std::enable_if<arr[idx] != '\0' && arr[idx+1] != '\0'>::type>
{
    using next_int_t = read_next_int<idx, size, arr>;
    using type = typename parse_file_to_int_list_helper<
        IntList<ints..., next_int_t::value>,
        next_int_t::last_seen_idx,
        size,
        arr
    >::type;

};

template<int size, const char (&arr)[size]>
struct parse_file_to_int_list 
    : parse_file_to_int_list_helper<IntList<>, 0, size, arr>
{};



/**
 * count_increases
 * count_increases_aux
 *
 * Provided an IntList, returns the number of consecutive pairs of numbers are
 * increasing.
 */
template<typename int_list>
struct count_increases;

template<int count, typename int_list>
struct count_increases_aux;

template<int count, int first, int second, int... rest>
struct count_increases_aux<count, IntList<first, second, rest...>>
    : count_increases_aux<count + (int)(first < second), IntList<second, rest...>>
{};

template<int count, int first, int second>
struct count_increases_aux<count, IntList<first, second>>
    : count_increases_aux<count + (int)(first < second), IntList<>>
{};

template<int count>
struct count_increases_aux<count, IntList<>> {
    constexpr static int value = count;
};

template<typename int_list>
struct count_increases
 : count_increases_aux<0, int_list> {};


using int_list = parse_file_to_int_list<
    array::length(PROGRAM_INPUT), PROGRAM_INPUT
>::type;
constexpr static int answer = count_increases<int_list>::value;

int main() {
    std::cout << "The answer is " << answer << std::endl;
}

// ================== Test cases =====================
constexpr static char test_ends_null[] = "\n\n123\n23";
constexpr static char test_data_1[] = "012345678";

template<int start, int size, const char (&arr)[size]>
struct test_read_int_helper :
    read_next_int<start, size, arr> {};

constexpr static int from_beginning =
    test_read_int_helper<0, array::length(test_ends_null), test_ends_null>::value;

constexpr static int from_one =
    test_read_int_helper<1, array::length(test_ends_null), test_ends_null>::value;

constexpr static int from_two =
    test_read_int_helper<1, array::length(test_ends_null), test_ends_null>::value;

constexpr static int from_five_null =
    test_read_int_helper<5, array::length(test_ends_null), test_ends_null>::value;

constexpr int test_read_next_int() {
    static_assert(from_beginning == 123, "");
    static_assert(from_one == 123, "");
    static_assert(from_two == 123, "");
    static_assert(from_five_null == 23, "");

    using test_int_list = parse_file_to_int_list<array::length(test_ends_null), test_ends_null>::type;
    static_assert(
            std::is_same<
                test_int_list,
                IntList<123, 23>
            >::value,
            "");
    return 0;
}

constexpr int tests() {
    test_read_next_int();
    using array::length;
    using normal_case = find_numeric_range_from<
        2, length(test_ends_null), test_ends_null>;
    static_assert(normal_case::left == 2, "Left range is broken.");
    static_assert(normal_case::right == 5, "Right range is broken.");

    using works_with_null_end = find_numeric_range_from<
        6, length(test_ends_null), test_ends_null>;
    static_assert(
        works_with_null_end::left == 6, "Left range doesn't catch null.");
    static_assert(
        works_with_null_end::right == 8, "Right range doesn't catch null.");

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

    return 0;
}
// ================ End Test cases ====================

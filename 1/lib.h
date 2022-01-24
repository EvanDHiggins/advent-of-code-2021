#include <iostream>
#include <utility>
#include "lib/cexpr/list.h"
#include "lib/cexpr/valuelist.h"
#include "lib/cexpr/array.h"
#include "lib/cexpr/primitives.h"
#include "lib/cexpr/math.h"
using namespace cexpr;
using cexpr::valuelist::String;

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

template<
    int idx, int size, const char (&arr)[size], typename = void>
struct read_next_int_lstrip;

template<
    typename str, int idx, int size, const char (&arr)[size], typename = void>
struct read_next_int_helper :
    read_next_int_helper<
        cexpr::valuelist::append_t<arr[idx], str>, idx+1, size, arr>
{};

template<typename str, int idx, int size, const char (&arr)[size]>
struct read_next_int_helper<
    str, idx, size, arr,
    std::enable_if_t<arr[idx] == '\n' || arr[idx] == '\0'>
> {
    constexpr static int value = cexpr::valuelist::to_int<10, str>::value;
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



using cexpr::valuelist::ValueList;

/**
 * parse_file_to_int_list
 * parse_file_to_int_list_helper
 *
 * Assumes arr is a char array of digits seperated by spaces, with no trailing
 * newline. Parses it as a list of integers.
 */
template<
    typename int_list, int idx, int size, const char (&arr)[size],
    typename = void>
struct parse_file_to_int_list_helper;

template<typename int_list, int idx, int size, const char (&arr)[size]>
struct parse_file_to_int_list_helper<
    int_list, idx, size, arr,
    std::enable_if_t<arr[idx] == '\0' || arr[idx+1] == '\0'>>
{
    using type = int_list;
};

template<int... ints, int idx, int size, const char (&arr)[size]>
struct parse_file_to_int_list_helper<
    ValueList<ints...>, idx, size, arr,
    std::enable_if_t<arr[idx] != '\0' && arr[idx+1] != '\0'>>
{
    using next_int_t = read_next_int<idx, size, arr>;
    using type = typename parse_file_to_int_list_helper<
        ValueList<ints..., next_int_t::value>,
        next_int_t::last_seen_idx,
        size,
        arr
    >::type;

};

template<int size, const char (&arr)[size]>
struct parse_file_to_int_list 
    : parse_file_to_int_list_helper<ValueList<>, 0, size, arr>
{};



/**
 * count_increases
 * count_increases_aux
 *
 * Provided an ValueList, returns the number of consecutive pairs of numbers are
 * increasing.
 */
template<typename int_list>
struct count_increases;

template<int count, typename int_list>
struct count_increases_aux;

template<int count, int first, int second, int... rest>
struct count_increases_aux<count, ValueList<first, second, rest...>>
    : count_increases_aux<
        count + (int)(first < second), ValueList<second, rest...>>
{};

template<int count, int first, int second>
struct count_increases_aux<count, ValueList<first, second>>
    : count_increases_aux<count + (int)(first < second), ValueList<>>
{};

template<int count>
struct count_increases_aux<count, ValueList<>> {
    constexpr static int value = count;
};

template<typename int_list>
struct count_increases
 : count_increases_aux<0, int_list>
{};

/**
 * count_increases_by_window
 * count_increases_by_window_aux
 *
 * Provided an ValueList, returns the number of consecutive pairs of numbers are
 * increasing.
 */
template<typename int_list>
struct count_increases_by_window;

template<int count, typename int_list>
struct count_increases_by_window_aux;

template<int count, int first, int second, int third, int fourth, int... rest>
struct count_increases_by_window_aux<
    count, ValueList<first, second, third, fourth, rest...>>
  : count_increases_by_window_aux<
        count + (int)((first + second + third) < (second + third + fourth)),
        ValueList<second, third, fourth, rest...>>
{};

template<int count, int first, int second, int third, int fourth>
struct count_increases_by_window_aux<
    count, ValueList<first, second, third, fourth>>
  : count_increases_by_window_aux<
        count + (int)((first + second + third) < (second + third + fourth)),
        ValueList<>>
{};

template<int count>
struct count_increases_by_window_aux<count, ValueList<>> {
    constexpr static int value = count;
};

template<typename int_list>
struct count_increases_by_window
 : count_increases_by_window_aux<0, int_list> {};

# pragma once

#include <utility>
#include <stdexcept>

#include "lib/cexpr/list.h"
#include "lib/cexpr/primitives.h"
#include "lib/cexpr/valuelist.h"

namespace cexpr {
namespace array {

/**
 * length
 *
 * Returns the length of arr.
 */
template<int N>
constexpr int length(const char (&arr)[N]) {
  return N;
}


/**
 * to_typed_char_list
 *
 * Returns a List of Chars (the template type) corresponding to the elements
 * of arr.
 *
 * NOTE: This struct is reeeeaaally slow to execute. Consuming a ~9800 char
 * array took about 20 seconds on my M1 Macbook Air.
 */
template<typename word_acc_list, int curr_idx, int size, const char (&arr)[size]>
struct to_typed_char_list_impl
{
    using type = typename to_typed_char_list_impl<
        /** word_acc_list =*/
            typename list::append<Char<arr[curr_idx]>, word_acc_list>::type, 
        /** curr_idx =*/ curr_idx + 1,
        size, 
        arr>::type;
};

template<typename word_acc_list, int size, const char (&arr)[size]>
struct to_typed_char_list_impl<word_acc_list, size, size, arr>
{
    using type = word_acc_list;
};


template<int size, const char (&arr)[size]>
struct to_typed_char_list {
    using type = typename to_typed_char_list_impl<
        /** word_acc_list =*/ List<>, 
        /** curr_idx =*/ 0,
        size, 
        arr>::type;
};

/**
 * array_substring
 *
 * array_substring<start, end, size, arr>::type is a cexpr::valuelist::String containing
 * the characters from arr in [start, end).
 */
template<typename str, int start, int end, int size, const char (&arr)[size], typename = void>
struct array_substring_helper;

template<typename str, int idx, int size, const char (&arr)[size]>
struct array_substring_helper<str, idx, idx, size, arr> {
    using type = str;
};

template<char... cs, int start, int end, int size, const char (&arr)[size]>
struct array_substring_helper<
    cexpr::valuelist::String<cs...>,
    start,
    end,
    size,
    arr,
    typename std::enable_if<start != end>::type
> {
    using type = typename array_substring_helper<
            cexpr::valuelist::String<cs..., arr[start]>,
            start + 1,
            end,
            size,
            arr
        >::type;
};

template<int start, int end, int size, const char (&arr)[size]>
struct array_substring {
    using type = typename array_substring_helper<
        cexpr::valuelist::String<>,
        start,
        end,
        size,
        arr
    >::type;
};

/**
 * readline
 *
 * Auxiliary function to serve readlines. Prefer to use that instead.
 *
 * Provides a typedef, type, which contains a cexpr::valuelist::String
 * corresponding to the characters from idx until the next '\n' or '\0'
 * character.
 *
 * Also provides a constexpr int, next_index, which represents the next unread
 * index of arr. This index might point beyond the end of arr, so bounds
 * checking MUST be done before subsequent calls.
 */
template<
    typename str, int idx, int size, const char (&arr)[size], typename = void>
struct readline_helper;

template<char... chars, int idx, int size, const char (&arr)[size]>
struct readline_helper<
    cexpr::valuelist::String<chars...>, idx, size, arr,
    std::enable_if_t<arr[idx] != '\n' && arr[idx] != '\0'>>
  : readline_helper<
        cexpr::valuelist::String<chars..., arr[idx]>, idx + 1, size, arr>
{};

template<typename str, int idx, int size, const char (&arr)[size]>
struct readline_helper<
    str, idx, size, arr,
    std::enable_if_t<arr[idx] == '\n' || arr[idx] == '\0'>>
{
    static constexpr int next_index = idx + 1;
    using type = str;
};

template<int start, int size, const char (&arr)[size]>
struct readline
    : readline_helper<cexpr::valuelist::String<>, start, size, arr> {};

/**
 * readlines
 *
 * Provides a typedef, type, which is a list of strings of lines seperated by
 * newlines read from arr.
 */
template<int size, const char (&arr)[size]>
struct readlines;

template<int size, const char (&arr)[size]>
using readlines_t = typename readlines<size, arr>::type;

template<
    typename lines, int idx, int end, int size, const char (&arr)[size],
    typename = void>
struct readlines_helper;

// Chew through ws characters when we find them.
template<
  typename... lines, int idx, int end, int size, const char (&arr)[size]>
struct readlines_helper<
    List<lines...>, idx, end, size, arr,
    std::enable_if_t<arr[idx] != '\0' && arr[idx] == '\n' && idx <= end>>
  : readlines_helper<
        List<lines...>, idx + 1, end, size, arr>
{};

template<
  typename... lines, int idx, int end, int size, const char (&arr)[size]>
struct readlines_helper<
    List<lines...>, idx, end, size, arr,
    std::enable_if_t<arr[idx] != '\0' && (idx <= end) && arr[idx] != '\n'>>
{
    using line = readline<idx, size, arr>;
    using type = typename readlines_helper<
            List<lines..., typename line::type>,
            line::next_index,
            end,
            size,
            arr
        >::type;
};

template<typename list, int idx, int end, int size, const char (&arr)[size]>
struct readlines_helper<
    list, idx, end, size, arr,
    std::enable_if_t<(idx > end) || idx >= size>>
{
        using type = list;
};

template<int size, const char (&arr)[size]>
using readlines_t = typename readlines<size, arr>::type;

constexpr bool is_whitespace(char c) {
    return c == '\n' || c == ' ' || c == '\t';
}

/**
 * first_from_left
 *
 * Defines an int, value, corresponding to the leftmost index of arr
 * which isn't whitespace (ignoring '\0'). If there are no non-whitespace
 * characters, value is not defined.
 */
template<int size, const char (&arr)[size]>
struct first_from_left;

template<int idx, int size, const char (&arr)[size], typename = void>
struct first_from_left_aux;


// Failure case. We didn't find any non-ws characters.
template<int idx, int size, const char (&arr)[size]>
struct first_from_left_aux<
    idx, size, arr, std::enable_if_t<arr[idx] == '\0'>>
{};

template<int idx, int size, const char (&arr)[size]>
struct first_from_left_aux<
    idx, size, arr, std::enable_if_t<is_whitespace(arr[idx])>>
  : first_from_left_aux<idx + 1, size, arr>
{};

template<int idx, int size, const char (&arr)[size]>
struct first_from_left_aux<
    idx, size, arr,
    std::enable_if_t<!is_whitespace(arr[idx]) && arr[idx] != '\0'>>
{
    constexpr static int value = idx;
};

template<int size, const char (&arr)[size]>
struct first_from_left : first_from_left_aux<0, size, arr>
{};


/**
 * first_from_right
 *
 * Defines an int, value, corresponding to the rightmost index of arr
 * which isn't whitespace (ignoring '\0'). If there are no non-whitespace
 * characters, value is not defined.
 */
template<int curr, int size, const char (&arr)[size], typename = void>
struct first_from_right_aux;

template<int curr, int size, const char (&arr)[size]>
struct first_from_right_aux<
    curr, size, arr, std::enable_if_t<is_whitespace(arr[curr])>>
  : first_from_right_aux<curr - 1, size, arr>
{};

template<int curr, int size, const char (&arr)[size]>
struct first_from_right_aux<
    curr, size, arr, std::enable_if_t<!is_whitespace(arr[curr])>>
{
    constexpr static int value = curr;
};

// Failure case. We don't render a value if the whole string is ws.
template<int size, const char (&arr)[size]>
struct first_from_right_aux<-1, size, arr> {};

// Start recursing with size - 2 to avoid '\0' in whitespace checking.
template<int size, const char (&arr)[size]>
struct first_from_right : first_from_right_aux<size-2, size, arr>
{};

template<int size, const char (&arr)[size]>
struct readlines : readlines_helper<
    List<>,
    first_from_left<size, arr>::value,
    first_from_right<size, arr>::value, size, arr>
{};



}; // namespace array
}; // namespace cexpr

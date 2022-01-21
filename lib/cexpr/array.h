# pragma once

#include <utility>

#include "lib/cexpr/list.h"
#include "lib/cexpr/primitives.h"
#include "lib/cexpr/string.h"

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
            typename Append<Char<arr[curr_idx]>, word_acc_list>::type, 
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
 * array_substring<start, end, size, arr>::type is a String containing
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
    String<cs...>,
    start,
    end,
    size,
    arr,
    typename std::enable_if<start != end>::type
> {
    using type = typename array_substring_helper<
            String<cs..., arr[start]>,
            start + 1,
            end,
            size,
            arr
        >::type;
};

template<int start, int end, int size, const char (&arr)[size]>
struct array_substring {
    using type = typename array_substring_helper<
        String<>,
        start,
        end,
        size,
        arr
    >::type;
};

}; // namespace array
}; // namespace cexpr

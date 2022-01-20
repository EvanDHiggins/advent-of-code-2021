#include "lib/cexpr/list.h"
#include "lib/cexpr/primitives.h"

namespace cexpr {
namespace array {

template<int N>
constexpr int length(const char (&arr)[N]) {
  return N;
}


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
}; // namespace array
}; // namespace cexpr

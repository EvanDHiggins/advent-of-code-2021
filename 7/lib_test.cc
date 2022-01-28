#include "7/lib.h"
#include "lib/cexpr/array.h"
#include "lib/cexpr/list.h"
#include <limits>
#include <iostream>
using cexpr::array::length;

constexpr static char sample_crabs[] = 
R"(
0,1,1,2,2,2,4,7,14,16
)";

constexpr static int int_crabs[] = {0,1,1,2,2,2,4,7,14,16};

using sol = solution<length(sample_crabs), sample_crabs>;
//using sol2 = solution_pt2<length(sample_crabs), sample_crabs>;

using lst = valuelist::fmap_to_list_t<
    cexpr::valuelist::box_value, ValueList<6, 9, 8, 4, 3, 10, 1, 0>>;

using sorted = cexpr::valuelist::fmap_to_value_t<
    cexpr::valuelist::unbox_value, cexpr::list::merge_sort<lst>::type>;

static_assert(
    std::is_same<
        input_list,
        ValueList<0,1,3,4,6,8,9,10>
    >::value);



int main() {
}

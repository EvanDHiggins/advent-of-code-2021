#include "7/lib.h"
#include "lib/cexpr/array.h"
#include "lib/cexpr/list.h"
#include <limits>
#include <iostream>
using cexpr::array::length;

constexpr static char sample_crabs[] = 
R"(
16,1,2,0,4,2,7,1,2,14
)";

using sol = solution<length(sample_crabs), sample_crabs>;
//using sol2 = solution_pt2<length(sample_crabs), sample_crabs>;

using lst = valuelist::fmap_to_list_t<
    cexpr::valuelist::box_value, ValueList<6, 9, 8, 4, 3, 10, 1, 0>>;

using sorted = cexpr::valuelist::fmap_to_value_t<
    cexpr::valuelist::unbox_value, cexpr::list::merge_sort<lst>::type>;

static_assert(
    std::is_same<
        sorted,
        ValueList<0,1,3,4,6,8,9,10>
    >::value);

//static_assert(
    //std::is_same<
        //sol::crabs,
        //ValueList<0,1,3,4,6,8,9,10>
    //>::value);


//using crab_freq = valuelist::build_freq_map_t<
    //sol::max_crab, sol::crabs>;
//static_assert(sol::initial_fuel == 49);
//static_assert(
        //std::is_same<

        //>::value);
//static_assert(std::is_same<lst, std::integral_constant<int, 0>>::value);

int main() {
    std::cout << sol::answer << std::endl;
}

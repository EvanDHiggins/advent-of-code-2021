#include "6/lib.h"
#include "lib/cexpr/array.h"
#include "lib/cexpr/list.h"
#include <limits>
#include <iostream>
using cexpr::array::length;

constexpr static char sample_fish[] = 
R"(
3,4,3,1,2
)";

using sol = solution<length(sample_fish), sample_fish>;
using sol2 = solution_pt2<length(sample_fish), sample_fish>;

//static_assert(std::is_same<std::integral_constant<std::uint64_t, sol2::answer>)

int main() {
    std::cout << sol2::answer << std::endl;
}

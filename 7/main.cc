#include <iostream>
#include "7/lib.h"
#include "7/__generated_input.h"
#include "lib/cexpr/array.h"
#include "7/input.h"

using sol = solution<
    cexpr::array::length(INT_INPUT), 
    INT_INPUT>;

using sol_pt2 = solution_pt2<
    cexpr::array::length(PROGRAM_INPUT), 
    PROGRAM_INPUT>;

int main() {
    std::cout << "Answer to part one is: " << sol::answer << std::endl;

    std::cout << "Answer to part two is: " << sol_pt2::answer << std::endl;
}

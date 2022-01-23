#include <iostream>
#include "4/lib.h"
#include "4/__generated_input.h"
#include "lib/cexpr/array.h"

using sol = solution<
    cexpr::array::length(PROGRAM_INPUT), 
    PROGRAM_INPUT>;

using sol_pt2 = solution_pt2<
    cexpr::array::length(PROGRAM_INPUT), 
    PROGRAM_INPUT>;

int main() {
    std::cout << "Answer to part one is: " << sol::answer << std::endl;
    std::cout << "Answer to part two is: " << sol_pt2::answer << std::endl;
}

#include <iostream>
#include "6/lib.h"
#include "6/__generated_input.h"
#include "lib/cexpr/array.h"

using sol = solution<
    cexpr::array::length(PROGRAM_INPUT), 
    PROGRAM_INPUT>;

using sol_pt2 = solution_pt2<
    cexpr::array::length(PROGRAM_INPUT), 
    PROGRAM_INPUT>;

int main() {
     //388739
    std::cout << "Answer to part one is: " << sol::answer << std::endl;

    //1741362314973
    std::cout << "Answer to part two is: " << sol_pt2::answer << std::endl;
}

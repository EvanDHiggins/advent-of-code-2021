#include <iostream>
#include "7/lib.h"
#include "7/__generated_input.h"
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
    static_assert(
            std::is_same<
                sol::crabs,
                cexpr::valuelist::ValueList<>
            >::value);

    //1741362314973
    std::cout << "Answer to part two is: " << sol_pt2::answer << std::endl;
}

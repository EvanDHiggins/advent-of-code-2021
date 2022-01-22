#include <iostream>
#include "3/lib.h"
#include "3/__generated_input.h"

using sol = solution<
    cexpr::array::length(PROGRAM_INPUT), 
    PROGRAM_INPUT>;

int main() {
    std::cout << "Answer to part one is: " << sol::part_one_answer << std::endl;
    std::cout << "Answer to part two is: " << sol::part_two_answer << std::endl;
}

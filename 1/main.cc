#include <iostream>
#include <utility>
#include "1/__generated_input.h"
#include "1/lib.h"

using int_list = parse_file_to_int_list<
    array::length(PROGRAM_INPUT), PROGRAM_INPUT
>::type;
constexpr static int answer = count_increases<int_list>::value;

constexpr static int answer_by_window =
    count_increases_by_window<int_list>::value;


int main() {
    std::cout << "The answer is " << answer << std::endl;
    std::cout << "The answer for part 2 is " << answer_by_window << std::endl;
}
// ================ End Test cases ====================

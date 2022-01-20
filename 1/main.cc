#include <iostream>
#include <utility>
#include "1/__generated_input.h"
#include "lib/cexpr/list.h"
#include "lib/cexpr/array.h"
#include "lib/cexpr/primitives.h"

using namespace cexpr;

int main() {
    using value =
        array::to_typed_char_list<
            array::length(PROGRAM_INPUT), PROGRAM_INPUT>::type;
    std::cout << Nth<2, value>::type::as_char << std::endl;
}

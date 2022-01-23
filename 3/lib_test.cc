#include "lib/cexpr/list.h"
#include "3/lib.h"
#include "lib/cexpr/array.h"

using cexpr::array::length;
using cexpr::array::readlines;
using cexpr::List;
using cexpr::list::fmap;

constexpr char sample_input[] =
R"(00100
11110
10110
10111
10101
01111
00111
11100
10000
11001
00010
01010
)";
constexpr static int expected_gamma = 22;
constexpr static int expected_epsilon = 9;

using lines = readlines<length(sample_input), sample_input>::type;

using intlists = fmap<
    negate_zeroes,
    fmap<str_to_intlist, lines>::type
>::type;

using counts = fold_int_lists<intlists>::type;
using gamma_bits = ifmap<compute_gamma, counts>::type;
using epsilon_bits = ifmap<compute_epsilon, counts>::type;
constexpr static int gamma =
    to_int<2, gamma_bits>::value;
constexpr static int epsilon =
    to_int<2, epsilon_bits>::value;

static_assert(gamma == expected_gamma);
static_assert(epsilon == expected_epsilon);

int main() {

}

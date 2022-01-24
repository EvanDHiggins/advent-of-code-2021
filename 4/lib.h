#pragma once
#include "lib/cexpr/array.h"
using namespace cexpr;
using cexpr::valuelist::ValueList;

template<typename... lists>
using Grid = List<lists...>;

template<typename lines>
struct parse_grid;

template<
    typename line1,
    typename line2,
    typename line3,
    typename line4,
    typename line5>
struct parse_grid<List<line1, line2, line3, line4, line5>> {
    using type = Grid<
    >;
};

template<typename lines>
struct read_boards {
    using type = list::prepend_t<
        typename parse_grid<list::take_t<5, lines>>::type,
        typename read_boards<list::drop_t<5, lines>>::type
    >;
};

template<>
struct read_boards<List<>> {
    using type = List<>;
};

template<int size, const char (&arr)[size]>
struct parse_input {
    using lines = typename cexpr::array::readlines<size, arr>::type;
    using draws = list::take_t<1, lines>;
    using boards = typename read_boards<list::drop_t<1, lines>>::type;

};

template<int size, const char (&arr)[size]>
struct solution {

    using parsed_input = parse_input<size, arr>;

    constexpr static int answer = 0;
};

template<int size, const char (&arr)[size]>
struct solution_pt2 {
    constexpr static int answer = 0;
};

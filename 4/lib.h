#pragma once
#include "lib/cexpr/array.h"
using namespace cexpr;
using cexpr::valuelist::ValueList;

template<typename _rows>
struct Grid {
    using rows = _rows;
};

template<int x, int y, typename grid>
struct get_cell {
    using type = cexpr::list::nth_t<x, cexpr::list::nth_t<y, typename grid::rows>>;
};

template<int x, int y, typename grid>
using get_cell_t = typename get_cell<x, y, grid>::type;

template<bool _drawn, int _value>
struct Cell {
    constexpr static bool drawn = _drawn;
    constexpr static int value = _value;
};

template<typename cell>
using draw = Cell<true, cell::value>;

// Indexed from the top left
template<int x, int y, typename grid>
struct draw_cell;

template<int x, int y, typename rows>
struct draw_cell<x, y, Grid<rows>> {
    using row = cexpr::list::nth_t<y, rows>;
    using type = Grid<cexpr::list::set_nth_t<y,
          cexpr::list::set_nth_t<
              x, draw<cexpr::list::nth_t<x, row>>, row>,
          rows>>;
};

template<int x, int y, typename grid>
using draw_cell_t = typename draw_cell<x, y, grid>::type;

template<int n, typename grid>
struct draw_number;

template<int n>
struct drawer {
    template<typename grid>
    struct func {
        using type = typename draw_number<n, grid>::type;
    };
};

template<int n, typename row, typename = void>
struct draw_number_in_row;

template<int n, typename x, typename... xs>
struct draw_number_in_row<
    n, List<x, xs...>, std::enable_if_t<x::value == n>> {
    using type = cexpr::list::prepend_t<
            draw<x>,
            typename draw_number_in_row<n, List<xs...>>::type
        >;
};

template<int n, typename x, typename... xs>
struct draw_number_in_row<
    n, List<x, xs...>, std::enable_if_t<x::value != n>> {
    using type = cexpr::list::prepend_t<
            x,
            typename draw_number_in_row<n, List<xs...>>::type
        >;
};

template<int n>
struct draw_number_in_row<n, List<>> {
    using type = List<>;
};

template<int n, typename grid>
struct draw_number {
    template<typename row>
    using draw_n = draw_number_in_row<n, row>;
    using type = Grid<
            cexpr::list::fmap_t<
                draw_n,
                typename grid::rows
            >
        >;
};

template<typename cell>
struct is_drawn {
    using type = std::integral_constant<bool, cell::drawn>;
};

/**
 * check_rows
 */
template<typename row>
struct check_row {
    using type = cexpr::list::all_t<is_drawn, row>;
};

template<typename grid>
struct check_rows;

template<typename rows>
struct check_rows<Grid<rows>> {
    using type = typename cexpr::list::any_t<
        check_row,
        rows>;
};

template<typename grid>
using check_rows_t = typename check_rows<grid>::type;


template<typename grid>
struct check_columns;

template<typename lst>
using take_one = cexpr::list::take<1, lst>;

template<int n, typename grid>
struct nth_column;

template<int n, typename rows>
struct nth_column<n, Grid<rows>> {
    template<typename lst>
    using take_n = cexpr::list::nth<n, lst>;
    using type = cexpr::list::fmap_t<take_n, rows>;
};

template<int n>
struct check_type;

// Transforms columns into rows (lists) and checks them with check_row.
template<typename grid>
struct check_columns {
    template<int col>
    using int_to_col = nth_column<col, grid>;
    using type = 
        typename cexpr::list::any_t<
            check_row,
            cexpr::valuelist::fmap_to_list_t<
                int_to_col,
                cexpr::valuelist::ValueList<0, 1, 2, 3, 4>>>;
};

template<typename grid>
using check_columns_t = typename check_columns<grid>::type;

template<typename grid>
struct has_bingo {
    constexpr static bool value =
        check_rows_t<grid>::value
        || check_columns_t<grid>::value;
};


template<typename lines>
struct parse_grid;

template<int value>
struct new_cell {
    using type = Cell<false, value>;
};

template<typename str>
using to_base_ten = cexpr::valuelist::to_int<10, str>;

template<typename str>
struct parse_line {
    using type =
        cexpr::valuelist::fmap_to_list_t<
            new_cell,
            cexpr::valuelist::fmap_to_value_t<
                to_base_ten,
                cexpr::valuelist::split_t<' ', str>>>;
};

template<typename lines>
struct parse_grid {
    using type = Grid<cexpr::list::fmap_t<parse_line, lines>>;
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
    using draws =
        cexpr::valuelist::fmap_to_value_t<
            to_base_ten,
            cexpr::valuelist::split_t<',', list::head_t<lines>>>;
    using boards = typename read_boards<list::drop_t<1, lines>>::type;

};

template<template<typename> typename func, typename grid>
struct gmap {
    template<typename row>
    struct map_row {
        using type = cexpr::list::fmap_t<func, row>;
    };
    using type = Grid<
            cexpr::list::fmap_t<map_row, typename grid::rows>
        >;
};

template<typename cell>
struct cell_to_score_value;

template<int value>
struct cell_to_score_value<Cell<true, value>> {
    using type = std::integral_constant<int, 0>;
};

template<int value>
struct cell_to_score_value<Cell<false, value>> {
    using type = std::integral_constant<int, value>;
};

template<typename lhs, typename rhs>
struct sum {
    using type = std::integral_constant<int, lhs::value + rhs::value>;
};

template<typename t, typename r, typename = void>
struct comp;

template<typename t, typename r>
struct comp<t, r, std::enable_if_t<std::is_same<t, r>::value>> {};

template<typename grid>
struct count_undrawn {
    using int_board = typename gmap<cell_to_score_value, grid>::type;

    template<typename before, typename after, typename = void>
    struct maybe_print_boards;

    template<typename before, typename after>
    struct maybe_print_boards<before, after, std::enable_if_t<has_bingo<before>::value>> {
        using t = comp<before, after>;
    };

    template<typename before, typename after>
    struct maybe_print_boards<before, after, std::enable_if_t<!has_bingo<before>::value>> {
        using t = comp<before, after>;
    };
    using f = maybe_print_boards<grid, int_board>;


    template<typename row>
    struct sum_row {
        using type = cexpr::list::lfold_t<
            sum, std::integral_constant<int, 0>, row>;
    };
    using type = cexpr::list::lfold_t<
        sum,
        std::integral_constant<int, 0>,
        cexpr::list::fmap_t<sum_row, typename int_board::rows>>;

};

// Returns a non-zero number if the board is a winner.
template<typename grid>
struct get_bingo_card_value;

template<bool winner, typename grid>
struct get_bingo_card_value_aux;

template<typename grid>
struct get_bingo_card_value_aux<false, grid> {
    using type = std::integral_constant<int, 0>;
};

template<typename grid>
struct get_bingo_card_value_aux<true, grid> {
    using type = typename count_undrawn<grid>::type;
};

template<typename grid>
struct get_bingo_card_value
    : get_bingo_card_value_aux<has_bingo<grid>::value, grid> {};

template<typename lhs, typename rhs, typename = void>
struct _max;

template<typename lhs, typename rhs>
using max = _max<lhs, rhs>;

template<typename lhs, typename rhs>
struct _max<lhs, rhs, std::enable_if_t<(lhs::value < rhs::value)>> {
    using type = rhs;
};

template<typename lhs, typename rhs>
struct _max<lhs, rhs, std::enable_if_t<(lhs::value >= rhs::value)>> {
    using type = lhs;
};

template<typename cards>
struct find_highscore {
    constexpr static int value = cexpr::list::lfold_t<
        max,
        std::integral_constant<int, 0>,
        cexpr::list::fmap_t<get_bingo_card_value, cards>>::value;
};

template<typename draws, typename cards>
struct run_game_until_winner;

template<int last_draw, int highscore, typename draws, typename cards, typename = void>
struct run_game_until_winner_aux;

template<int last_draw, int draw, int... draws, typename cards>
struct run_game_until_winner_aux<
    last_draw, 0, ValueList<draw, draws...>, cards> {

    template<typename card>
    using my_drawer = typename drawer<draw>::template func<card>;

    using new_cards = cexpr::list::fmap_t<my_drawer, cards>;
    using type = typename run_game_until_winner_aux<
            draw,
            find_highscore<new_cards>::value,
            ValueList<draws...>,
            new_cards
        >::type;
};

template<int last_draw, int highscore, typename draws, typename cards>
struct run_game_until_winner_aux<last_draw, highscore, draws, cards, std::enable_if_t<highscore != 0>> {
    using type = std::integral_constant<int, highscore * last_draw>;
};

template<typename draws, typename cards>
struct run_game_until_winner : run_game_until_winner_aux<0, 0, draws, cards> {};

template<typename card>
struct no_bingo {
    constexpr static bool value = !has_bingo<card>::value;
};

template<typename cards>
struct filter_winners_unless_one;

template<typename winner>
struct filter_winners_unless_one<List<winner>> {
    using type = List<winner>;
};


template<typename cards>
struct filter_winners_unless_one {
    using type = cexpr::list::filter_t<
            no_bingo,
            cards
        >;
};

template<int last_draw, int highscore, typename draws, typename cards, typename = void>
struct run_game_until_loser_aux;

template<int last_draw, int draw, int... draws, typename cards>
struct run_game_until_loser_aux<
    last_draw, 0, ValueList<draw, draws...>, cards> {
    template<typename card>
    using my_drawer = typename drawer<draw>::template func<card>;
    using new_cards_temp = cexpr::list::fmap_t<my_drawer, cards>;
    using new_cards = typename filter_winners_unless_one<new_cards_temp>::type;
    using type = typename run_game_until_loser_aux<
            draw,
            find_highscore<new_cards>::value,
            ValueList<draws...>,
            new_cards
        >::type;
};

template<int last_draw, int highscore, typename draws, typename last_card>
struct run_game_until_loser_aux<
    last_draw, highscore, draws, List<last_card>,
    std::enable_if_t<highscore != 0>> {
        using type = std::integral_constant<int, highscore * last_draw>;
};

template<typename draws, typename cards>
struct run_game_until_loser : run_game_until_loser_aux<0, 0, draws, cards> {};

template<int size, const char (&arr)[size]>
struct solution {
    using parsed_input = parse_input<size, arr>;
    constexpr static int answer = run_game_until_winner<
        typename parsed_input::draws, typename parsed_input::boards>::type::value;
};

template<int size, const char (&arr)[size]>
struct solution_pt2 {
    using parsed_input = parse_input<size, arr>;

    constexpr static int answer = run_game_until_loser<
        typename parsed_input::draws, typename parsed_input::boards>::type::value;
};

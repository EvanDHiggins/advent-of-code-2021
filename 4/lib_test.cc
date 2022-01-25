#include "4/lib.h"
#include "lib/cexpr/array.h"
#include "lib/cexpr/list.h"

constexpr char test_data[] =
R"(
7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

22 13 17 11  0
 8  2 23  4 24
21  9 14 16  7
 6 10  3 18  5
 1 12 20 15 19

 3 15  0  2 22
 9 18 13 17  5
19  8  7 25 23
20 11 10 24  4
14 21 16 12  6

14 21 17 24  4
10 16 15  9 19
18  8 23 26 20
22 11 13  6  5
 2  0 12  3  7
)";

using first_board = Grid<
    List<
        valuelist::fmap_to_list_t<new_cell, ValueList<22, 13, 17, 11, 0>>,
        valuelist::fmap_to_list_t<new_cell, ValueList<8, 2, 23, 4, 24>>,
        valuelist::fmap_to_list_t<new_cell, ValueList<21, 9, 14, 16, 7>>,
        valuelist::fmap_to_list_t<new_cell, ValueList<6, 10, 3, 18, 5>>,
        valuelist::fmap_to_list_t<new_cell, ValueList<1, 12, 20, 15, 19>>
    >
>;

using first_row_complete =
    draw_cell_t<0, 1,
    draw_cell_t<1, 1,
    draw_cell_t<2, 1,
    draw_cell_t<3, 1,
    draw_cell_t<4, 1, first_board>>>>>;

using second_col_complete =
    draw_cell_t<2, 0,
    draw_cell_t<2, 1,
    draw_cell_t<2, 2,
    draw_cell_t<2, 3,
    draw_cell_t<2, 4, first_board>>>>>;

using left_diag_complete =
    draw_cell_t<0, 0,
    draw_cell_t<1, 1,
    draw_cell_t<2, 2,
    draw_cell_t<3, 3,
    draw_cell_t<4, 4, first_board>>>>>;

using right_diag_complete =
    draw_cell_t<0, 4,
    draw_cell_t<1, 3,
    draw_cell_t<2, 2,
    draw_cell_t<3, 1,
    draw_cell_t<4, 0, first_board>>>>>;
    

using sol = solution<cexpr::array::length(test_data), test_data>;

using actual_draws = cexpr::valuelist::take_t<12, sol::parsed_input::draws>;
using winning_board = cexpr::list::drop_t<2, sol::parsed_input::boards>;


//static_assert(std::is_same<sol::parsed_input::boards, cexpr::List<>>::value);
static_assert(
        std::is_same<
            cexpr::Nth<0, sol::parsed_input::boards>::type,
            first_board>::value);

static_assert(check_rows_t<first_row_complete>::value);
static_assert(check_columns_t<second_col_complete>::value);
static_assert(check_diag_t<left_diag_complete>::value);
static_assert(check_diag_t<left_diag_complete>::value);
static_assert(has_bingo<first_row_complete>::value);
static_assert(has_bingo<second_col_complete>::value);
static_assert(has_bingo<right_diag_complete>::value);
//static_assert(run_game_until_winner<actual_draws, winning_board>::type::value);
static_assert(std::is_same<
            std::integral_constant<int, 0>,
            get_bingo_card_value<cexpr::list::head_t<sol::parsed_input::boards>>::type
        >::value);
static_assert(std::is_same<
            std::integral_constant<int, 239>,
            get_bingo_card_value<first_row_complete>::type
        >::value);

// 7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

template<typename draws, typename cards>
struct iter;

template<int draw, int... draws, typename cards>
struct iter<ValueList<draw, draws...>, cards> {
    template<typename card>
    using my_drawer = typename drawer<draw>::template func<card>;

    using new_cards = cexpr::list::fmap_t<my_drawer, cards>;
    using highscore = std::integral_constant<int, find_highscore<new_cards>::value>;
    using next_draws = ValueList<draws...>;
};

template<typename last_iter>
struct next_iter : iter<typename last_iter::next_draws, typename last_iter::new_cards> {};

using first_iter = iter<actual_draws, winning_board>;
using two = next_iter<first_iter>;
using three = next_iter<two>;
using four = next_iter<three>;
using five = next_iter<four>;
using six = next_iter<five>;
using seven = next_iter<six>;
using eight = next_iter<seven>;
using nine = next_iter<eight>;
using ten = next_iter<nine>;
using eleven = next_iter<ten>;
using twelve = next_iter<eleven>;

static_assert(std::is_same<std::integral_constant<int, 0>, twelve::highscore>::value);
static_assert(std::is_same<twelve::new_cards, List<>>::value);
static_assert(std::is_same<twelve::next_draws, List<>>::value);
static_assert(std::is_same<get_bingo_card_value<cexpr::list::head_t<twelve::new_cards>>, std::integral_constant<int, 0>>::value);


//using final_board =
            //d_t<7,
            //d_t<4,
            //d_t<9,
            //d_t<5,
            //d_t<11,
            //d_t<17,
            //d_t<23,
            //d_t<2,
            //d_t<0,
            //d_t<14,
            //d_t<21,
            //d_t<24,
            //d_t<10,
            //d_t<16,
            //d_t<13,
            //d_t<6,
            //d_t<15,
            //d_t<25,
            //d_t<12,
            //d_t<22,
            //d_t<18,
            //d_t<20,
            //d_t<8,
            //d_t<19,
            //d_t<3,
            //d_t<26,
            //d_t<1,
            //cexpr::list::nth_t<2, sol::parsed_input::boards>
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>;
//static_assert(
            //std::is_same<get_bingo_card_value<final_board>,
            //List<>
        //>::value);
//static_assert(std::is_same<std::integral_constant<int, 0>, sol::parsed_input::draws>::value);
//static_assert(
        //std::is_same<
            //std::integral_constant<int, 0>,
            //run_game_until_winner<actual_draws, winning_board>::type
        //>::value);
//static_assert(std::is_same<
            //std::integral_constant<int, sol::answer>,
            //std::integral_constant<int, 4512>
        //>::value);
//static_assert(std::is_same<sol::parsed_input::boards, cexpr::List<>>::value);

int main() {}

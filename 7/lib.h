#pragma once
#include "lib/cexpr/array.h"
#include "lib/cexpr/math.h"
#include <numeric>
using namespace cexpr;
using cexpr::valuelist::ValueList;

template<std::int64_t _count, std::int64_t _fuel>
struct CrabWindow {
    constexpr static decltype(_count) count = _count;
    constexpr static decltype(_fuel) fuel = _fuel;
};

template<typename freq>
struct initial_fuel;

template<std::int64_t idx, typename freq>
struct initial_fuel_aux;

template<std::int64_t idx, auto crab, auto... crabs>
struct initial_fuel_aux<idx, ValueList<crab, crabs...>> {
    constexpr static std::int64_t value =
        crab * idx + initial_fuel_aux<idx + 1, ValueList<crabs...>>::value;
};

template<std::int64_t idx>
struct initial_fuel_aux<idx, ValueList<>> {
    constexpr static std::int64_t value = 0;
};

template<typename freq>
struct initial_fuel : initial_fuel_aux<0, freq> {};

template<
    std::int64_t best,
    int pos,
    typename left_window, 
    typename crab_freq,
    typename right_window,
    typename = void>
struct align_crabs;

template<
    std::int64_t best,
    int pos,
    auto left_crabs,
    auto left_fuel,
    typename crab_freq,
    auto right_crabs,
    auto right_fuel>
struct align_crabs<
    best,
    pos,
    CrabWindow<left_crabs, left_fuel>,
    crab_freq,
    CrabWindow<right_crabs, right_fuel>,
    std::enable_if_t<(pos + 1 < valuelist::length<crab_freq>::value)>> {
        constexpr static int new_best =
            math::min<best, left_fuel + right_fuel>::value;
        constexpr static std::int64_t value =
            align_crabs<
                new_best,
                pos + 1,
                CrabWindow<
                    left_crabs + valuelist::nth<pos, crab_freq>::value,
                    left_fuel
                        + valuelist::nth<pos, crab_freq>::value + left_crabs>,
                crab_freq,
                CrabWindow<
                    right_crabs - valuelist::nth<pos + 1, crab_freq>::value,
                    right_fuel - right_crabs
                >
            >::value;
};

template<
    std::int64_t best,
    int pos,
    typename left_window, 
    typename crab_freq,
    typename right_window>
struct align_crabs<
    best, pos, left_window, crab_freq, right_window,
    std::enable_if_t<(pos + 1 >= valuelist::length<crab_freq>::value)>> {
    constexpr static std::int64_t value =
        math::min<best, left_window::fuel>::value;
};


template<int size, const char (&arr)[size]>
struct solution {

    using unsorted_crabs = valuelist::fmap_to_value_t<
        valuelist::to_base_ten,
        valuelist::split_t<
            ',', list::head_t<array::readlines_t<size, arr>>>>;

    using crabs = valuelist::fmap_to_value_t<
            valuelist::unbox_value,
            typename list::merge_sort<
                valuelist::fmap_to_list_t<
                    valuelist::box_value, unsorted_crabs>>::type>;

    constexpr static std::int64_t max_crab = valuelist::max<crabs>::value;

    using crab_freq = valuelist::build_freq_map_t<
        max_crab, crabs>;
    constexpr static std::uint64_t total_crabs =
        valuelist::sum<crab_freq>::value;
    constexpr static std::int64_t initial_fuel =
        ::initial_fuel<crab_freq>::value;

    using left_window = CrabWindow<0, 0>;
    using right_window = CrabWindow<
        total_crabs - valuelist::nth<0, crab_freq>::value,
        initial_fuel
    >;

    using t = initial_fuel::type;

    constexpr static std::uint64_t answer = align_crabs<
        std::numeric_limits<std::int64_t>::max(),
        0,
        left_window,
        crab_freq,
        right_window>::value;

};

template<int size, const char (&arr)[size]>
struct solution_pt2 {
    constexpr static std::uint64_t answer = 0;
};

#pragma once
#include "lib/cexpr/array.h"
using namespace cexpr;
using cexpr::valuelist::ValueList;

template<typename fishies>
struct next_day {
    constexpr static unsigned long long spawners = cexpr::valuelist::nth<0, fishies>::value;
    using type = cexpr::valuelist::append_t<
        spawners,
        cexpr::valuelist::set_nth_t<
            6,
            spawners + (unsigned long long)cexpr::valuelist::nth<7, fishies>::value,
            cexpr::valuelist::tail_t<fishies>>>;
};



template<typename freq, typename fishies>
struct count_fish;

template<typename freq, int fish, int... fishies>
struct count_fish<freq, ValueList<fish, fishies...>> {
    using type = typename count_fish<
            valuelist::set_nth_t<
                fish, valuelist::nth<fish, freq>::value + 1, freq>,
            ValueList<fishies...>
        >::type;
};

template<typename freq>
struct count_fish<freq, ValueList<>> {
    using type = freq;
};

template<int days, typename fishie_freq, typename = void>
struct simulate_days;

template<int days, typename fishie_freq>
struct simulate_days<days, fishie_freq, std::enable_if_t<days != 0>> {
    constexpr static unsigned long long value =
        simulate_days<days - 1, typename next_day<fishie_freq>::type>::value;
};

template<typename fishie_freq>
struct simulate_days<0, fishie_freq> {
    constexpr static unsigned long long value =
        valuelist::sum<fishie_freq>::value;
};

template<unsigned long long... values>
using LongList = ValueList<values...>;

template<int days, typename fishies>
struct count_fish_in_n_days {
    constexpr static unsigned long long value = simulate_days<
        days,
        typename count_fish<LongList<
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0>, fishies>::type
    >::value;
};

template<int size, const char (&arr)[size]>
struct solution {
    using input = cexpr::valuelist::fmap_to_value_t<
        cexpr::valuelist::to_base_ten,
        cexpr::valuelist::split_t<
            ',', cexpr::list::head_t<cexpr::array::readlines_t<size, arr>>>>;
    constexpr static unsigned long long answer =
        count_fish_in_n_days<80, input>::value;
            
};

template<int size, const char (&arr)[size]>
struct solution_pt2 {
    using input = cexpr::valuelist::fmap_to_value_t<
        cexpr::valuelist::to_base_ten,
        cexpr::valuelist::split_t<
            ',', cexpr::list::head_t<cexpr::array::readlines_t<size, arr>>>>;
    constexpr static unsigned long long answer =
        count_fish_in_n_days<256, input>::value;
};

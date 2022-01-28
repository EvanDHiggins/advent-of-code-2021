#pragma once
#include "lib/cexpr/array.h"
#include "lib/cexpr/math.h"
#include "lib/cexpr/array.h"
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
    std::size_t furthest_crab,
    const std::int64_t (&crab_freq)[furthest_crab],
    typename right_window,
    typename = void>
struct align_crabs;

template<
    std::int64_t best,
    int pos,
    auto left_crabs,
    auto left_fuel,
    std::size_t furthest_crab,
    const std::int64_t (&crab_freq)[furthest_crab],
    auto right_crabs,
    auto right_fuel>
struct align_crabs<
    best,
    pos,
    CrabWindow<left_crabs, left_fuel>,
    furthest_crab,
    crab_freq,
    CrabWindow<right_crabs, right_fuel>,
    std::enable_if_t<(pos + 1 < furthest_crab)>> {
        constexpr static int new_best =
            math::min<best, left_fuel + right_fuel>::value;
        constexpr static std::int64_t value =
            align_crabs<
                new_best,
                pos + 1,
                CrabWindow<
                    left_crabs + crab_freq[pos],
                    left_fuel + crab_freq[pos] + left_crabs>,
                furthest_crab,
                crab_freq,
                CrabWindow<
                    right_crabs - crab_freq[pos + 1],
                    right_fuel - right_crabs
                >
            >::value;
};

template<
    std::int64_t best,
    int pos,
    typename left_window, 
    std::size_t furthest_crab,
    const std::int64_t (&crab_freq)[furthest_crab],
    typename right_window>
struct align_crabs<
    best, pos, left_window, furthest_crab, crab_freq, right_window,
    std::enable_if_t<(pos + 1 >= furthest_crab)>> {
    constexpr static std::int64_t value =
        math::min<best, left_window::fuel>::value;
};

template<int size, const char (&arr)[size]>
struct fast_comma_split;

template<
    typename ints,
    int idx,
    typename buffer,
    int size, const char (&arr)[size],
    typename = void>
struct fast_comma_split_aux;

template<int size, const char (&arr)[size]>
struct fast_comma_split
    : fast_comma_split_aux<ValueList<>, 0, ValueList<>, size, arr> {};

template<
    typename ints,
    int idx,
    typename buffer,
    int size,
    const char (&arr)[size]>
struct fast_comma_split_aux<
    ints, idx, buffer, size, arr, std::enable_if_t<(idx+1 >= size)>> {
        using type = valuelist::append_t<
            valuelist::to_base_ten<buffer>::value, ints>;
};

template<
    typename ints,
    int idx,
    typename buffer,
    int size,
    const char (&arr)[size]>
struct fast_comma_split_aux<
    ints, idx, buffer, size, arr, std::enable_if_t<
        arr[idx] != ',' && arr[idx] != '\n' && idx+1 < size>> {
        using type = typename fast_comma_split_aux<
                ints,
                idx + 1,
                valuelist::append_t<arr[idx], buffer>,
                size,
                arr
            >::type;
};

template<
    typename ints,
    int idx,
    typename buffer,
    int size,
    const char (&arr)[size]>
struct fast_comma_split_aux<
    ints, idx, buffer, size, arr, std::enable_if_t<
        arr[idx] == '\n'>> {
        using type = typename fast_comma_split_aux<
                ints,
                idx + 1,
                buffer,
                size,
                arr
            >::type;
};


template<
    typename ints,
    int idx,
    typename buffer,
    int size,
    const char (&arr)[size]>
struct fast_comma_split_aux<
    ints, idx, buffer, size, arr, std::enable_if_t<arr[idx] == ','>> {
        using type = typename fast_comma_split_aux<
                valuelist::append_t<
                    valuelist::to_base_ten<buffer>::value, ints>,
                idx + 1,
                ValueList<>,
                size,
                arr
            >::type;
};

template<typename freq_map>
struct FreqMapArray;

template<auto value, auto... rest>
struct FreqMapArray<ValueList<value, rest...>> {
    constexpr static decltype(value) values[sizeof...(rest)+1] = {
        value,
        rest...
    };
    constexpr static std::size_t size = sizeof...(rest) + 1;
};

template<
    std::size_t idx,
    std::size_t size,
    const std::int64_t (&arr)[size],
    typename = void>
struct read_int_array_aux;

template<
    std::size_t idx,
    std::size_t size,
    const std::int64_t (&arr)[size]>
struct read_int_array_aux<idx, size, arr, std::enable_if_t<idx == size>> {
    using type = ValueList<>;
};

template<
    std::size_t idx,
    std::size_t size,
    const std::int64_t (&arr)[size]>
struct read_int_array_aux<idx, size, arr, std::enable_if_t<(idx < size)>> {
    using type = valuelist::prepend_t<
            arr[idx],
            typename read_int_array_aux<idx + 1, size, arr>::type
        >;
};

template<std::size_t size, const std::int64_t (&arr)[size]>
struct read_int_array : read_int_array_aux<0, size, arr> {};

template<int size, const std::int64_t (&arr)[size]>
struct solution {

    using crabs =
        typename read_int_array<size, arr>::type;

    constexpr static std::int64_t max_crab = valuelist::max<crabs>::value;

    using crab_freq = typename valuelist::build_freq_map_from_sorted<
        max_crab, crabs>::type;

    using crab_freq_array = FreqMapArray<crab_freq>;

    constexpr static std::uint64_t total_crabs =
        valuelist::sum<crab_freq>::value;
    constexpr static std::int64_t initial_fuel =
        ::initial_fuel<crab_freq>::value;

    using left_window = CrabWindow<0, 0>;
    using right_window = CrabWindow<
        total_crabs - valuelist::nth<0, crab_freq>::value,
        initial_fuel
    >;
    
    constexpr static std::uint64_t answer = align_crabs<
        std::numeric_limits<std::int64_t>::max(),
        0,
        left_window,
        crab_freq::size,
        crab_freq::values,
        right_window>::value;

};

template<int size, const char (&arr)[size]>
struct solution_pt2 {
    constexpr static std::uint64_t answer = 0;
};

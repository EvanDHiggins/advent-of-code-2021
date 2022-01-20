#pragma once

/**
 * Templates support non-type parameters like 'int', 'char', etc., but they
 * exist in a slightly different namespace as types. And we can't write
 * generic templates with them until C++14. So instead we will wrap primitives
 * in template types so we can write types paramaterized by them.
 */
template<char c>
struct Char {
    constexpr static char as_char = c;
};

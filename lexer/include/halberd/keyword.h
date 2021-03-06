#pragma once

#include <utility> // std::pair

#include <cstddef> // std::size_t


namespace halberd
{
namespace lexer
{
    // Keywords are split into strict and weak categories
    // strict: keywords can only be used in their expected contexts
    // weak:   keywords have special meaning only in certain contexts
    enum class keyword
    {
        strict_var,
        strict_const,
        strict_i8,
        strict_i16,
        strict_i32,
        strict_i64
    };

    std::pair<keyword, bool> to_keyword(const char* str);
    std::pair<keyword, bool> to_keyword(const char* str, std::size_t length);

    template<std::size_t N>
    std::pair<keyword, bool> to_keyword(const char (&str)[N])
    {
        return to_keyword(str, length(str));
    }

    const char* to_string(keyword kw) noexcept;
}
}

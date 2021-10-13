#pragma once

#include <cstddef> // std::size_t


namespace halberd
{
namespace util
{
    template<std::size_t N>
    constexpr std::size_t length(const char (&str)[N]) noexcept
    {
        return str[N - 1U] != '\0' ? N : N - 1U;
    }
}
}

#pragma once

#include <cstddef> // std::size_t


namespace halberd
{
namespace parser
{
    // Index type

    template<std::size_t Idx>
    struct index_tag
    {
    };

    // Variables

    template<std::size_t Idx>
    constexpr index_tag<Idx> index_tag_v;
}
}

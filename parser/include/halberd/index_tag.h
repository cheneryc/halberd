#pragma once

#include <cstddef> // std::size_t


namespace halberd
{
namespace parser
{
    // Index types

    template<std::size_t Idx>
    struct index_tag
    {
    };

    template<std::size_t... Is>
    struct index_sequence_tag
    {
    };

    // Variables

    template<std::size_t Idx>
    constexpr index_tag<Idx> index_tag_v;

    template<std::size_t... Is>
    constexpr index_sequence_tag<Is...> index_sequence_tag_v;
}
}

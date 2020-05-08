#pragma once

#include <cstddef> // std::size_t


namespace halberd
{
namespace lexer
{
    // Index types

    using state_index_t = std::size_t;

    template<state_index_t Idx>
    struct state_index_tag
    {
        const state_index_t value = Idx;
    };

    struct state_index_self_tag
    {
    };
    
    // Variables

    template<state_index_t Idx>
    constexpr state_index_tag<Idx> state_index_tag_v;
}
}

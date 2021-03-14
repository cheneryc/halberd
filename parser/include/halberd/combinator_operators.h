#pragma once

#include "combinator.h"
#include "combinator_choice.h"
#include "combinator_sequence.h"

#include <type_traits> // std::enable_if_t


namespace halberd
{
namespace parser
{
    template<typename P1,
             typename P2,
             typename = std::enable_if_t<is_combinator<P1>::value>,
             typename = std::enable_if_t<is_combinator<P2>::value>>
    constexpr auto operator|(P1 p1, P2 p2) noexcept
    {
        auto choice1 = make_choice(std::move(p1));
        auto choice2 = make_choice(std::move(p2));

        return concat(std::move(choice1), std::move(choice2));
    }

    template<typename P1,
             typename P2,
             typename = std::enable_if_t<is_combinator<P1>::value>,
             typename = std::enable_if_t<is_combinator<P2>::value>>
    constexpr auto operator+(P1 p1, P2 p2) noexcept
    {
        auto seq1 = make_sequence(std::move(p1));
        auto seq2 = make_sequence(std::move(p2));

        return concat(std::move(seq1), std::move(seq2));
    }
}
}

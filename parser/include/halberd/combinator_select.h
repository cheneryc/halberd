#pragma once

#include "combinator.h"
#include "parse_result.h"
#include "source.h"

#include <utility> // std::index_sequence

#include <cstddef> // std::size_t


namespace halberd
{
namespace parser
{
    template<typename P, std::size_t... Is>
    class combinator_select : public combinator
    {
    public:
        constexpr combinator_select(P parser) noexcept : _parser(std::move(parser))
        {
        }

        template<typename T, typename R>
        auto apply(source<T, R>& source) const -> parse_result<apply_result_element_t<Is, P, decltype(source)>...>
        {
            if (auto result = _parser.apply(source))
            {
                return select(std::move(result), std::index_sequence<Is...>());
            }

            return {};
        }

    private:
        P _parser;
    };

    template<std::size_t... Is, typename P>
    constexpr combinator_select<P, Is...> make_select(P parser) noexcept
    {
        return { std::move(parser) };
    }
}
}

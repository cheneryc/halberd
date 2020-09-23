#pragma once

#include "combinator.h"
#include "parse_result.h"
#include "source.h"

#include <tuple> // std::tuple_element
#include <utility> // std::declval

#include <cstddef> // std::size_t


namespace halberd
{
namespace parser
{
    template<typename P, std::size_t Idx>
    class combinator_select : public combinator
    {
        template<typename TSrc>
        using type_parser_result = typename decltype(std::declval<P>().apply(std::declval<TSrc>()))::type;

        template<typename TSrc>
        using type_parser_result_index = typename std::tuple_element<Idx, type_parser_result<TSrc>>::type;

    public:
        constexpr combinator_select(P parser) noexcept : _parser(std::move(parser))
        {
        }

        template<typename T, typename TRef>
        auto apply(source<T, TRef>& source) const -> parse_result<type_parser_result_index<decltype(source)>>
        {
            if (auto result = _parser.apply(source))
            {
                return { std::get<Idx>(result.get()) };
            }

            return { false };
        }

    private:
        P _parser;
    };

    template<std::size_t Idx, typename P>
    constexpr combinator_select<P, Idx> make_select(P parser) noexcept
    {
        return { std::move(parser) };
    }
}
}

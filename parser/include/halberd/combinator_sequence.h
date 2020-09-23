#pragma once

#include "combinator.h"
#include "parse_result.h"
#include "source.h"

#include <tuple> // std::tuple, std::tuple_element, std::tuple_cat, std::make_tuple
#include <utility> // std::index_sequence, std::index_sequence_for, std::declval

#include <cstddef> // std::size_t


namespace halberd
{
namespace parser
{
    template<typename... Ps>
    class combinator_sequence : public combinator
    {
        template<std::size_t Idx>
        using type_parser = typename std::tuple_element<Idx, std::tuple<Ps...>>::type;

        template<std::size_t Idx, typename TSrc>
        using type_parser_result = typename decltype(std::declval<type_parser<Idx>>().apply(std::declval<TSrc>()))::type;

    public:
        constexpr combinator_sequence(Ps... parsers) noexcept : _parsers(std::move(parsers)...)
        {
        }

        template<typename T, typename TRef>
        auto apply(source<T, TRef>& source) const
        {
            return apply_impl(source, std::index_sequence_for<Ps...>());
        }

    private:
        template<typename T, typename TRef, std::size_t Idx>
        auto apply_impl(source<T, TRef>& source, std::index_sequence<Idx>) const -> parse_result<std::tuple<type_parser_result<Idx, decltype(source)>>>
        {
            if (auto result = std::get<Idx>(_parsers).apply(source))
            {
                return { std::make_tuple(result.get()) };
            }

            return { false };
        }

        template<typename T, typename TRef, std::size_t Idx, std::size_t... Is>
        auto apply_impl(source<T, TRef>& source, std::index_sequence<Idx, Is...>) const -> parse_result<std::tuple<type_parser_result<Idx, decltype(source)>, type_parser_result<Is, decltype(source)>...>>
        {
            if (auto result = apply_impl(source, std::index_sequence<Idx>()))
            {
                if (auto result_rec = apply_impl(source, std::index_sequence<Is...>()))
                {
                    return { std::tuple_cat(result.get(), result_rec.get()) };
                }
            }

            return { false };
        }

        std::tuple<Ps...> _parsers;
    };

    template<typename... Ps>
    constexpr combinator_sequence<Ps...> make_sequence(Ps... parsers) noexcept
    {
        return { std::move(parsers)... };
    }
}
}

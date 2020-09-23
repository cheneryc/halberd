#pragma once

#include "combinator.h"
#include "parse_result.h"
#include "source.h"

#include <tuple> // std::tuple
#include <utility> // std::index_sequence, std::index_sequence_for

#include <cstddef> // std::size_t


namespace halberd
{
namespace parser
{
    template<typename... Ps>
    class combinator_choice : public combinator
    {
    public:
        constexpr combinator_choice(Ps... parsers) noexcept : _parsers(std::move(parsers)...)
        {
        }

        template<typename T, typename TRef>
        auto apply(source<T, TRef>& source) const
        {
            return apply_impl(source, std::index_sequence_for<Ps...>());
        }

    private:
        //TODO: need to determine the parse_result's value type from the first parser
        template<typename T, typename TRef, std::size_t Idx>
        auto apply_impl(source<T, TRef>& source, std::index_sequence<Idx>) const -> parse_result<TRef>
        {
            if (auto result = std::get<Idx>(_parsers).apply(source))
            {
                return { result.get() };
            }

            return { false };
        }

        template<typename T, typename TRef, std::size_t Idx, std::size_t... Is>
        auto apply_impl(source<T, TRef>& source, std::index_sequence<Idx, Is...>) const -> parse_result<TRef>
        {
            auto source_cpy = source; // Copy the source before modifying its state

            if (auto result = apply_impl(source, std::index_sequence<Idx>()))
            {
                return result;
            }

            if (auto result = apply_impl(source_cpy, std::index_sequence<Is...>()))
            {
                return result;
            }

            return { false };
        }

        std::tuple<Ps...> _parsers;
    };

    template<typename... Ps>
    constexpr combinator_choice<Ps...> make_choice(Ps... parsers) noexcept
    {
        return { std::move(parsers)... };
    }
}
}

#pragma once

#include "combinator.h"
#include "parse_result.h"
#include "source.h"

#include <tuple> // std::tuple, std::tuple_element_t
#include <utility> // std::index_sequence, std::index_sequence_for

#include <cstddef> // std::size_t


namespace halberd
{
namespace parser
{
    template<typename... Ps>
    class combinator_choice : public combinator
    {
        template<std::size_t Idx>
        using choice_element_t = std::tuple_element_t<Idx, std::tuple<Ps...>>;

        template<std::size_t Idx, typename TSrc>
        using choice_element_result_t = apply_result_t<choice_element_t<Idx>, TSrc>;

    public:
        constexpr combinator_choice(Ps... parsers) noexcept : _parsers(std::move(parsers)...)
        {
        }

        template<typename T, typename R>
        auto apply(source<T, R>& source) const
        {
            return apply_impl(source, std::index_sequence_for<Ps...>());
        }

    private:
        // The result type is the same as the first parser type in Ps
        template<typename T, typename R, std::size_t Idx>
        auto apply_impl(source<T, R>& source, std::index_sequence<Idx>) const -> choice_element_result_t<0U, decltype(source)>
        {
            auto result = std::get<Idx>(_parsers).apply(source);

            if (!result)
            {
                return {};
            }
            else
            {
                try_advance(source);
            }
            
            return result;
            
        }

        // The result type is the same as the first parser type in Ps
        template<typename T, typename R, std::size_t Idx, std::size_t... Is>
        auto apply_impl(source<T, R>& source, std::index_sequence<Idx, Is...>) const -> choice_element_result_t<0U, decltype(source)>
        {
            if (auto result = apply_impl(source, std::index_sequence<Idx>()))
            {
                return result;
            }

            if (auto result = apply_impl(source, std::index_sequence<Is...>()))
            {
                return result;
            }

            return {};
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

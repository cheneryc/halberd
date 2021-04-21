#pragma once

#include "combinator.h"
#include "parse_result.h"
#include "source.h"

#include <tuple> // std::tuple, std::tuple_cat, std::tuple_element_t
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

        template<typename... Ts>
        constexpr combinator_choice(std::tuple<Ts...> parsers) noexcept : _parsers(std::move(parsers))
        {
        }

        template<typename T, typename R>
        auto apply(source<T, R>& source) const
        {
            return apply_impl(source, std::index_sequence_for<Ps...>());
        }

        template<typename... P1s, typename... P2s>
        constexpr friend combinator_choice<P1s..., P2s...> concat(
            combinator_choice<P1s...> choice1,
            combinator_choice<P2s...> choice2) noexcept;

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

    template<typename... Ps>
    constexpr combinator_choice<Ps...> make_choice(std::tuple<Ps...> parsers) noexcept
    {
        return { std::move(parsers) };
    }

    template<typename... Ps>
    constexpr combinator_choice<Ps...> make_choice(combinator_choice<Ps...> choice) noexcept
    {
        return std::move(choice); // Pass through overload lets operator| concatenate combinator_choice parameters
    }

    template<typename... P1s, typename... P2s>
    constexpr combinator_choice<P1s..., P2s...> concat(
        combinator_choice<P1s...> choice1,
        combinator_choice<P2s...> choice2) noexcept
    {
        return make_choice(std::tuple_cat(std::move(choice1._parsers), std::move(choice2._parsers)));
    }
}
}

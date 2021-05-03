#pragma once

#include "combinator.h"
#include "combinator_select.h"
#include "index_tag.h"
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
    class combinator_sequence : public combinator
    {
        template<std::size_t Idx>
        using sequence_element_t = std::tuple_element_t<Idx, std::tuple<Ps...>>;

        template<std::size_t Idx, typename TSrc>
        using sequence_element_result_t = apply_result_t<sequence_element_t<Idx>, TSrc>;

        template<typename... Ts>
        using sequence_result_t = decltype(detail::to_parse_result(meta::concat(detail::parse_result_type_list_v<Ts>...)));

    public:
        constexpr combinator_sequence(Ps... parsers) noexcept : _parsers(std::move(parsers)...)
        {
        }

        template<typename... Ts>
        constexpr combinator_sequence(std::tuple<Ts...> parsers) noexcept : _parsers(std::move(parsers))
        {
        }

        template<typename T, typename R>
        auto apply(source<T, R>& source) const
        {
            return apply_impl(source, std::index_sequence_for<Ps...>());
        }

        template<std::size_t Idx>
        constexpr auto operator[](index_tag<Idx>) && noexcept
        {
            return make_select<Idx>(std::move(*this));
        }

        template<std::size_t... Is>
        constexpr auto operator[](index_sequence_tag<Is...>) && noexcept
        {
            return make_select<Is...>(std::move(*this));
        }

        template<std::size_t Idx>
        constexpr auto operator[](index_tag<Idx>) const & noexcept
        {
            return make_select<Idx>(*this);
        }

        template<std::size_t... Is>
        constexpr auto operator[](index_sequence_tag<Is...>) const & noexcept
        {
            return make_select<Is...>(*this);
        }

        template<typename... P1s, typename... P2s>
        constexpr friend combinator_sequence<P1s..., P2s...> concat(
            combinator_sequence<P1s...> seq1,
            combinator_sequence<P2s...> seq2) noexcept;

    private:
        template<typename T, typename R, std::size_t Idx>
        auto apply_impl(source<T, R>& source, std::index_sequence<Idx>) const
        {
            return std::get<Idx>(_parsers).apply(source);
        }

        template<typename T, typename R, std::size_t Idx, std::size_t... Is>
        auto apply_impl(source<T, R>& source, std::index_sequence<Idx, Is...>) const ->
            sequence_result_t<
                sequence_element_result_t<Idx, decltype(source)>,
                sequence_element_result_t<Is, decltype(source)>...>
        {
            halberd::parser::source<T, R> source_cpy(source, source_flags::none); // Create a copy of the source that cannot advance its buffer

            if (auto result = apply_impl(source_cpy, std::index_sequence<Idx>()))
            {
                if (auto result_rec = apply_impl(source_cpy, std::index_sequence<Is...>()))
                {
                    try_advance(source, source_cpy.get_position());

                    return concat(std::move(result), std::move(result_rec));
                }
            }

            return {};
        }

        std::tuple<Ps...> _parsers;
    };

    template<typename... Ps>
    constexpr combinator_sequence<Ps...> make_sequence(Ps... parsers) noexcept
    {
        return { std::move(parsers)... };
    }

    template<typename... Ps>
    constexpr combinator_sequence<Ps...> make_sequence(std::tuple<Ps...> parsers) noexcept
    {
        return { std::move(parsers) };
    }

    template<typename... Ps>
    constexpr combinator_sequence<Ps...> make_sequence(combinator_sequence<Ps...> seq) noexcept
    {
        return std::move(seq); // Pass through overload lets operator+ concatenate combinator_sequence parameters
    }

    template<typename... P1s, typename... P2s>
    constexpr combinator_sequence<P1s..., P2s...> concat(
        combinator_sequence<P1s...> seq1,
        combinator_sequence<P2s...> seq2) noexcept
    {
        return make_sequence(std::tuple_cat(std::move(seq1._parsers), std::move(seq2._parsers)));
    }
}
}

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

        template<typename T, typename R>
        auto apply(source<T, R>& source) const
        {
            return apply_impl(source, std::index_sequence_for<Ps...>());
        }

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

                    return concat(result, result_rec);
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
}
}

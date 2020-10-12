#pragma once

#include "util/optional.h"

//TODO: make the meta headers a header-only library independent of lexer
#include <../../lexer/include/halberd/meta/type_list.h>
#include <../../lexer/include/halberd/meta/dependent.h>

#include <tuple> // std::tuple, std::tuple_element, std::tuple_cat, std::make_tuple
#include <type_traits> // std::true_type, std::is_same, std::decay_t
#include <utility> // std::forward, std::index_sequence

#include <cstddef> // std::size_t


namespace halberd
{
namespace parser
{
    // Forward declarations

    template<typename... Ts>
    class parse_result;

    template<typename... Ts>
    std::tuple<Ts...> get_as_tuple(parse_result<Ts...>&& result);

    template<typename... Ts>
    std::tuple<Ts...> get_as_tuple(const parse_result<Ts...>& result);

    // Type

    template<typename... Ts>
    class parse_result final
    {
        template<typename... As>
        struct validate_args : std::true_type
        {
        };

        template<typename A>
        struct validate_args<A>
        {
            constexpr static bool value = !std::is_same<parse_result, std::decay_t<A>>::value;
        };

    public:
        constexpr parse_result() noexcept : _values()
        {
        }

        template<typename... As, typename = std::enable_if_t<validate_args<As...>::value>>
        parse_result(As&&... args) : _values(std::make_tuple(std::forward<As>(args)...))
        {
        }

        template<std::size_t N>
        constexpr auto& get()
        {
            return std::get<N>(_values.get());
        }

        template<std::size_t N>
        constexpr const auto& get() const
        {
            return std::get<N>(_values.get());
        }

        friend std::tuple<Ts...> get_as_tuple<>(parse_result&& result);
        friend std::tuple<Ts...> get_as_tuple<>(const parse_result& result);

        constexpr explicit operator bool() const noexcept
        {
            return _values.has_value();
        }

    private:
        util::optional<std::tuple<Ts...>> _values;
    };

    // Specialization for zero template parameters
    template<>
    class parse_result<> final
    {
    public:
        constexpr parse_result(bool is_success) noexcept : _is_success(is_success)
        {
        }

        constexpr explicit operator bool() const noexcept
        {
            return _is_success;
        }

    private:
        const bool _is_success;
    };

    // Specialization for one template parameter
    template<typename T>
    class parse_result<T> final
    {
    public:
        constexpr parse_result() noexcept : _value()
        {
        }

        parse_result(T&& value) : _value(std::move(value))
        {
        }

        parse_result(const T& value) : _value(value)
        {
        }

        constexpr auto& get()
        {
            return _value.get();
        }

        constexpr const auto& get() const
        {
            return _value.get();
        }

        constexpr explicit operator bool() const noexcept
        {
            return _value.has_value();
        }

    private:
        util::optional<T> _value;
    };

    // Traits

    template<std::size_t Idx, typename T>
    struct parse_result_element
    {
        static_assert(meta::dependent_false<T>, "parse_result_element: template parameter T must be parse_result");
    };

    template<std::size_t Idx, typename... Ts>
    struct parse_result_element<Idx, parse_result<Ts...>> : std::tuple_element<Idx, std::tuple<Ts...>>
    {
        static_assert(Idx < sizeof...(Ts), "parse_result_element: template parameter Idx must be less than the number of elements in variadic template parameter Ts");
    };

    template<std::size_t Idx, typename T>
    using parse_result_element_t = typename parse_result_element<Idx, T>::type;

    // Variadic operations

    namespace detail
    {
        template<typename... Ts, std::size_t... Is>
        parse_result<Ts...> parse_result_from_tuple(std::tuple<Ts...>&& values, std::index_sequence<Is...>)
        {
            return { std::move(std::get<Is>(values))... };
        }
    }

    template<typename... T1s, typename... T2s>
    auto concat(parse_result<T1s...> result1, parse_result<T2s...> result2) -> parse_result<T1s..., T2s...>
    {
        auto values1 = get_as_tuple(std::move(result1));
        auto values2 = get_as_tuple(std::move(result2));
        auto values  = std::tuple_cat(std::move(values1), std::move(values2));

        return detail::parse_result_from_tuple(std::move(values), std::index_sequence_for<T1s..., T2s...>());
    }

    // Converters

    namespace detail
    {
        template<typename... Ts>
        constexpr parse_result<Ts...> to_parse_result(meta::type_list<Ts...>)
        {
            return {};
        }

        template<typename T>
        struct parse_result_type_list
        {
            static_assert(meta::dependent_false<T>, "parse_result_type_list: template parameter T must be parse_result");
        };

        template<typename... Ts>
        struct parse_result_type_list<parse_result<Ts...>>
        {
            using type = meta::type_list<Ts...>;
        };

        template<typename T>
        constexpr typename parse_result_type_list<T>::type parse_result_type_list_v;
    }

    template<typename... Ts>
    std::tuple<Ts...> get_as_tuple(parse_result<Ts...>&& result)
    {
        return std::move(result._values.get());
    }

    template<typename... Ts>
    std::tuple<Ts...> get_as_tuple(const parse_result<Ts...>& result)
    {
        return result._values.get();
    }

    // Explicit specialization for zero template parameters
    template<>
    inline std::tuple<> get_as_tuple<>(parse_result<>&& result)
    {
        return {};
    }

    // Explicit specialization for zero template parameters
    template<>
    inline std::tuple<> get_as_tuple<>(const parse_result<>& result)
    {
        return {};
    }

    // Function overload (not a specialization) for one template parameter
    template<typename T>
    std::tuple<T> get_as_tuple(parse_result<T>&& result)
    {
        return { std::move(result.get()) };
    }

    // Function overload (not a specialization) for one template parameter
    template<typename T>
    std::tuple<T> get_as_tuple(const parse_result<T>& result)
    {
        return { result.get() };
    }
}
}

#pragma once

#include <utility> // std::forward
#include <type_traits> // std::remove_cv_t, std::remove_reference_t


namespace halberd
{
namespace util
{
    namespace detail
    {
        // Combines multiple invocable types, such as lambdas or function objects,
        // into a single invocable type with multiple function call operators.
        template<typename Fn, typename... Fs>
        struct overload_invocable : overload_invocable<Fn>, overload_invocable<Fs...>
        {
            template<typename A, typename... As>
            overload_invocable(A&& arg, As&&... args) :
                overload_invocable<Fn>(std::forward<A>(arg)),
                overload_invocable<Fs...>(std::forward<As>(args)...)
            {
            }

            using overload_invocable<Fn>::operator();
            using overload_invocable<Fs...>::operator();
        };

        template<typename Fn>
        struct overload_invocable<Fn> : Fn
        {
            template<typename A>
            overload_invocable(A&& arg) : Fn(std::forward<A>(arg))
            {
            }

            using Fn::operator();
        };

        template<typename Fn>
        using overloadable_t = std::remove_cv_t<std::remove_reference_t<Fn>>;
    }

    template<typename... Fs>
    auto overload(Fs&&... funcs) -> detail::overload_invocable<detail::overloadable_t<Fs>...>
    {
        return {
            std::forward<Fs>(funcs)...
        };
    }
}
}

#pragma once

#include "state_index.h"

#include "symbol_set.h"

#include "meta/dependent.h"

#include <limits> // std::numeric_limits


namespace halberd
{
namespace lexer
{
    // State types

    template<typename TSym, state_index_t IdxFrom, state_index_t IdxTo, typename TSymSet>
    struct state_transition
    {
        static_assert(meta::dependent_false<TSymSet>, "state_transition: template parameter TSymSet must be basic_symbol_set");
    };

    template<typename TSym, state_index_t IdxFrom, state_index_t IdxTo, TSym... Symbols>
    struct state_transition<TSym, IdxFrom, IdxTo, basic_symbol_set<TSym, Symbols...>>
    {
    };

    namespace detail
    {
        template<typename TSymSet, typename TIndex>
        struct state_transition_desc
        {
            static_assert(meta::dependent_false<TSymSet, TIndex>, "state_transition_desc: template parameters TSymSet must be basic_symbol_set and TIndex must be state_index_tag or state_index_self_tag");
        };

        template<typename TSym, state_index_t IdxTo, TSym... Symbols>
        struct state_transition_desc<basic_symbol_set<TSym, Symbols...>, state_index_tag<IdxTo>>
        {
        };

        template<typename TSym, TSym... Symbols>
        struct state_transition_desc<basic_symbol_set<TSym, Symbols...>, state_index_self_tag>
        {
        };

        template<typename TSym, state_index_t IdxTo, TSym... Symbols>
        constexpr auto make_transition_desc(basic_symbol_set<TSym, Symbols...>, state_index_tag<IdxTo>) noexcept
        {
            return state_transition_desc<basic_symbol_set<TSym, Symbols...>, state_index_tag<IdxTo>>();
        }

        template<typename TSym, TSym... Symbols>
        constexpr auto make_transition_desc(basic_symbol_set<TSym, Symbols...>, state_index_self_tag) noexcept
        {
            return state_transition_desc<basic_symbol_set<TSym, Symbols...>, state_index_self_tag>();
        }
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    struct state
    {
        //TODO: add asserts that TTrans contains only state_transition(s)
        //TODO: ensure that the transitions are disjoint (i.e. transition symbol sets have no common symbols) - a state 'alphabet' symbol set type might be one way to do this

        template<TSym... Symbols, state_index_t IdxTo>
        constexpr auto operator+(detail::state_transition_desc<basic_symbol_set<TSym, Symbols...>, state_index_tag<IdxTo>>) const noexcept
        {
            return state<TSym, Idx, B, TTrans..., state_transition<TSym, Idx, IdxTo, basic_symbol_set<TSym, Symbols...>>>();
        }

        template<TSym... Symbols>
        constexpr auto operator+(detail::state_transition_desc<basic_symbol_set<TSym, Symbols...>, state_index_self_tag>) const noexcept
        {
            return state<TSym, Idx, B, TTrans..., state_transition<TSym, Idx, Idx, basic_symbol_set<TSym, Symbols...>>>();
        }
    };

    template<typename TSym, state_index_t IdxStart, typename... TStates>
    struct state_machine
    {
        //TODO: add asserts that TStates contains only state(s) (with valid indices starting at zero and incrementing by one)
        //TODO: ensure that IdxStart is a valid state (assert that IdxStart < sizeof...(TStates))

        template<state_index_t Idx, bool B, typename... TTrans>
        constexpr auto operator+(state<TSym, Idx, B, TTrans...>) const noexcept
        {
            return state_machine<TSym, IdxStart, TStates..., state<TSym, Idx, B, TTrans...>>();
        }
    };

    // Variables

    template<typename TSym, state_index_t IdxFrom, state_index_t IdxTo, typename TSymSet>
    constexpr state_transition<TSym, IdxFrom, IdxTo, TSymSet> state_transition_v;

    template<typename TSym, state_index_t Idx, bool B = false, typename... TTrans>
    constexpr state<TSym, Idx, B, TTrans...> state_v;

    template<typename TSym, state_index_t IdxStart = 0U, typename... TStates>
    constexpr state_machine<TSym, IdxStart, TStates...> state_machine_v;

    // Functions

    template<typename TSymbols, state_index_t IdxTo>
    constexpr auto make_transition(TSymbols symbols, state_index_tag<IdxTo> index) noexcept
    {
        return detail::make_transition_desc(make_symbol_set(symbols), index);
    }

    template<typename TSymbols>
    constexpr auto make_transition_self(TSymbols symbols) noexcept
    {
        return detail::make_transition_desc(make_symbol_set(symbols), state_index_self_tag());
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    constexpr state_index_t get_index(state<TSym, Idx, B, TTrans...>) noexcept
    {
        return Idx;
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    constexpr auto get_index_tag(state<TSym, Idx, B, TTrans...>) noexcept
    {
        return state_index_tag<Idx>();
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    constexpr state_index_t next_index(state<TSym, Idx, B, TTrans...>) noexcept
    {
        static_assert(Idx < std::numeric_limits<state_index_t>::max(), "next_index: template parameter Idx is the maximum possible value");
        return Idx + 1U;
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    constexpr auto next_index_tag(state<TSym, Idx, B, TTrans...>) noexcept
    {
        static_assert(Idx < std::numeric_limits<state_index_t>::max(), "next_index_tag: template parameter Idx is the maximum possible value");
        return state_index_tag<Idx + 1U>();
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    constexpr state_index_t prev_index(state<TSym, Idx, B, TTrans...>) noexcept
    {
        static_assert(Idx > std::numeric_limits<state_index_t>::min(), "prev_index: template parameter Idx is the minimum possible value");
        return Idx - 1U;
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    constexpr auto prev_index_tag(state<TSym, Idx, B, TTrans...>) noexcept
    {
        static_assert(Idx > std::numeric_limits<state_index_t>::min(), "prev_index_tag: template parameter Idx is the minimum possible value");
        return state_index_tag<Idx - 1U>();
    }

    template<typename TSym, state_index_t IdxFrom, state_index_t IdxTo, TSym... Symbols>
    constexpr state_index_t get_index_from(state_transition<TSym, IdxFrom, IdxTo, basic_symbol_set<TSym, Symbols...>>) noexcept
    {
        return IdxFrom;
    }

    template<typename TSym, state_index_t IdxFrom, state_index_t IdxTo, TSym... Symbols>
    constexpr state_index_t get_index_to(state_transition<TSym, IdxFrom, IdxTo, basic_symbol_set<TSym, Symbols...>>) noexcept
    {
        return IdxTo;
    }
}
}

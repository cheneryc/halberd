#pragma once

#include "state_index.h"

#include "symbol_set.h"

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

    template<typename TSym, state_index_t IdxFrom, state_index_t IdxTo, TSym... SymSet>
    struct state_transition<TSym, IdxFrom, IdxTo, basic_symbol_set<TSym, SymSet...>>
    {
    };

    namespace detail
    {
        template<typename TSymSet, typename TIndex>
        struct state_transition_desc
        {
            static_assert(meta::dependent_false<TSymSet, TIndex>, "state_transition_desc: template parameters TSymSet must be basic_symbol_set and TIndex must be state_index_tag or state_index_self_tag");
        };

        template<typename TSym, state_index_t IdxTo, TSym... SymSet>
        struct state_transition_desc<basic_symbol_set<TSym, SymSet...>, state_index_tag<IdxTo>>
        {
        };

        template<typename TSym, TSym... SymSet>
        struct state_transition_desc<basic_symbol_set<TSym, SymSet...>, state_index_self_tag>
        {
        };
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    struct state
    {
        //TODO: add asserts that TTrans contains only state_transition(s)
        //TODO: ensure that the transitions are disjoint (i.e. transition symbol sets have no common symbols) - a state 'alphabet' symbol set type might be one way to do this

        template<TSym... SymSet, state_index_t IdxTo>
        constexpr auto operator+(const detail::state_transition_desc<basic_symbol_set<TSym, SymSet...>, state_index_tag<IdxTo>>&) const noexcept
        {
            return state<TSym, Idx, B, TTrans..., state_transition<TSym, Idx, IdxTo, basic_symbol_set<TSym, SymSet...>>>();
        }

        template<TSym... SymSet>
        constexpr auto operator+(const detail::state_transition_desc<basic_symbol_set<TSym, SymSet...>, state_index_self_tag>&) const noexcept
        {
            return state<TSym, Idx, B, TTrans..., state_transition<TSym, Idx, Idx, basic_symbol_set<TSym, SymSet...>>>();
        }
    };

    template<typename TSym, state_index_t IdxStart, typename... TStates>
    struct state_machine
    {
        //TODO: add asserts that TStates contains only state(s) (with valid indices starting at zero and incrementing by one)
        //TODO: ensure that IdxStart is a valid state (assert that IdxStart < sizeof...(TStates))

        template<state_index_t Idx, bool B, typename... TTrans>
        constexpr auto operator+(const state<TSym, Idx, B, TTrans...>&) const noexcept
        {
            return state_machine<TSym, IdxStart, TStates..., state<TSym, Idx, B, TTrans...>>();
        }
    };

    // Variables

    template<typename TSym, state_index_t Idx, bool B = false, typename... TTrans>
    constexpr state<TSym, Idx, B, TTrans...> state_v;

    template<typename TSym, state_index_t IdxStart = 0U, typename... TStates>
    constexpr state_machine<TSym, IdxStart, TStates...> state_machine_v;

    // Functions

    template<typename TSymSet, state_index_t IdxTo>
    constexpr auto make_transition(const TSymSet&, state_index_tag<IdxTo>) noexcept
    {
        return detail::state_transition_desc<typename make_symbol_set<TSymSet>::type, state_index_tag<IdxTo>>();
    }

    template<typename TSymSet>
    constexpr auto make_transition_self(const TSymSet&) noexcept
    {
        return detail::state_transition_desc<typename make_symbol_set<TSymSet>::type, state_index_self_tag>();
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    constexpr state_index_t get_index(const state<TSym, Idx, B, TTrans...>&) noexcept
    {
        return Idx;
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    constexpr auto get_index_tag(const state<TSym, Idx, B, TTrans...>&) noexcept
    {
        return state_index_tag<Idx>();
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    constexpr state_index_t next_index(const state<TSym, Idx, B, TTrans...>&) noexcept
    {
        static_assert(Idx < std::numeric_limits<state_index_t>::max(), "next_index: template parameter Idx is the maximum possible value");
        return Idx + 1U;
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    constexpr auto next_index_tag(const state<TSym, Idx, B, TTrans...>&) noexcept
    {
        static_assert(Idx < std::numeric_limits<state_index_t>::max(), "next_index_tag: template parameter Idx is the maximum possible value");
        return state_index_tag<Idx + 1U>();
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    constexpr state_index_t prev_index(const state<TSym, Idx, B, TTrans...>&) noexcept
    {
        static_assert(Idx > std::numeric_limits<state_index_t>::min(), "prev_index: template parameter Idx is the minimum possible value");
        return Idx - 1U;
    }

    template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
    constexpr auto prev_index_tag(const state<TSym, Idx, B, TTrans...>&) noexcept
    {
        static_assert(Idx > std::numeric_limits<state_index_t>::min(), "prev_index_tag: template parameter Idx is the minimum possible value");
        return state_index_tag<Idx - 1U>();
    }
}
}

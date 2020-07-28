#pragma once

#include "state_index.h"
#include "state_machine.h"

#include "symbol_set.h"
#include "symbol_set_array.h"

#include "meta/dependent.h"

#include <iterator> // std::begin, std::end


namespace halberd
{
namespace lexer
{
    namespace detail
    {
        template<typename T>
        class span
        {
        public:
            using value_type = T;

            constexpr span() noexcept :
                _pbegin(nullptr),
                _pend(nullptr)
            {
            }

            template<std::size_t N>
            constexpr span(const value_type (&array)[N]) noexcept :
                _pbegin(std::begin(array)),
                _pend(std::end(array))
            {
            }

            using const_iterator = const value_type*;

            constexpr const_iterator begin() const noexcept
            {
                return _pbegin;
            }

            constexpr const_iterator end() const noexcept
            {
                return _pend;
            }

            using const_reference = const value_type&;

            constexpr const_reference operator[](std::size_t idx) const
            {
                return *(_pbegin + idx);
            }

        private:
            const_iterator const _pbegin;
            const_iterator const _pend;
        };
    }

    // Array & View types

    template<typename TSym>
    struct state_transition_view : public detail::span<TSym>
    {
        template<std::size_t N>
        constexpr state_transition_view(const TSym (&array)[N], state_index_t idx_from, state_index_t idx_to) noexcept :
            detail::span<TSym>(array),
            idx_from(idx_from),
            idx_to(idx_to)
        {
        }

        const state_index_t idx_from;
        const state_index_t idx_to;
    };

    template<typename TSym, state_index_t IdxFrom, state_index_t IdxTo, TSym... Symbols>
    constexpr state_transition_view<TSym> to_state_transition_view(state_transition<TSym, IdxFrom, IdxTo, basic_symbol_set<TSym, Symbols...>>) noexcept
    {
        return { basic_symbol_set_array<basic_symbol_set<TSym, Symbols...>>::values, IdxFrom, IdxTo };
    }

    template<typename T>
    struct state_array
    {
        static_assert(meta::dependent_false<T>, "state_array: template parameter T must be state");
    };

    template<typename TSym, state_index_t Idx, bool B, typename TTag, TTag Tag, typename... TTrans>
    struct state_array<state<TSym, Idx, B, TTag, Tag, TTrans...>>
    {
        using value_type = state_transition_view<TSym>;
        using const_iterator = const value_type*;

        constexpr const_iterator begin() const noexcept
        {
            return std::begin(values);
        }

        constexpr const_iterator end() const noexcept
        {
            return std::end(values);
        }

        static constexpr value_type values[] = { to_state_transition_view(TTrans())... };
    };

    template<typename TSym, state_index_t Idx, bool B, typename TTag, TTag Tag, typename... TTrans>
    constexpr state_transition_view<TSym> state_array<state<TSym, Idx, B, TTag, Tag, TTrans...>>::values[]; // Out-of-line static member definition

    template<typename TSym, typename TTag = state_tag>
    struct state_view : public detail::span<state_transition_view<TSym>>
    {
        constexpr state_view(state_index_t idx, bool is_accept_state, TTag tag) noexcept : detail::span<state_transition_view<TSym>>(), idx(idx), tag(tag), is_accept_state(is_accept_state)
        {
        }

        template<std::size_t N>
        constexpr state_view(const state_transition_view<TSym> (&array)[N], state_index_t idx, bool is_accept_state, TTag tag) noexcept : detail::span<state_transition_view<TSym>>(array), idx(idx), tag(tag), is_accept_state(is_accept_state)
        {
        }

        const state_index_t idx;
        const TTag tag;
        const bool is_accept_state;
    };

    template<typename TSym, state_index_t Idx, bool B, typename TTag, TTag Tag>
    constexpr state_view<TSym, TTag> to_state_view(state<TSym, Idx, B, TTag, Tag>) noexcept
    {
        return { Idx, B, Tag };
    }

    template<typename TSym, state_index_t Idx, bool B, typename TTag, TTag Tag, typename... TTrans>
    constexpr state_view<TSym, TTag> to_state_view(state<TSym, Idx, B, TTag, Tag, TTrans...>) noexcept
    {
        return { state_array<state<TSym, Idx, B, TTag, Tag, TTrans...>>::values, Idx, B, Tag };
    }

    template<typename T>
    struct state_machine_array
    {
        static_assert(meta::dependent_false<T>, "state_machine_array: template parameter T must be state_machine");
    };

    template<typename TSym, state_index_t IdxStart, typename TTag, typename... TStates>
    struct state_machine_array<state_machine<TSym, IdxStart, TTag, TStates...>>
    {
        using value_type = state_view<TSym, TTag>;
        using const_iterator = const value_type*;

        constexpr const_iterator begin() const noexcept
        {
            return std::begin(values);
        }

        constexpr const_iterator end() const noexcept
        {
            return std::end(values);
        }

        static constexpr value_type values[] = { to_state_view(TStates())... };
    };

    template<typename TSym, state_index_t IdxStart, typename TTag, typename... TStates>
    constexpr state_view<TSym, TTag> state_machine_array<state_machine<TSym, IdxStart, TTag, TStates...>>::values[]; // Out-of-line static member definition

    template<typename TSym, typename TTag = state_tag>
    struct state_machine_view : public detail::span<state_view<TSym, TTag>>
    {
        template<std::size_t N>
        constexpr state_machine_view(const state_view<TSym, TTag> (&array)[N], state_index_t idx_start) noexcept : detail::span<state_view<TSym, TTag>>(array), idx_start(idx_start)
        {
        }

        const state_index_t idx_start;
    };

    template<typename TSym, state_index_t IdxStart, typename TTag, typename... TStates>
    constexpr state_machine_view<TSym, TTag> to_state_machine_view(state_machine<TSym, IdxStart, TTag, TStates...>) noexcept
    {
        return { state_machine_array<state_machine<TSym, IdxStart, TTag, TStates...>>::values, IdxStart };
    }
}
}

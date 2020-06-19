#pragma once

#include "state_index.h"
#include "state_machine.h"

#include "meta/dependent.h"
#include "meta/type_list.h"
#include "meta/type_wrapper.h"
#include "meta/value_set.h"
#include "meta/value_list.h"
#include "meta/value_list_transform.h"
#include "meta/value_wrapper.h"


namespace halberd
{
namespace lexer
{
    // Operators

    namespace detail
    {
        template<typename TSym, state_index_t Idx, bool B>
        constexpr auto get_alphabet(state<TSym, Idx, B>) noexcept
        {
            return meta::value_set_v<TSym>;
        }

        template<typename TSym, state_index_t Idx, bool B, state_index_t IdxTo, TSym... Symbols, typename... TTrans>
        constexpr auto get_alphabet(state<TSym, Idx, B, state_transition<TSym, Idx, IdxTo, basic_symbol_set<TSym, Symbols...>>, TTrans...>) noexcept
        {
            return meta::value_set_v<TSym, Symbols...> | get_alphabet(state_v<TSym, Idx, B, TTrans...>);
        }

        //C++17: investigate if this class can be replaced with a constexpr lambda
        template<typename TSym, TSym Sym>
        struct fn_find_transition
        {
            template<state_index_t IdxFrom, state_index_t IdxTo, TSym... Symbols>
            constexpr bool operator()(meta::type_wrapper<state_transition<TSym, IdxFrom, IdxTo, basic_symbol_set<TSym, Symbols...>>>) const noexcept
            {
                return meta::contains(meta::value_list_v<TSym, Symbols...>, Sym);
            }
        };

        template<typename TSym, state_index_t Idx, bool B, typename... TTrans, TSym Sym>
        static constexpr state_index_t get_transition_index(state<TSym, Idx, B, TTrans...>, basic_symbol<TSym, Sym>, state_index_t idx_end) noexcept
        {
            using state_transition_invalid = state_transition<TSym, state_index_invalid, state_index_invalid, basic_symbol_set<TSym, TSym{}>>;

            constexpr auto trans_find = meta::find_if(
                meta::type_list_v<TTrans...>,
                meta::type_wrapper_v<state_transition_invalid>,
                fn_find_transition<TSym, Sym>());

            const auto idx_to = get_index_to(meta::unwrap(trans_find));
            const auto idx = idx_to != state_index_invalid ? idx_to : idx_end;

            return idx;
        }

        template<typename TSym, state_index_t Idx, bool B, typename... TTrans>
        constexpr auto make_state(meta::type_list<TTrans...>) noexcept
        {
            return state_v<TSym, Idx, B, TTrans...>;
        }

        template<typename TSym, state_index_t IdxStart, typename... TStates>
        constexpr auto make_state_machine(meta::type_list<TStates...>) noexcept
        {
            return state_machine_v<TSym, IdxStart, TStates...>;
        }

        template<typename T1, typename T2>
        struct fn_create_state
        {
            static_assert(meta::dependent_false<T1, T2>, "detail::fn_create_state: template parameters T1 and T2 must be state_machine");
        };
        
        template<typename TSym, state_index_t IdxStart1, typename... TStates1, state_index_t IdxStart2, typename... TStates2>
        struct fn_create_state<state_machine<TSym, IdxStart1, TStates1...>, state_machine<TSym, IdxStart2, TStates2...>>
        {
            static constexpr state_index_t get_combined_idx(state_index_t idx1, state_index_t idx2)
            {
                return (idx1 * (sizeof...(TStates2) + 1U)) + idx2; // An index is reserved in each row for [s1, e] states along with an extra row for [e, s2] states
            }

            template<typename T1, typename T2>
            struct fn_create_state_transition
            {
                static_assert(meta::dependent_false<T1, T2>, "detail::fn_create_state_transition: template parameters T1 and T2 must be state");
            };

            template<state_index_t Idx1, bool B1, typename... TTrans1, state_index_t Idx2, bool B2, typename... TTrans2>
            struct fn_create_state_transition<state<TSym, Idx1, B1, TTrans1...>, state<TSym, Idx2, B2, TTrans2...>>
            {
                //TODO: filter out transitions to [e, e]
                template<TSym Sym>
                constexpr auto operator()(meta::value_wrapper<TSym, Sym>) const noexcept
                {
                    constexpr state_index_t idx_from = get_combined_idx(Idx1, Idx2);
                    constexpr state_index_t idx_to   = get_combined_idx(
                        get_transition_index(state_v<TSym, Idx1, B1, TTrans1...>, basic_symbol_v<TSym, Sym>, sizeof...(TStates1)),
                        get_transition_index(state_v<TSym, Idx2, B2, TTrans2...>, basic_symbol_v<TSym, Sym>, sizeof...(TStates2)));

                    return meta::wrap(state_transition_v<TSym, idx_from, idx_to, basic_symbol_set<TSym, Sym>>);
                }
            };

            template<state_index_t Idx1, bool B1, typename... TTrans1, state_index_t Idx2, bool B2, typename... TTrans2>
            constexpr auto operator()(
                meta::type_wrapper<state<TSym, Idx1, B1, TTrans1...>>,
                meta::type_wrapper<state<TSym, Idx2, B2, TTrans2...>>) const noexcept
            {
                constexpr auto state_alphabet =
                    get_alphabet(state_v<TSym, Idx1, B1, TTrans1...>) |
                    get_alphabet(state_v<TSym, Idx2, B2, TTrans2...>);

                using fn_create_state_transition = fn_create_state_transition<
                    state<TSym, Idx1, B1, TTrans1...>,
                    state<TSym, Idx2, B2, TTrans2...>>;

                constexpr state_index_t idx = get_combined_idx(Idx1, Idx2);

                //TODO: merge transitions that go to the same 'to' state (currently each symbol results in a separate transition)
                constexpr auto state_transition_list = meta::transform_values(meta::set_to_list(state_alphabet), fn_create_state_transition());
                constexpr auto state_transition = make_state<TSym, idx, B1 || B2>(state_transition_list);

                return meta::wrap(state_transition);
            }
        };
    }

    template<typename TSym, state_index_t IdxStart1, typename... TStates1, state_index_t IdxStart2, typename... TStates2>
    constexpr auto operator|(state_machine<TSym, IdxStart1, TStates1...>, state_machine<TSym, IdxStart2, TStates2...>) noexcept
    {
        // Append an empty state to the end of each list
        constexpr auto list1 = meta::type_list_v<TStates1..., state<TSym, sizeof...(TStates1), false>>;
        constexpr auto list2 = meta::type_list_v<TStates2..., state<TSym, sizeof...(TStates2), false>>;

        using fn_create_state = detail::fn_create_state<
            state_machine<TSym, IdxStart1, TStates1...>,
            state_machine<TSym, IdxStart2, TStates2...>>;

        constexpr state_index_t idx_start = fn_create_state::get_combined_idx(IdxStart1, IdxStart2);

        constexpr auto state_list = meta::cartesian_product(list1, list2, fn_create_state());
        constexpr auto state_machine = detail::make_state_machine<TSym, idx_start>(state_list);

        return state_machine;
    }
}
}

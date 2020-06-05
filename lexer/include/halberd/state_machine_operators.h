#pragma once

#include "state_index.h"
#include "state_machine.h"

#include "meta/dependent.h"
#include "meta/type_list.h"
#include "meta/type_wrapper.h"
#include "meta/value_set.h"


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

        /*template<typename T1, typename T2>
        struct fn_combine
        {
            static_assert(meta::dependent_false<T1, T2>, "detail::fn_combine: template parameters T1 and T2 must be state");
        };*/
        
        //template<typename TSym, state_index_t Idx1, bool B1, typename... TTrans1, state_index_t Idx2, bool B2, typename... TTrans2>
        struct fn_combine/*<state<TSym, Idx1, B1, TTrans1...>, state<TSym, Idx2, B2, TTrans2...>>*/
        {
            template<typename TSym, state_index_t Idx1, bool B1, typename... TTrans1, state_index_t Idx2, bool B2, typename... TTrans2>
            constexpr auto operator()(
                meta::type_wrapper<state<TSym, Idx1, B1, TTrans1...>>,
                meta::type_wrapper<state<TSym, Idx2, B2, TTrans2...>>) const noexcept
            {
                constexpr auto state_alphabet =
                    get_alphabet(state_v<TSym, Idx1, B1, TTrans1...>) |
                    get_alphabet(state_v<TSym, Idx2, B2, TTrans2...>); 

                return meta::wrap(state_alphabet); //TODO: remove, just needed to get this compiling for the time being
            }

            /*template<TSym Sym>
            struct create_state_transition_fn
            {
                //TODO: filter out transitions to [e, e]
                using type = basic_state_transition<TSym,
                    get_combined_idx(Idx1, Idx2),
                    get_combined_idx(
                        s1_t::template get_transition_state_index<Sym>(sizeof...(TStates1)),
                        s2_t::template get_transition_state_index<Sym>(sizeof...(TStates2))),
                    basic_symbol_set<TSym, Sym>>;
            };

            //TODO: merge transitions that go to the same 'to' state (currently each symbol results in a separate transition)
            using new_state_transition_list = typename list_transform_value<TSym, alphabet_t, create_state_transition_fn>::type;*/
        };
    }

    template<typename TSym, state_index_t IdxStart1, typename... TStates1, state_index_t IdxStart2, typename... TStates2>
    constexpr auto operator|(const state_machine<TSym, IdxStart1, TStates1...>&, const state_machine<TSym, IdxStart2, TStates2...>&) noexcept
    {
        // Append an empty state to the end of each list
        constexpr auto list1 = meta::type_list_v<TStates1..., state<TSym, sizeof...(TStates1), false>>;
        constexpr auto list2 = meta::type_list_v<TStates2..., state<TSym, sizeof...(TStates2), false>>;

        //TODO: convert the returned list of states back into a state_machine
        return meta::cartesian_product(list1, list2, detail::fn_combine());
    }
}
}

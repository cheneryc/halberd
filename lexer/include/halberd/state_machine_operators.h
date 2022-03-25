#pragma once

#include "state_machine.h"
#include "state_index.h"
#include "symbol_set.h"
#include "symbol_set_operators.h"

// halberd::meta
#include <halberd/meta/dependent.h>
#include <halberd/meta/static_if.h>
#include <halberd/meta/type_list.h>
#include <halberd/meta/type_list_transform.h>
#include <halberd/meta/type_wrapper.h>
#include <halberd/meta/value_set.h>
#include <halberd/meta/value_list.h>
#include <halberd/meta/value_list_transform.h>
#include <halberd/meta/value_wrapper.h>


namespace halberd
{
namespace lexer
{
    // Operators

    namespace detail
    {
        template<typename TSym, state_index_t Idx, typename TTag, TTag Tag, bool B>
        constexpr auto get_alphabet(state<TSym, Idx, B, TTag, Tag>) noexcept
        {
            return meta::value_set_v<TSym>;
        }

        template<typename TSym, state_index_t Idx, bool B, typename TTag, TTag Tag, state_index_t IdxTo, TSym... Symbols, typename... TTrans>
        constexpr auto get_alphabet(state<TSym, Idx, B, TTag, Tag, state_transition<TSym, Idx, IdxTo, basic_symbol_set<TSym, Symbols...>>, TTrans...>) noexcept
        {
            return meta::value_set_v<TSym, Symbols...> | get_alphabet(state_v<TSym, Idx, B, TTag, Tag, TTrans...>);
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

        template<typename TSym, state_index_t Idx, bool B, typename TTag, TTag Tag, typename... TTrans, TSym Sym>
        constexpr state_index_t get_transition_index(state<TSym, Idx, B, TTag, Tag, TTrans...>, basic_symbol<TSym, Sym>, state_index_t idx_end) noexcept
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

        template<typename TSym, state_index_t IdxFrom, state_index_t IdxTo, TSym... Symbols>
        constexpr auto make_state_transition(basic_symbol_set<TSym, Symbols...>)
        {
            return state_transition_v<TSym, IdxFrom, IdxTo, basic_symbol_set<TSym, Symbols...>>;
        }

        template<typename TSym, state_index_t Idx, bool B, typename TTag, TTag Tag, typename... TTrans>
        constexpr auto make_state(meta::type_list<TTrans...>) noexcept
        {
            return state_v<TSym, Idx, B, TTag, Tag, TTrans...>;
        }

        template<typename TSym, state_index_t IdxStart, typename TTag, typename... TStates>
        constexpr auto make_state_machine(meta::type_list<TStates...>) noexcept
        {
            return state_machine_v<TSym, IdxStart, TTag, TStates...>;
        }

        template<typename TSym, state_index_t Idx, TSym... SymbolsMerge>
        struct fn_merge_transform
        {
            template<state_index_t IdxFrom, state_index_t IdxTo, TSym... Symbols>
            constexpr auto operator()(meta::type_wrapper<state_transition<TSym, IdxFrom, IdxTo, basic_symbol_set<TSym, Symbols...>>>) const noexcept
            {
                constexpr auto symbols = meta::static_if<Idx == IdxTo>(
                    basic_symbol_set_v<TSym, Symbols...> | basic_symbol_set_v<TSym, SymbolsMerge...>,
                    basic_symbol_set_v<TSym, Symbols...>);

                return meta::wrap(make_state_transition<TSym, IdxFrom, IdxTo>(symbols));
            }
        };

        template<typename TSym, state_index_t Idx>
        struct fn_merge_accumulate
        {
            template<typename... TTrans, state_index_t IdxFrom, state_index_t IdxTo, TSym... Symbols>
            constexpr auto operator()(meta::type_wrapper<meta::type_list<TTrans...>>, meta::type_wrapper<state_transition<TSym, IdxFrom, IdxTo, basic_symbol_set<TSym, Symbols...>>>) const noexcept
            {
                static_assert(Idx == IdxFrom, "detail::fn_merge_accumulate::operator(): state transition indices must match");

                constexpr auto transition = state_transition_v<TSym, IdxFrom, IdxTo, basic_symbol_set<TSym, Symbols...>>;
                constexpr auto transitions_merged = meta::type_list_v<TTrans...>;
                constexpr auto transitions_merged_updated = meta::transform(transitions_merged, fn_merge_transform<TSym, IdxTo, Symbols...>());
                constexpr auto transitions_merged_appended = meta::append(transitions_merged, meta::wrap(transition));

                // If the type lists are different then this transition's symbols
                // were merged with an existing transition during the transform
                constexpr bool is_merged = transitions_merged != transitions_merged_updated;

                constexpr auto result = meta::static_if<is_merged>(
                    transitions_merged_updated,
                    transitions_merged_appended);

                return meta::wrap(result);
            }
        };

        template<typename T1, typename T2>
        struct fn_create_state
        {
            static_assert(meta::dependent_false<T1, T2>, "detail::fn_create_state: template parameters T1 and T2 must be state_machine");
        };

        template<typename TSym, typename TTag, state_index_t IdxStart1, typename... TStates1, state_index_t IdxStart2, typename... TStates2>
        struct fn_create_state<state_machine<TSym, IdxStart1, TTag, TStates1...>, state_machine<TSym, IdxStart2, TTag, TStates2...>>
        {
            static constexpr state_index_t get_combined_index(state_index_t idx1, state_index_t idx2) noexcept
            {
                return (idx1 * (sizeof...(TStates2) + 1U)) + idx2; // An index is reserved in each row for [s1, e] states along with an extra row for [e, s2] states
            }

            static constexpr TTag get_combined_tag(bool is_accept_state_one, TTag tag1,
                                                   bool is_accept_state_two, TTag tag2) noexcept
            {
                if (is_accept_state_one && is_accept_state_two)
                {
                    return tag1 | tag2;
                }
                else if (is_accept_state_one)
                {
                    return tag1;
                }
                else if (is_accept_state_two)
                {
                    return tag2;
                }
                else
                {
                    return {};
                }
            }

            template<typename T1, typename T2>
            struct fn_create_state_transition
            {
                static_assert(meta::dependent_false<T1, T2>, "detail::fn_create_state_transition: template parameters T1 and T2 must be state");
            };

            template<state_index_t Idx1, bool B1, TTag Tag1, typename... TTrans1, state_index_t Idx2, bool B2, TTag Tag2, typename... TTrans2>
            struct fn_create_state_transition<state<TSym, Idx1, B1, TTag, Tag1, TTrans1...>, state<TSym, Idx2, B2, TTag, Tag2, TTrans2...>>
            {
                template<TSym Sym>
                constexpr auto operator()(meta::value_wrapper<TSym, Sym>) const noexcept
                {
                    constexpr auto symbol  = basic_symbol_v<TSym, Sym>;
                    constexpr auto symbols = basic_symbol_set_v<TSym, Sym>;

                    constexpr state_index_t idx_from = get_combined_index(Idx1, Idx2);
                    constexpr state_index_t idx_to   = get_combined_index(
                        get_transition_index(state_v<TSym, Idx1, B1, TTag, Tag1, TTrans1...>, symbol, sizeof...(TStates1)),
                        get_transition_index(state_v<TSym, Idx2, B2, TTag, Tag2, TTrans2...>, symbol, sizeof...(TStates2)));

                    return meta::wrap(make_state_transition<TSym, idx_from, idx_to>(symbols));
                }
            };

            template<state_index_t Idx1, bool B1, TTag Tag1, typename... TTrans1, state_index_t Idx2, bool B2, TTag Tag2, typename... TTrans2>
            constexpr auto operator()(
                meta::type_wrapper<state<TSym, Idx1, B1, TTag, Tag1, TTrans1...>>,
                meta::type_wrapper<state<TSym, Idx2, B2, TTag, Tag2, TTrans2...>>) const noexcept
            {
                constexpr auto state_alphabet =
                    get_alphabet(state_v<TSym, Idx1, B1, TTag, Tag1, TTrans1...>) |
                    get_alphabet(state_v<TSym, Idx2, B2, TTag, Tag2, TTrans2...>);

                constexpr auto combined_idx = get_combined_index(Idx1, Idx2);
                constexpr auto combined_tag = get_combined_tag(B1, Tag1, B2, Tag2);

                using fn_transform_states = fn_create_state_transition<
                    state<TSym, Idx1, B1, TTag, Tag1, TTrans1...>,
                    state<TSym, Idx2, B2, TTag, Tag2, TTrans2...>>;

                constexpr auto state_transition_list = meta::transform(meta::set_to_list(state_alphabet), fn_transform_states());
                constexpr auto state_transition_list_merged = meta::unwrap(meta::accumulate(state_transition_list, meta::wrap(meta::type_list_v<>), fn_merge_accumulate<TSym, combined_idx>()));
                constexpr auto state = make_state<TSym, combined_idx, B1 || B2, TTag, combined_tag>(state_transition_list_merged);

                return meta::wrap(state);
            }
        };
    }

    template<typename TSym, typename TTag, state_index_t IdxStart1, typename... TStates1, state_index_t IdxStart2, typename... TStates2>
    constexpr auto operator|(state_machine<TSym, IdxStart1, TTag, TStates1...>, state_machine<TSym, IdxStart2, TTag, TStates2...>) noexcept
    {
        constexpr TTag tag_empty = {};

        // Append an empty state to the end of each list
        constexpr auto list1 = meta::type_list_v<TStates1..., state<TSym, sizeof...(TStates1), false, TTag, tag_empty>>;
        constexpr auto list2 = meta::type_list_v<TStates2..., state<TSym, sizeof...(TStates2), false, TTag, tag_empty>>;

        using fn_create_state = detail::fn_create_state<
            state_machine<TSym, IdxStart1, TTag, TStates1...>,
            state_machine<TSym, IdxStart2, TTag, TStates2...>>;

        constexpr state_index_t idx_start = fn_create_state::get_combined_index(IdxStart1, IdxStart2);

        constexpr auto state_list = meta::cartesian_product(list1, list2, fn_create_state());
        constexpr auto state_machine = detail::make_state_machine<TSym, idx_start, TTag>(state_list);

        return state_machine;
    }
}
}

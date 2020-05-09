#include <halberd/lexer.h>

#include <halberd/state_machine.h>
#include <halberd/state_machine_array.h>
#include <halberd/symbol_set.h>
#include <halberd/symbol_set_operators.h>


namespace
{
    namespace ns = halberd::lexer;

    template<typename TSym>
    constexpr ns::basic_symbol_range<TSym, '0', '9'> digit_symbol_range;

    template<typename TSym>
    constexpr auto get_sm_identifier() noexcept
    {
        constexpr auto nondigit_symbol_set =
            ns::basic_symbol_range_v<TSym, 'a', 'z'> |
            ns::basic_symbol_range_v<TSym, 'A', 'Z'> |
            ns::basic_symbol_v<TSym, '_'>;

        constexpr auto s_end =
            ns::state_v<TSym, 0U, true>
                + ns::make_transition_self(nondigit_symbol_set)
                + ns::make_transition_self(digit_symbol_range<TSym>);

        constexpr auto s_start =
            ns::state_v<TSym, ns::next_index(s_end)>
                + ns::make_transition(nondigit_symbol_set, ns::get_index_tag(s_end));

        constexpr auto sm =
            ns::state_machine_v<TSym, ns::get_index(s_start)>
                + s_end
                + s_start;

        return sm;
    }

    template<typename TSym>
    constexpr auto get_sm_fractional_literal() noexcept
    {
        constexpr auto period_symbol = ns::basic_symbol_v<TSym, '.'>;

        constexpr auto s_end =
            ns::state_v<TSym, 0U, true>
                + ns::make_transition_self(digit_symbol_range<TSym>);

        constexpr auto s1 =
            ns::state_v<TSym, ns::next_index(s_end)>
                + ns::make_transition(digit_symbol_range<TSym>, ns::get_index_tag(s_end));
                
        constexpr auto s2 =
            ns::state_v<TSym, ns::next_index(s1)>
                + ns::make_transition_self(digit_symbol_range<TSym>)
                + ns::make_transition(period_symbol, ns::get_index_tag(s_end));

        constexpr auto s_start =
            ns::state_v<TSym, ns::next_index(s2)>
                + ns::make_transition(period_symbol, ns::get_index_tag(s1))
                + ns::make_transition(digit_symbol_range<TSym>, ns::get_index_tag(s2));

        constexpr auto sm =
            ns::state_machine_v<TSym, ns::get_index(s_start)>
                + s_end
                + s1
                + s2
                + s_start;

        return sm;
    }
}

ns::state_machine_view<char> ns::get_smv_identifier() noexcept
{
    constexpr auto sm = ::get_sm_identifier<char>();
    constexpr auto smv = ns::to_state_machine_view(sm);

    return smv;
}

ns::state_machine_view<char> ns::get_smv_fractional_literal() noexcept
{
    constexpr auto sm = ::get_sm_fractional_literal<char>();
    constexpr auto smv = ns::to_state_machine_view(sm);

    return smv;
}

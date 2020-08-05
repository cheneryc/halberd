#include <halberd/lexer.h>

#include <halberd/state_machine.h>
#include <halberd/state_machine_array.h>
#include <halberd/state_machine_operators.h>
#include <halberd/symbol_set.h>
#include <halberd/symbol_set_operators.h>

#include <type_traits> // std::underlying_type_t


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
            ns::state_v<TSym, 0U, true, ns::lexer_tag, ns::lexer_tag::accept_identifier>
                + ns::make_transition_self(nondigit_symbol_set)
                + ns::make_transition_self(digit_symbol_range<TSym>);

        constexpr auto s_start =
            ns::state_v<TSym, ns::next_index(s_end), false, ns::lexer_tag>
                + ns::make_transition(nondigit_symbol_set, ns::get_index_tag(s_end));

        constexpr auto sm =
            ns::state_machine_v<TSym, ns::get_index(s_start), ns::lexer_tag>
                + s_end
                + s_start;

        return sm;
    }

    template<typename TSym>
    constexpr auto get_sm_fractional_literal() noexcept
    {
        constexpr auto period_symbol = ns::basic_symbol_v<TSym, '.'>;

        constexpr auto s_end =
            ns::state_v<TSym, 0U, true, ns::lexer_tag, ns::lexer_tag::accept_literal_fraction>
                + ns::make_transition_self(digit_symbol_range<TSym>);

        constexpr auto s1 =
            ns::state_v<TSym, ns::next_index(s_end), false, ns::lexer_tag>
                + ns::make_transition(digit_symbol_range<TSym>, ns::get_index_tag(s_end));
                
        constexpr auto s2 =
            ns::state_v<TSym, ns::next_index(s1), false, ns::lexer_tag>
                + ns::make_transition_self(digit_symbol_range<TSym>)
                + ns::make_transition(period_symbol, ns::get_index_tag(s_end));

        constexpr auto s_start =
            ns::state_v<TSym, ns::next_index(s2), false, ns::lexer_tag>
                + ns::make_transition(period_symbol, ns::get_index_tag(s1))
                + ns::make_transition(digit_symbol_range<TSym>, ns::get_index_tag(s2));

        constexpr auto sm =
            ns::state_machine_v<TSym, ns::get_index(s_start), ns::lexer_tag>
                + s_end
                + s1
                + s2
                + s_start;

        return sm;
    }
}

namespace halberd
{
namespace lexer
{
    constexpr lexer_tag operator|(lexer_tag tag1, lexer_tag tag2) noexcept
    {
        const auto tag_or = static_cast<std::underlying_type_t<lexer_tag>>(tag1) |
                            static_cast<std::underlying_type_t<lexer_tag>>(tag2);

        return static_cast<lexer_tag>(tag_or);
    }
}
}

ns::state_machine_view<char, ns::lexer_tag> ns::get_smv_identifier() noexcept
{
    constexpr auto sm = ::get_sm_identifier<char>();
    constexpr auto smv = ns::to_state_machine_view(sm);

    return smv;
}

ns::state_machine_view<char, ns::lexer_tag> ns::get_smv_fractional_literal() noexcept
{
    constexpr auto sm = ::get_sm_fractional_literal<char>();
    constexpr auto smv = ns::to_state_machine_view(sm);

    return smv;
}

ns::state_machine_view<char, ns::lexer_tag> ns::get_smv_union() noexcept
{
    constexpr auto sm_union =
        ::get_sm_identifier<char>() |
        ::get_sm_fractional_literal<char>();

    constexpr auto smv_union = ns::to_state_machine_view(sm_union);

    return smv_union;
}

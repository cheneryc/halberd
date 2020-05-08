#include <halberd/state_machine.h>
#include <halberd/state_machine_array.h>
#include <halberd/symbol_set.h>
#include <halberd/symbol_set_operators.h>

#include <gtest/gtest.h>


using namespace halberd;

namespace
{
    template<typename TSym>
    constexpr auto get_state_machine() noexcept
    {
        using namespace lexer;

        constexpr auto digit_symbol_range = basic_symbol_range_v<TSym, '0', '9'>;
        
        constexpr auto nondigit_symbol_set =
            basic_symbol_range_v<TSym, 'a', 'z'> |
            basic_symbol_range_v<TSym, 'A', 'Z'> |
            basic_symbol_v<TSym, '_'>;

        constexpr auto s_end =
            state_v<TSym, 0U, true>
                + make_transition_self(nondigit_symbol_set)
                + make_transition_self(digit_symbol_range);

        constexpr auto s_start =
            state_v<TSym, next_index(s_end)>
                + make_transition(nondigit_symbol_set, get_index_tag(s_end));

        constexpr auto sm =
            state_machine_v<TSym, get_index(s_start)>
                + s_end
                + s_start;

        return sm;
    }
}

TEST(state_machine, lexer_status_true)
{
    constexpr auto sm = ::get_state_machine<char>();
    constexpr auto smv = lexer::to_state_machine_view(sm);

    ASSERT_EQ(1U, smv.idx_start);
    ASSERT_EQ(2U, std::distance(smv.begin(), smv.end()));
}
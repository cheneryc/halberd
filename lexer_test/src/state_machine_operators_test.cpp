#include <halberd/state_machine_operators.h>
#include <halberd/state_machine.h>
#include <halberd/state_machine_array.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace
{
    namespace ns = halberd::lexer;
}

TEST(state_machine_operators, union_operator)
{
    constexpr auto sm1 =
        ns::state_machine_v<char>
            + (ns::state_v<char, 0U, true>
                + ns::make_transition_self(ns::basic_symbol_v<char, '0'>));

    constexpr auto sm2 =
        ns::state_machine_v<char>
            + (ns::state_v<char, 0U, true>
                + ns::make_transition_self(ns::basic_symbol_v<char, '1'>));

    constexpr auto sm_union = sm1 | sm2;
}

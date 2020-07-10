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
    constexpr auto smv_union = ns::to_state_machine_view(sm_union);

    // Combined state machine has 4 states
    ASSERT_EQ(0U, smv_union.idx_start);
    ASSERT_EQ(4U, std::distance(smv_union.begin(), smv_union.end()));

    const auto& sv0 = smv_union[0]; // [s1, s2]

    ASSERT_TRUE(sv0.is_accept_state);
    ASSERT_EQ(0U, sv0.idx);
    ASSERT_EQ(2U, std::distance(sv0.begin(), sv0.end())); // The state has two transitions

    ASSERT_EQ(0U, sv0[0].idx_from);
    ASSERT_EQ(1U, sv0[0].idx_to);
    ASSERT_THAT(sv0[0], ::testing::ElementsAre('0'));

    ASSERT_EQ(0U, sv0[1].idx_from);
    ASSERT_EQ(2U, sv0[1].idx_to);
    ASSERT_THAT(sv0[1], ::testing::ElementsAre('1'));

    const auto& sv1 = smv_union[1]; // [s1, e]

    ASSERT_TRUE(sv1.is_accept_state);
    ASSERT_EQ(1U, sv1.idx);
    ASSERT_EQ(1U, std::distance(sv1.begin(), sv1.end())); // The state has one self-transition

    ASSERT_EQ(1U, sv1[0].idx_from);
    ASSERT_EQ(1U, sv1[0].idx_to);
    ASSERT_THAT(sv1[0], ::testing::ElementsAre('0'));

    const auto& sv2 = smv_union[2]; // [e, s2]

    ASSERT_TRUE(sv2.is_accept_state);
    ASSERT_EQ(2U, sv2.idx);
    ASSERT_EQ(1U, std::distance(sv2.begin(), sv2.end())); // The state has one self-transition

    ASSERT_EQ(2U, sv2[0].idx_from);
    ASSERT_EQ(2U, sv2[0].idx_to);
    ASSERT_THAT(sv2[0], ::testing::ElementsAre('1'));

    const auto& sv3 = smv_union[3]; // [e, e]

    ASSERT_FALSE(sv3.is_accept_state);
    ASSERT_EQ(3U, sv3.idx);
    ASSERT_EQ(0U, std::distance(sv3.begin(), sv3.end())); // The state has no transitions
}

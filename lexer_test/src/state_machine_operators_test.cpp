#include <halberd/state_machine_operators.h>
#include <halberd/state_machine.h>
#include <halberd/state_machine_array.h>
#include <halberd/symbol_set.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <type_traits> // std::underlying_type_t


namespace
{
    namespace ns = halberd::lexer;

    enum class test_tag : unsigned short
    {
        none = 0U,
        accept_zeroes = 0x1,
        accept_tens   = 0x2
    };

    constexpr test_tag operator|(test_tag tag1, test_tag tag2) noexcept
    {
        const auto tag_or = static_cast<std::underlying_type_t<test_tag>>(tag1) |
                            static_cast<std::underlying_type_t<test_tag>>(tag2);

        return static_cast<test_tag>(tag_or);
    }
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

    const auto& sv0 = smv_union[0]; // [sm1.0, sm2.0]

    ASSERT_TRUE(sv0.is_accept_state);
    ASSERT_EQ(0U, sv0.idx);
    ASSERT_EQ(2U, std::distance(sv0.begin(), sv0.end())); // The state has two transitions

    ASSERT_EQ(0U, sv0[0].idx_from);
    ASSERT_EQ(1U, sv0[0].idx_to);
    ASSERT_THAT(sv0[0], ::testing::ElementsAre('0'));

    ASSERT_EQ(0U, sv0[1].idx_from);
    ASSERT_EQ(2U, sv0[1].idx_to);
    ASSERT_THAT(sv0[1], ::testing::ElementsAre('1'));

    const auto& sv1 = smv_union[1]; // [sm1.0, sm2.e]

    ASSERT_TRUE(sv1.is_accept_state);
    ASSERT_EQ(1U, sv1.idx);
    ASSERT_EQ(1U, std::distance(sv1.begin(), sv1.end())); // The state has one self-transition

    ASSERT_EQ(1U, sv1[0].idx_from);
    ASSERT_EQ(1U, sv1[0].idx_to);
    ASSERT_THAT(sv1[0], ::testing::ElementsAre('0'));

    const auto& sv2 = smv_union[2]; // [sm1.e, sm2.0]

    ASSERT_TRUE(sv2.is_accept_state);
    ASSERT_EQ(2U, sv2.idx);
    ASSERT_EQ(1U, std::distance(sv2.begin(), sv2.end())); // The state has one self-transition

    ASSERT_EQ(2U, sv2[0].idx_from);
    ASSERT_EQ(2U, sv2[0].idx_to);
    ASSERT_THAT(sv2[0], ::testing::ElementsAre('1'));

    const auto& sv3 = smv_union[3]; // [sm1.e, sm2.e]

    ASSERT_FALSE(sv3.is_accept_state);
    ASSERT_EQ(3U, sv3.idx);
    ASSERT_EQ(0U, std::distance(sv3.begin(), sv3.end())); // The state has no transitions
}

TEST(state_machine_operators, union_operator_tagging)
{
    // State machine accepts 0..n instances of '00'
    constexpr auto sm1 =
        ns::state_machine_v<char, 0U, test_tag>
            + (ns::state_v<char, 0U, true, test_tag, test_tag::accept_zeroes>
                + ns::make_transition(ns::basic_symbol_v<char, '0'>, ns::state_index_tag_v<1U>))
            + (ns::state_v<char, 1U, false, test_tag>
                + ns::make_transition(ns::basic_symbol_v<char, '0'>, ns::state_index_tag_v<0U>));

    // State machine accepts 0..n instances of '10'
    constexpr auto sm2 =
        ns::state_machine_v<char, 0U, test_tag>
            + (ns::state_v<char, 0U, true, test_tag, test_tag::accept_tens>
                + ns::make_transition(ns::basic_symbol_v<char, '1'>, ns::state_index_tag_v<1U>))
            + (ns::state_v<char, 1U, false, test_tag>
                + ns::make_transition(ns::basic_symbol_v<char, '0'>, ns::state_index_tag_v<0U>));

    constexpr auto sm_union = sm1 | sm2;
    constexpr auto smv_union = ns::to_state_machine_view(sm_union);

    // Combined state machine has 9 states
    ASSERT_EQ(0U, smv_union.idx_start);
    ASSERT_EQ(9U, std::distance(smv_union.begin(), smv_union.end()));

    const auto& sv0 = smv_union[0]; // [sm1.0, sm2.0]

    EXPECT_TRUE(sv0.is_accept_state);
    EXPECT_EQ(test_tag::accept_zeroes | test_tag::accept_tens, sv0.tag);

    const auto& sv1 = smv_union[1]; // [sm1.0, sm2.1]

    EXPECT_TRUE(sv1.is_accept_state);
    EXPECT_EQ(test_tag::accept_zeroes, sv1.tag);

    const auto& sv2 = smv_union[2]; // [sm1.0, sm2.e]

    EXPECT_TRUE(sv2.is_accept_state);
    EXPECT_EQ(test_tag::accept_zeroes, sv2.tag);

    const auto& sv3 = smv_union[3]; // [sm1.1, sm2.0]

    EXPECT_TRUE(sv3.is_accept_state);
    EXPECT_EQ(test_tag::accept_tens, sv3.tag);

    const auto& sv4 = smv_union[4]; // [sm1.1, sm2.1]

    EXPECT_FALSE(sv4.is_accept_state);
    EXPECT_EQ(test_tag::none, sv4.tag);

    const auto& sv5 = smv_union[5]; // [sm1.1, sm2.e]

    EXPECT_FALSE(sv5.is_accept_state);
    EXPECT_EQ(test_tag::none, sv5.tag);

    const auto& sv6 = smv_union[6]; // [sm1.e, sm2.0]

    EXPECT_TRUE(sv6.is_accept_state);
    EXPECT_EQ(test_tag::accept_tens, sv6.tag);

    const auto& sv7 = smv_union[7]; // [sm1.e, sm2.1]

    EXPECT_FALSE(sv7.is_accept_state);
    EXPECT_EQ(test_tag::none, sv7.tag);

    const auto& sv8 = smv_union[8]; // [sm1.e, sm2.e]

    EXPECT_FALSE(sv8.is_accept_state);
    EXPECT_EQ(test_tag::none, sv8.tag);
}

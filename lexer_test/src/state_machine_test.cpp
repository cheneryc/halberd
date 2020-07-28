#include <halberd/state_machine.h>
#include <halberd/state_machine_array.h>

#include <halberd/symbol_set.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace
{
    namespace ns = halberd::lexer;
}

TEST(state_machine, state_transition_to_view)
{
    using test_transition_symbols = ns::basic_symbol_set<char, 'a', 'b', 'c'>;

    constexpr auto test_transition = ns::state_transition_v<char, 0U, 1U, test_transition_symbols>;
    constexpr auto test_transition_view = ns::to_state_transition_view(test_transition);

    ASSERT_EQ(0U, test_transition_view.idx_from);
    ASSERT_EQ(1U, test_transition_view.idx_to);

    ASSERT_THAT(test_transition_view, ::testing::ElementsAre('a', 'b', 'c'));
}

TEST(state_machine, state_to_view)
{
    using test_transition_symbols = ns::basic_symbol_set<char, 'X', 'Y', 'Z'>;
    using test_transition = ns::state_transition<char, 0U, 0U, test_transition_symbols>;

    constexpr auto test_state = ns::state_v<char, 0U, false, ns::state_tag, ns::state_tag_default, test_transition>;
    constexpr auto test_state_view = ns::to_state_view(test_state);

    ASSERT_EQ(0U, test_state_view.idx);
    ASSERT_FALSE(test_state_view.is_accept_state);
    ASSERT_EQ(1U, std::distance(test_state_view.begin(), test_state_view.end()));

    ASSERT_THAT(test_state_view[0], ::testing::ElementsAre('X', 'Y', 'Z'));
}

TEST(state_machine, state_operator_plus)
{
    constexpr auto test_state =
        ns::state_v<char, 0U, false>
            + ns::make_transition(ns::basic_symbol_v<char, '?'>, ns::state_index_tag_v<0U>);

    constexpr auto test_state_view = ns::to_state_view(test_state);

    ASSERT_EQ(0U, test_state_view.idx);
    ASSERT_FALSE(test_state_view.is_accept_state);
    ASSERT_EQ(1U, std::distance(test_state_view.begin(), test_state_view.end()));

    ASSERT_THAT(test_state_view[0], ::testing::ElementsAre('?'));
}

TEST(state_machine, state_operator_plus_chaining)
{
    constexpr auto test_state =
        ns::state_v<char, 0U, false>
            + ns::make_transition(ns::basic_symbol_v<char, '?'>, ns::state_index_tag_v<0U>)
            + ns::make_transition(ns::basic_symbol_v<char, '!'>, ns::state_index_tag_v<0U>);

    constexpr auto test_state_view = ns::to_state_view(test_state);

    ASSERT_EQ(0U, test_state_view.idx);
    ASSERT_FALSE(test_state_view.is_accept_state);
    ASSERT_EQ(2U, std::distance(test_state_view.begin(), test_state_view.end()));

    ASSERT_THAT(test_state_view[0], ::testing::ElementsAre('?'));
    ASSERT_THAT(test_state_view[1], ::testing::ElementsAre('!'));
}

TEST(state_machine, state_operator_plus_self)
{
    constexpr auto test_state =
        ns::state_v<char, 0U, false>
            + ns::make_transition_self(ns::basic_symbol_v<char, '?'>);

    constexpr auto test_state_view = ns::to_state_view(test_state);

    ASSERT_EQ(0U, test_state_view.idx);
    ASSERT_FALSE(test_state_view.is_accept_state);
    ASSERT_EQ(1U, std::distance(test_state_view.begin(), test_state_view.end()));

    ASSERT_EQ(0U, test_state_view[0].idx_from);
    ASSERT_EQ(0U, test_state_view[0].idx_to);

    ASSERT_THAT(test_state_view[0], ::testing::ElementsAre('?'));
}

TEST(state_machine, state_operator_plus_chaining_self)
{
    constexpr auto test_state =
        ns::state_v<char, 0U, false>
            + ns::make_transition_self(ns::basic_symbol_v<char, '?'>)
            + ns::make_transition_self(ns::basic_symbol_v<char, '!'>);

    constexpr auto test_state_view = ns::to_state_view(test_state);

    ASSERT_EQ(0U, test_state_view.idx);
    ASSERT_FALSE(test_state_view.is_accept_state);
    ASSERT_EQ(2U, std::distance(test_state_view.begin(), test_state_view.end()));

    ASSERT_EQ(0U, test_state_view[0].idx_from);
    ASSERT_EQ(0U, test_state_view[0].idx_to);
    ASSERT_EQ(0U, test_state_view[1].idx_from);
    ASSERT_EQ(0U, test_state_view[1].idx_to);

    ASSERT_THAT(test_state_view[0], ::testing::ElementsAre('?'));
    ASSERT_THAT(test_state_view[1], ::testing::ElementsAre('!'));
}

TEST(state_machine, state_machine_to_view)
{
    using test_transition_symbols = ns::basic_symbol_set<char, '$'>;
    using test_transition = ns::state_transition<char, 0U, 0U, test_transition_symbols>;
    using test_state = ns::state<char, 0U, true, ns::state_tag, ns::state_tag_default, test_transition>;
    
    constexpr auto test_state_machine = ns::state_machine_v<char, 0U, ns::state_tag, test_state>;
    constexpr auto test_state_machine_view = ns::to_state_machine_view(test_state_machine);

    ASSERT_EQ(0U, test_state_machine_view.idx_start);
    ASSERT_EQ(1U, std::distance(test_state_machine_view.begin(), test_state_machine_view.end()));
    ASSERT_EQ(1U, std::distance(test_state_machine_view[0].begin(), test_state_machine_view[0].end()));

    ASSERT_THAT(test_state_machine_view[0][0], ::testing::ElementsAre('$'));
}

TEST(state_machine, state_machine_operator_plus)
{
    constexpr auto test_state_machine =
        ns::state_machine_v<char>
            + (ns::state_v<char, 0U>
                + ns::make_transition_self(ns::basic_symbol_v<char, '@'>));

    constexpr auto test_state_machine_view = ns::to_state_machine_view(test_state_machine);

    ASSERT_EQ(0U, test_state_machine_view.idx_start);
    ASSERT_EQ(1U, std::distance(test_state_machine_view.begin(), test_state_machine_view.end()));
    ASSERT_EQ(1U, std::distance(test_state_machine_view[0].begin(), test_state_machine_view[0].end()));

    ASSERT_THAT(test_state_machine_view[0][0], ::testing::ElementsAre('@'));
}

TEST(state_machine, state_machine_operator_plus_chaining)
{
    constexpr auto test_state_machine =
        ns::state_machine_v<char>
            + (ns::state_v<char, 0U>
                + ns::make_transition_self(ns::basic_symbol_v<char, '<'>))
            + (ns::state_v<char, 1U>
                + ns::make_transition_self(ns::basic_symbol_v<char, '>'>));

    constexpr auto test_state_machine_view = ns::to_state_machine_view(test_state_machine);

    ASSERT_EQ(0U, test_state_machine_view.idx_start);
    ASSERT_EQ(2U, std::distance(test_state_machine_view.begin(), test_state_machine_view.end()));
    ASSERT_EQ(1U, std::distance(test_state_machine_view[0].begin(), test_state_machine_view[0].end()));
    ASSERT_EQ(1U, std::distance(test_state_machine_view[1].begin(), test_state_machine_view[1].end()));

    ASSERT_THAT(test_state_machine_view[0][0], ::testing::ElementsAre('<'));
    ASSERT_THAT(test_state_machine_view[1][0], ::testing::ElementsAre('>'));
}

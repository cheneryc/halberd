#include <halberd/symbol_set.h>
#include <halberd/symbol_set_array.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using namespace halberd;

TEST(symbol_set, basic_symbol_to_set)
{
    using test_symbol = lexer::basic_symbol<char, 'a'>;
    using test_symbol_set = typename lexer::make_symbol_set<test_symbol>::type;
    using test_symbol_set_values = lexer::basic_symbol_set_array<test_symbol_set>;

    ASSERT_THAT(test_symbol_set_values::values, ::testing::ElementsAre('a'));
}

TEST(symbol_set, basic_symbol_sequence_to_set)
{
    using test_symbol_sequence = lexer::basic_symbol_sequence<char, 'a', 4U>;
    using test_symbol_set = typename lexer::make_symbol_set<test_symbol_sequence>::type;
    using test_symbol_set_values = lexer::basic_symbol_set_array<test_symbol_set>;

    ASSERT_THAT(test_symbol_set_values::values, ::testing::ElementsAre('a', 'b', 'c', 'd'));
}

TEST(symbol_set, basic_symbol_range_to_set)
{
    using test_symbol_range = lexer::basic_symbol_range<char, '0', '9'>;
    using test_symbol_set = typename lexer::make_symbol_set<test_symbol_range>::type;
    using test_symbol_set_values = lexer::basic_symbol_set_array<test_symbol_set>;

    ASSERT_THAT(test_symbol_set_values::values, ::testing::ElementsAre('0', '1', '2', '3', '4', '5', '6', '7', '8', '9'));
}

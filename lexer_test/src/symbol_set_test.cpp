#include <halberd/symbol_set.h>
#include <halberd/symbol_set_array.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace
{
    namespace ns = halberd::lexer;
}

TEST(symbol_set, basic_symbol_to_set)
{
    constexpr auto test_symbol = ns::basic_symbol_v<char, 'a'>;
    constexpr auto test_symbol_set = ns::make_symbol_set(test_symbol);
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a'));
}

TEST(symbol_set, basic_symbol_sequence_to_set)
{
    constexpr auto test_symbol_sequence = ns::basic_symbol_sequence_v<char, 'a', 4U>;
    constexpr auto test_symbol_set = ns::make_symbol_set(test_symbol_sequence);
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b', 'c', 'd'));
}

TEST(symbol_set, basic_symbol_range_to_set)
{
    constexpr auto test_symbol_range = ns::basic_symbol_range_v<char, '0', '9'>;
    constexpr auto test_symbol_set = ns::make_symbol_set(test_symbol_range);
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('0', '1', '2', '3', '4', '5', '6', '7', '8', '9'));
}

#include <halberd/symbol_set_operators.h>
#include <halberd/symbol_set_array.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using namespace halberd;

TEST(symbol_set_operators, union_operator)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_set_v<char, 'a'> |
                                     lexer::basic_symbol_set_v<char, 'b'>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b'));
}

TEST(symbol_set_operators, union_operator_reverse_args)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_set_v<char, 'b'> |
                                     lexer::basic_symbol_set_v<char, 'a'>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b'));
}

TEST(symbol_set_operators, union_operator_two_symbols)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_set_v<char, 'a', 'b'> |
                                     lexer::basic_symbol_set_v<char, 'b', 'c'>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b', 'c'));
}

TEST(symbol_set_operators, union_operator_two_symbols_reverse_args)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_set_v<char, 'b', 'c'> |
                                     lexer::basic_symbol_set_v<char, 'a', 'b'>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b', 'c'));
}

TEST(symbol_set_operators, union_operator_varying_symbols)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_set_v<char, 'a'> |
                                     lexer::basic_symbol_set_v<char, 'b', 'c', 'd'>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b', 'c', 'd'));
}

TEST(symbol_set_operators, union_operator_varying_symbols_reverse_args)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_set_v<char, 'b', 'c', 'd'> |
                                     lexer::basic_symbol_set_v<char, 'a'>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b', 'c', 'd'));
}

TEST(symbol_set_operators, union_operator_chaining)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_set_v<char, 'a', 'b'> |
                                     lexer::basic_symbol_set_v<char, 'b', 'c'> |
                                     lexer::basic_symbol_set_v<char, 'c', 'd'>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b', 'c', 'd'));
}

TEST(symbol_set_operators, union_operator_chaining_reverse_args)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_set_v<char, 'c', 'd'> |
                                     lexer::basic_symbol_set_v<char, 'b', 'c'> |
                                     lexer::basic_symbol_set_v<char, 'a', 'b'>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b', 'c', 'd'));
}

TEST(symbol_set_operators, union_operator_convert_symbol)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_v<char, 'a'> |
                                     lexer::basic_symbol_v<char, 'b'>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b'));
}

TEST(symbol_set_operators, union_operator_convert_symbol_reverse_args)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_v<char, 'b'> |
                                     lexer::basic_symbol_v<char, 'a'>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b'));
}

TEST(symbol_set_operators, union_operator_convert_symbol_sequence)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_sequence_v<char, 'a', 3U> |
                                     lexer::basic_symbol_sequence_v<char, '0', 4U>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('0', '1', '2', '3', 'a', 'b', 'c'));
}

TEST(symbol_set_operators, union_operator_convert_symbol_sequence_reverse_args)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_sequence_v<char, '0', 4U> |
                                     lexer::basic_symbol_sequence_v<char, 'a', 3U>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('0', '1', '2', '3', 'a', 'b', 'c'));
}

TEST(symbol_set_operators, union_operator_convert_symbol_range)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_range_v<char, 'a', 'd'> |
                                     lexer::basic_symbol_range_v<char, '0', '2'>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('0', '1', '2', 'a', 'b', 'c', 'd'));
}

TEST(symbol_set_operators, union_operator_convert_symbol_range_reverse_args)
{
    constexpr auto test_symbol_set = lexer::basic_symbol_range_v<char, '0', '2'> |
                                     lexer::basic_symbol_range_v<char, 'a', 'd'>;
    constexpr auto test_symbol_set_array = lexer::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('0', '1', '2', 'a', 'b', 'c', 'd'));
}

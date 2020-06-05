#include <halberd/symbol_set_operators.h>
#include <halberd/symbol_set.h>
#include <halberd/symbol_set_array.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace
{
    namespace ns = halberd::lexer;
}

TEST(symbol_set_operators, union_operator)
{
    constexpr auto test_symbol_set = ns::basic_symbol_set_v<char, 'a'> |
                                     ns::basic_symbol_set_v<char, 'b'>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b'));
}

TEST(symbol_set_operators, union_operator_reverse_args)
{
    constexpr auto test_symbol_set = ns::basic_symbol_set_v<char, 'b'> |
                                     ns::basic_symbol_set_v<char, 'a'>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b'));
}

TEST(symbol_set_operators, union_operator_two_symbols)
{
    constexpr auto test_symbol_set = ns::basic_symbol_set_v<char, 'a', 'b'> |
                                     ns::basic_symbol_set_v<char, 'b', 'c'>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b', 'c'));
}

TEST(symbol_set_operators, union_operator_two_symbols_reverse_args)
{
    constexpr auto test_symbol_set = ns::basic_symbol_set_v<char, 'b', 'c'> |
                                     ns::basic_symbol_set_v<char, 'a', 'b'>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b', 'c'));
}

TEST(symbol_set_operators, union_operator_varying_symbols)
{
    constexpr auto test_symbol_set = ns::basic_symbol_set_v<char, 'a'> |
                                     ns::basic_symbol_set_v<char, 'b', 'c', 'd'>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b', 'c', 'd'));
}

TEST(symbol_set_operators, union_operator_varying_symbols_reverse_args)
{
    constexpr auto test_symbol_set = ns::basic_symbol_set_v<char, 'b', 'c', 'd'> |
                                     ns::basic_symbol_set_v<char, 'a'>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b', 'c', 'd'));
}

TEST(symbol_set_operators, union_operator_chaining)
{
    constexpr auto test_symbol_set = ns::basic_symbol_set_v<char, 'a', 'b'> |
                                     ns::basic_symbol_set_v<char, 'b', 'c'> |
                                     ns::basic_symbol_set_v<char, 'c', 'd'>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b', 'c', 'd'));
}

TEST(symbol_set_operators, union_operator_chaining_reverse_args)
{
    constexpr auto test_symbol_set = ns::basic_symbol_set_v<char, 'c', 'd'> |
                                     ns::basic_symbol_set_v<char, 'b', 'c'> |
                                     ns::basic_symbol_set_v<char, 'a', 'b'>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b', 'c', 'd'));
}

TEST(symbol_set_operators, union_operator_convert_symbol)
{
    constexpr auto test_symbol_set = ns::basic_symbol_v<char, 'a'> |
                                     ns::basic_symbol_v<char, 'b'>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b'));
}

TEST(symbol_set_operators, union_operator_convert_symbol_reverse_args)
{
    constexpr auto test_symbol_set = ns::basic_symbol_v<char, 'b'> |
                                     ns::basic_symbol_v<char, 'a'>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('a', 'b'));
}

TEST(symbol_set_operators, union_operator_convert_symbol_sequence)
{
    constexpr auto test_symbol_set = ns::basic_symbol_sequence_v<char, 'a', 3U> |
                                     ns::basic_symbol_sequence_v<char, '0', 4U>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('0', '1', '2', '3', 'a', 'b', 'c'));
}

TEST(symbol_set_operators, union_operator_convert_symbol_sequence_reverse_args)
{
    constexpr auto test_symbol_set = ns::basic_symbol_sequence_v<char, '0', 4U> |
                                     ns::basic_symbol_sequence_v<char, 'a', 3U>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('0', '1', '2', '3', 'a', 'b', 'c'));
}

TEST(symbol_set_operators, union_operator_convert_symbol_range)
{
    constexpr auto test_symbol_set = ns::basic_symbol_range_v<char, 'a', 'd'> |
                                     ns::basic_symbol_range_v<char, '0', '2'>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('0', '1', '2', 'a', 'b', 'c', 'd'));
}

TEST(symbol_set_operators, union_operator_convert_symbol_range_reverse_args)
{
    constexpr auto test_symbol_set = ns::basic_symbol_range_v<char, '0', '2'> |
                                     ns::basic_symbol_range_v<char, 'a', 'd'>;
    constexpr auto test_symbol_set_array = ns::to_symbol_set_array(test_symbol_set);

    ASSERT_THAT(test_symbol_set_array, ::testing::ElementsAre('0', '1', '2', 'a', 'b', 'c', 'd'));
}

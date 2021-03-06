#include "combinator_test_common.h"

#include <halberd/parser.h>

#include <gtest/gtest.h>

#include <tuple> // std::tuple, std::make_tuple


namespace
{
    namespace ns = halberd::parser;

    constexpr char toupper(char ch) noexcept
    {
        return ch - ('a' - 'A'); // std::toupper isn't constexpr
    }

    constexpr char toupper_transform(std::tuple<char> ch) noexcept
    {
        return toupper(std::get<0U>(ch));
    }

    template<char Ch>
    constexpr auto test_match_v = ns::make_filter(test::is_token<Ch>);

    template<char Ch>
    constexpr auto test_transform_v = ns::make_transform(test_match_v<Ch>, toupper_transform);
}

TEST(parser, combinator_one_apply_failure)
{
    test::scanner scanner = {};
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    ns::combinator_one cb_one;

    auto res = cb_one.apply(source);
    ASSERT_FALSE(res);
}

TEST(parser, combinator_one_apply_success)
{
    test::scanner scanner = { { 'a' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    ns::combinator_one cb_one;

    auto res1 = cb_one.apply(source);
    ASSERT_TRUE(res1);
    ASSERT_EQ('a', res1.get());

    auto res2 = cb_one.apply(source);
    ASSERT_FALSE(res2);
}

TEST(parser, combinator_one_apply_success_repeat)
{
    test::scanner scanner = { { 'a', 'b' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    ns::combinator_one cb_one;

    auto res1 = cb_one.apply(source);
    ASSERT_TRUE(res1);
    ASSERT_EQ('a', res1.get());

    auto res2 = cb_one.apply(source);
    ASSERT_TRUE(res2);
    ASSERT_EQ('b', res2.get());

    auto res3 = cb_one.apply(source);
    ASSERT_FALSE(res3);
}

TEST(parser, combinator_end_apply_failure)
{
    test::scanner scanner = { { 'a' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    ns::combinator_end cb_end;

    auto res = cb_end.apply(source);
    ASSERT_FALSE(res);
}

TEST(parser, combinator_end_apply_success)
{
    test::scanner scanner = {};
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    ns::combinator_end cb_end;

    auto res = cb_end.apply(source);
    ASSERT_TRUE(res);
}

TEST(parser, combinator_filter_apply_failure)
{
    test::scanner scanner = { { '!' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res = test_match_v<'?'>.apply(source);
    ASSERT_FALSE(res);
}

TEST(parser, combinator_filter_apply_success)
{
    test::scanner scanner = { { 'a' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res = test_match_v<'a'>.apply(source);
    ASSERT_TRUE(res);
    ASSERT_EQ('a', res.get());
}

TEST(parser, combinator_filter_apply_success_repeat)
{
    test::scanner scanner = { { 'a', 'b' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res1 = test_match_v<'a'>.apply(source);
    ASSERT_TRUE(res1);
    ASSERT_EQ('a', res1.get());

    auto res2 = test_match_v<'b'>.apply(source);
    ASSERT_TRUE(res2);
    ASSERT_EQ('b', res2.get());
}

TEST(parser, combinator_filter_apply_success_failure)
{
    test::scanner scanner = { { '0', '1' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res1 = test_match_v<'0'>.apply(source);
    ASSERT_TRUE(res1);
    ASSERT_EQ('0', res1.get());

    auto res2 = test_match_v<'0'>.apply(source);
    ASSERT_FALSE(res2);

    // Ensure failing to parse didn't advance to the next token
    auto res3 = test_match_v<'1'>.apply(source);
    ASSERT_TRUE(res3);
    ASSERT_EQ('1', res3.get());
}

TEST(parser, combinator_transform_apply_failure)
{
    test::scanner scanner = { { '!' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res = test_transform_v<'?'>.apply(source);
    ASSERT_FALSE(res);
}

TEST(parser, combinator_transform_apply_success)
{
    test::scanner scanner = { { 'a' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res = test_transform_v<'a'>.apply(source);
    ASSERT_TRUE(res);
    ASSERT_EQ('A', res.get());
}

TEST(parser, combinator_transform_apply_success_repeat)
{
    test::scanner scanner = { { 'a', 'b' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res1 = test_transform_v<'a'>.apply(source);
    ASSERT_TRUE(res1);
    ASSERT_EQ('A', res1.get());

    auto res2 = test_transform_v<'b'>.apply(source);
    ASSERT_TRUE(res2);
    ASSERT_EQ('B', res2.get());
}

TEST(parser, combinator_transform_apply_success_failure)
{
    test::scanner scanner = { { 'x', 'y' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res1 = test_transform_v<'x'>.apply(source);
    ASSERT_TRUE(res1);
    ASSERT_EQ('X', res1.get());

    auto res2 = test_transform_v<'x'>.apply(source);
    ASSERT_FALSE(res2);

    // Ensure failing to parse didn't advance to the next token
    auto res3 = test_transform_v<'y'>.apply(source);
    ASSERT_TRUE(res3);
    ASSERT_EQ('Y', res3.get());
}

TEST(parser, combinator_choice_apply_failure)
{
    test::scanner scanner = { { '!' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    constexpr auto parser = ns::make_choice(test_match_v<'('>, test_match_v<')'>);

    auto res = parser.apply(source);
    ASSERT_FALSE(res);
}

TEST(parser, combinator_choice_apply_success)
{
    test::scanner scanner = { { 'a', 'b', 'c' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    constexpr auto parser = ns::make_choice(test_match_v<'a'>, test_match_v<'b'>, test_match_v<'c'>);

    auto res1 = parser.apply(source);
    ASSERT_TRUE(res1);
    ASSERT_EQ('a', res1.get());

    auto res2 = parser.apply(source);
    ASSERT_TRUE(res2);
    ASSERT_EQ('b', res2.get());

    auto res3 = parser.apply(source);
    ASSERT_TRUE(res3);
    ASSERT_EQ('c', res3.get());
}

TEST(parser, combinator_choice_apply_success_failure)
{
    test::scanner scanner = { { 'x', 'y', 'z' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    constexpr auto parser = ns::make_choice(test_match_v<'x'>, test_match_v<'z'>);

    auto res1 = parser.apply(source);
    ASSERT_TRUE(res1);
    ASSERT_EQ('x', res1.get());

    auto res2 = parser.apply(source);
    ASSERT_FALSE(res2);

    // Ensure failing to parse didn't advance to the next token
    auto res3 = parser.apply(source);
    ASSERT_FALSE(res3);
}

TEST(parser, combinator_sequence_apply_failure)
{
    test::scanner scanner = { { '1', '2', '3' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    constexpr auto parser = ns::make_sequence(test_match_v<'1'>, test_match_v<'2'>, test_match_v<'3'>, test_match_v<'4'>);

    auto res = parser.apply(source);
    ASSERT_FALSE(res);
}

TEST(parser, combinator_sequence_apply_success)
{
    test::scanner scanner = { { '1', '2', '3', '4' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    constexpr auto parser = ns::make_sequence(test_match_v<'1'>, test_match_v<'2'>, test_match_v<'3'>, test_match_v<'4'>);

    auto res = parser.apply(source);
    ASSERT_TRUE(res);
    ASSERT_EQ(std::make_tuple('1', '2', '3', '4'), get_as_tuple(res));
}

TEST(parser, combinator_sequence_apply_success_repeat)
{
    test::scanner scanner = { { '1', '2', '3', '1', '2', '3' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    constexpr auto parser = ns::make_sequence(test_match_v<'1'>, test_match_v<'2'>, test_match_v<'3'>);

    auto res1 = parser.apply(source);
    ASSERT_TRUE(res1);
    ASSERT_EQ(std::make_tuple('1', '2', '3'), get_as_tuple(res1));

    auto res2 = parser.apply(source);
    ASSERT_TRUE(res2);
    ASSERT_EQ(std::make_tuple('1', '2', '3'), get_as_tuple(res2));
}

TEST(parser, combinator_sequence_apply_success_failure)
{
    test::scanner scanner = { { '1', '2', '3', '1', '2', '1', '2', '3' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    constexpr auto parser = ns::make_sequence(test_match_v<'1'>, test_match_v<'2'>, test_match_v<'3'>);

    auto res1 = parser.apply(source);
    ASSERT_TRUE(res1);
    ASSERT_EQ(std::make_tuple('1', '2', '3'), get_as_tuple(res1));

    auto res2 = parser.apply(source);
    ASSERT_FALSE(res2);

    // Ensure failing to parse didn't advance to the next token
    auto res3 = parser.apply(source);
    ASSERT_FALSE(res3);
}

TEST(parser, combinator_select_apply_failure)
{
    test::scanner scanner = { { 'A', 'B', 'A' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    constexpr auto parser = ns::make_select<1U>(ns::make_sequence(test_match_v<'B'>, test_match_v<'A'>, test_match_v<'B'>));

    auto res = parser.apply(source);
    ASSERT_FALSE(res);
}

TEST(parser, combinator_select_apply_success)
{
    test::scanner scanner = { { '(', '*', ')' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    constexpr auto parser = ns::make_select<1U>(ns::make_sequence(test_match_v<'('>, test_match_v<'*'>, test_match_v<')'>));

    auto res = parser.apply(source);
    ASSERT_TRUE(res);
    ASSERT_EQ('*', res.get());
}

TEST(parser, combinator_select_apply_success_repeat)
{
    test::scanner scanner = { { '$', '&', '$', '&' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    constexpr auto parser = ns::make_select<0U>(ns::make_sequence(test_match_v<'$'>, test_match_v<'&'>));

    auto res1 = parser.apply(source);
    ASSERT_TRUE(res1);
    ASSERT_EQ('$', res1.get());

    auto res2 = parser.apply(source);
    ASSERT_TRUE(res2);
    ASSERT_EQ('$', res2.get());
}

TEST(parser, combinator_select_apply_success_failure)
{
    test::scanner scanner = { { '[', '0', ']', '?', '[', '0', ']' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    constexpr auto parser = ns::make_select<1U>(ns::make_sequence(test_match_v<'['>, test_match_v<'0'>, test_match_v<']'>));

    auto res1 = parser.apply(source);
    ASSERT_TRUE(res1);
    ASSERT_EQ('0', res1.get());

    auto res2 = parser.apply(source);
    ASSERT_FALSE(res2);

    // Ensure failing to parse didn't advance to the next token
    auto res3 = parser.apply(source);
    ASSERT_FALSE(res3);
}

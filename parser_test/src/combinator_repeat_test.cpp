#include "combinator_test_common.h"

#include <halberd/combinator_repeat.h>
#include <halberd/combinator_filter.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace
{
    namespace ns = halberd::parser;

    template<char Ch>
    constexpr auto test_match_v = ns::make_filter(test::is_token<Ch>);

    template<char Ch>
    constexpr auto test_repeat_v = ns::make_repeat(test_match_v<Ch>);
}

TEST(combinator_repeat, apply_failure)
{
    test::scanner scanner = { { '!' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res = test_repeat_v<'?'>.apply(source);
    ASSERT_TRUE(res); // Check that the parser succeeds when there is no match
    auto res_vec = res.get();

    ASSERT_TRUE(res_vec.empty()); // Result contains an empty std::vector
}

TEST(combinator_repeat, apply_success)
{
    test::scanner scanner = { { '#' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res = test_repeat_v<'#'>.apply(source);
    ASSERT_TRUE(res);
    auto res_vec = res.get();

    ASSERT_THAT(res_vec, ::testing::ElementsAre('#'));
}

TEST(combinator_repeat, apply_success_repeat)
{
    test::scanner scanner = { { '#', '#', '#' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res = test_repeat_v<'#'>.apply(source);
    ASSERT_TRUE(res);
    auto res_vec = res.get();

    ASSERT_THAT(res_vec, ::testing::ElementsAre('#', '#', '#'));
}

TEST(combinator_repeat, apply_success_failure)
{
    test::scanner scanner = { { '#', '#', '?', '#' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    constexpr auto parser = ns::make_repeat(test_match_v<'#'>);

    auto res1 = parser.apply(source);
    ASSERT_TRUE(res1);
    auto res1_vec = res1.get();

    ASSERT_THAT(res1_vec, ::testing::ElementsAre('#', '#'));

    auto res2 = parser.apply(source);
    ASSERT_TRUE(res2);
    auto res2_vec = res2.get();

    ASSERT_TRUE(res2_vec.empty());

    // Ensure failing to parse didn't advance to the next token
    auto res3 = parser.apply(source);
    ASSERT_TRUE(res3);
    auto res3_vec = res3.get();

    ASSERT_TRUE(res3_vec.empty());
}
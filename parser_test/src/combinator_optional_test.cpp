#include "combinator_test_common.h"

#include <halberd/combinator_optional.h>
#include <halberd/combinator_filter.h>

#include <gtest/gtest.h>


namespace
{
    namespace ns = halberd::parser;

    template<char Ch>
    constexpr auto test_match_v = ns::make_filter(test::is_token<Ch>);

    template<char Ch>
    constexpr auto test_optional_v = ns::make_optional(test_match_v<Ch>);
}

TEST(combinator_optional, apply_failure)
{
    test::scanner scanner = { { '!' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res = test_optional_v<'?'>.apply(source);
    ASSERT_TRUE(res); // Check that the parser succeeds when there is no match
    auto res_opt = res.get();

    ASSERT_FALSE(res_opt); // Result contains an empty util::optional
}

TEST(combinator_optional, apply_success)
{
    test::scanner scanner = { { 'a' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res = test_optional_v<'a'>.apply(source);
    ASSERT_TRUE(res);
    auto res_opt = res.get();

    ASSERT_TRUE(res_opt);
    ASSERT_EQ('a', res_opt.get());
}

TEST(combinator_optional, apply_success_repeat)
{
    test::scanner scanner = { { 'a', 'b' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res1 = test_optional_v<'a'>.apply(source);
    ASSERT_TRUE(res1);
    auto res1_opt = res1.get();

    ASSERT_TRUE(res1_opt);
    ASSERT_EQ('a', res1_opt.get());

    auto res2 = test_optional_v<'b'>.apply(source);
    ASSERT_TRUE(res2);
    auto res2_opt = res2.get();

    ASSERT_TRUE(res2_opt);
    ASSERT_EQ('b', res2_opt.get());
}

TEST(combinator_optional, apply_success_failure)
{
    test::scanner scanner = { { '0', '1' } };
    auto source = ns::make_source([&scanner](){ return scanner.scan(); });

    auto res1 = test_optional_v<'0'>.apply(source);
    ASSERT_TRUE(res1);
    auto res1_opt = res1.get();

    ASSERT_TRUE(res1_opt);
    ASSERT_EQ('0', res1_opt.get());

    auto res2 = test_optional_v<'0'>.apply(source);
    ASSERT_TRUE(res2);
    auto res2_opt = res2.get();

    ASSERT_FALSE(res2_opt);

    // Ensure failing to parse didn't advance to the next token
    auto res3 = test_optional_v<'1'>.apply(source);
    ASSERT_TRUE(res3);
    auto res3_opt = res3.get();

    ASSERT_TRUE(res3_opt);
    ASSERT_EQ('1', res3_opt.get());
}
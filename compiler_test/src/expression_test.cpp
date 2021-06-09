#include "compiler_test_common.h"

#include <halberd/compiler.h>

#include <gtest/gtest.h>


namespace
{
    namespace ns = halberd::compiler;

    auto compile_expression(const char* src)
    {
        return ns::compile_rule(ns::rule::expression, src);
    }

    auto compile_expression(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
    {
        return ns::compile_rule(ns::rule::expression, std::move(tokens));
    }

    auto compile_expression_postfix(const char* src)
    {
        return ns::compile_rule(ns::rule::expression_postfix, src);
    }

    auto compile_expression_postfix(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
    {
        return ns::compile_rule(ns::rule::expression_postfix, std::move(tokens));
    }
}

TEST(expression, expression_primary)
{
    ASSERT_TRUE(compile_expression("1.0"));
    ASSERT_TRUE(compile_expression("(1.0)"));
    ASSERT_TRUE(compile_expression("x"));
    ASSERT_TRUE(compile_expression("x y"));
    //ASSERT_TRUE(compile_expression("x y z"));
    //ASSERT_TRUE(compile_expression("x 1.0 (y)"));
}

TEST(expression, expression_postfix_inc_passthrough)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_postfix(std::move(tokens)));
}

TEST(expression, expression_postfix_inc)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"),
        make_symbol(halberd::lexer::symbol::op_increment));

    ASSERT_TRUE(compile_expression_postfix(std::move(tokens)));
}

TEST(expression, expression_postfix_inc_multiple)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"),
        make_symbol(halberd::lexer::symbol::op_increment),
        make_symbol(halberd::lexer::symbol::op_increment));

    ASSERT_TRUE(compile_expression_postfix(std::move(tokens)));
}

TEST(expression, expression_postfix_inc_src)
{
    ASSERT_TRUE(compile_expression_postfix("my_var++"));
}

TEST(expression, expression_postfix_dec_passthrough)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_postfix(std::move(tokens)));
}

TEST(expression, expression_postfix_dec)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"),
        make_symbol(halberd::lexer::symbol::op_decrement));

    ASSERT_TRUE(compile_expression_postfix(std::move(tokens)));
}

TEST(expression, expression_postfix_dec_multiple)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"),
        make_symbol(halberd::lexer::symbol::op_decrement),
        make_symbol(halberd::lexer::symbol::op_decrement));

    ASSERT_TRUE(compile_expression_postfix(std::move(tokens)));
}

TEST(expression, expression_postfix_dec_src)
{
    ASSERT_TRUE(compile_expression_postfix("my_var--"));
}

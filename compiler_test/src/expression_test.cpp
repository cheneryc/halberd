#include "compiler_test_common.h"

// halberd::compiler
#include <halberd/compiler.h>

// halberd::syntax
#include <halberd/operator_unary_prefix.h>
#include <halberd/operator_unary_postfix.h>

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

    auto compile_expression_prefix(const char* src)
    {
        return ns::compile_rule(ns::rule::expression_prefix, src);
    }

    auto compile_expression_prefix(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
    {
        return ns::compile_rule(ns::rule::expression_prefix, std::move(tokens));
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

TEST(expression, expression_postfix_passthrough)
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
    ASSERT_TRUE(compile_expression_postfix("my_var"));
    ASSERT_TRUE(compile_expression_postfix("my_var++"));
    ASSERT_TRUE(compile_expression_postfix("my_var++++"));
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
    ASSERT_TRUE(compile_expression_postfix("my_var"));
    ASSERT_TRUE(compile_expression_postfix("my_var--"));
    ASSERT_TRUE(compile_expression_postfix("my_var----"));
}

TEST(expression, expression_postfix_both)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"),
        make_symbol(halberd::lexer::symbol::op_increment),
        make_symbol(halberd::lexer::symbol::op_decrement),
        make_symbol(halberd::lexer::symbol::op_increment),
        make_symbol(halberd::lexer::symbol::op_decrement));

    ASSERT_TRUE(compile_expression_postfix(std::move(tokens)));
}

TEST(expression, expression_postfix_both_src)
{
    ASSERT_TRUE(compile_expression_postfix("my_var"));
    ASSERT_TRUE(compile_expression_postfix("my_var++--"));
    ASSERT_TRUE(compile_expression_postfix("my_var++--++--"));
}

TEST(expression, expression_prefix_passthrough)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_prefix(std::move(tokens)));
}

TEST(expression, expression_prefix_inc)
{
    using namespace test;

    auto tokens = make_tokens(
        make_symbol(halberd::lexer::symbol::op_increment),
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_prefix(std::move(tokens)));
}

TEST(expression, expression_prefix_inc_multiple)
{
    using namespace test;

    auto tokens = make_tokens(
        make_symbol(halberd::lexer::symbol::op_increment),
        make_symbol(halberd::lexer::symbol::op_increment),
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_prefix(std::move(tokens)));
}

TEST(expression, expression_prefix_inc_src)
{
    ASSERT_TRUE(compile_expression_prefix("my_var"));
    ASSERT_TRUE(compile_expression_prefix("++my_var"));
    ASSERT_TRUE(compile_expression_prefix("++++my_var"));
}

TEST(expression, expression_prefix_dec)
{
    using namespace test;

    auto tokens = make_tokens(
        make_symbol(halberd::lexer::symbol::op_decrement),
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_prefix(std::move(tokens)));
}

TEST(expression, expression_prefix_dec_multiple)
{
    using namespace test;

    auto tokens = make_tokens(
        make_symbol(halberd::lexer::symbol::op_decrement),
        make_symbol(halberd::lexer::symbol::op_decrement),
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_prefix(std::move(tokens)));
}

TEST(expression, expression_prefix_dec_src)
{
    ASSERT_TRUE(compile_expression_prefix("my_var"));
    ASSERT_TRUE(compile_expression_prefix("--my_var"));
    ASSERT_TRUE(compile_expression_prefix("----my_var"));
}

TEST(expression, expression_prefix_both)
{
    using namespace test;

    auto tokens = make_tokens(
        make_symbol(halberd::lexer::symbol::op_increment),
        make_symbol(halberd::lexer::symbol::op_decrement),
        make_symbol(halberd::lexer::symbol::op_increment),
        make_symbol(halberd::lexer::symbol::op_decrement),
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_prefix(std::move(tokens)));
}

TEST(expression, expression_prefix_both_src)
{
    ASSERT_TRUE(compile_expression_prefix("my_var"));
    ASSERT_TRUE(compile_expression_prefix("--++my_var"));
    ASSERT_TRUE(compile_expression_prefix("--++--++my_var"));
}

TEST(expression, expression_increment_both)
{
    using namespace test;

    // The postfix increment operator has a higher precedence than the prefix increment
    // operator so this is parsed as if the following parentheses are present: ++(it++)
    auto tokens = make_tokens(
        make_symbol(halberd::lexer::symbol::op_increment),
        make_identifier("my_var"),
        make_symbol(halberd::lexer::symbol::op_increment));

    auto result = compile_expression_prefix(std::move(tokens));

    ASSERT_TRUE(result);
    ASSERT_TRUE(result.get());

    const auto& node = *(result.get());
    const auto& op_unary_prefix = dynamic_cast<const halberd::syntax::operator_unary_prefix&>(node);

    ASSERT_EQ(halberd::syntax::operator_unary_prefix_id::increment, op_unary_prefix.operator_id);

    const auto& node_child = op_unary_prefix.get_operand();
    const auto& op_unary_postfix = dynamic_cast<const halberd::syntax::operator_unary_postfix&>(node_child);

    ASSERT_EQ(halberd::syntax::operator_unary_postfix_id::increment, op_unary_postfix.operator_id);
}

TEST(expression, expression_increment_both_src)
{
    ASSERT_TRUE(compile_expression_prefix("my_var"));
    ASSERT_TRUE(compile_expression_prefix("++my_var++"));
    ASSERT_TRUE(compile_expression_prefix("++++my_var++++"));
}

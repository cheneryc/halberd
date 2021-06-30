#include "compiler_test_common.h"

// halberd::compiler
#include <halberd/compiler.h>

// halberd::syntax
#include <halberd/operator_binary.h>
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

    auto compile_expression_multiplicative(const char* src)
    {
        return ns::compile_rule(ns::rule::expression_multiplicative, src);
    }

    auto compile_expression_multiplicative(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
    {
        return ns::compile_rule(ns::rule::expression_multiplicative, std::move(tokens));
    }
}

TEST(compile_expression, primary_src)
{
    ASSERT_TRUE(compile_expression("1.0"));
    ASSERT_TRUE(compile_expression("(1.0)"));
    ASSERT_TRUE(compile_expression("x"));
}

TEST(compile_expression, postfix_passthrough)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_postfix(std::move(tokens)));
}

TEST(compile_expression, postfix_inc)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"),
        make_symbol(halberd::lexer::symbol::op_increment));

    ASSERT_TRUE(compile_expression_postfix(std::move(tokens)));
}

TEST(compile_expression, postfix_inc_multiple)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"),
        make_symbol(halberd::lexer::symbol::op_increment),
        make_symbol(halberd::lexer::symbol::op_increment));

    ASSERT_TRUE(compile_expression_postfix(std::move(tokens)));
}

TEST(compile_expression, postfix_inc_src)
{
    ASSERT_TRUE(compile_expression_postfix("my_var"));
    ASSERT_TRUE(compile_expression_postfix("my_var++"));
    ASSERT_TRUE(compile_expression_postfix("my_var++++"));
}

TEST(compile_expression, postfix_dec)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"),
        make_symbol(halberd::lexer::symbol::op_decrement));

    ASSERT_TRUE(compile_expression_postfix(std::move(tokens)));
}

TEST(compile_expression, postfix_dec_multiple)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"),
        make_symbol(halberd::lexer::symbol::op_decrement),
        make_symbol(halberd::lexer::symbol::op_decrement));

    ASSERT_TRUE(compile_expression_postfix(std::move(tokens)));
}

TEST(compile_expression, postfix_dec_src)
{
    ASSERT_TRUE(compile_expression_postfix("my_var"));
    ASSERT_TRUE(compile_expression_postfix("my_var--"));
    ASSERT_TRUE(compile_expression_postfix("my_var----"));
}

TEST(compile_expression, postfix_both)
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

TEST(compile_expression, postfix_both_src)
{
    ASSERT_TRUE(compile_expression_postfix("my_var"));
    ASSERT_TRUE(compile_expression_postfix("my_var++--"));
    ASSERT_TRUE(compile_expression_postfix("my_var++--++--"));
}

TEST(compile_expression, prefix_passthrough)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_prefix(std::move(tokens)));
}

TEST(compile_expression, prefix_inc)
{
    using namespace test;

    auto tokens = make_tokens(
        make_symbol(halberd::lexer::symbol::op_increment),
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_prefix(std::move(tokens)));
}

TEST(compile_expression, prefix_inc_multiple)
{
    using namespace test;

    auto tokens = make_tokens(
        make_symbol(halberd::lexer::symbol::op_increment),
        make_symbol(halberd::lexer::symbol::op_increment),
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_prefix(std::move(tokens)));
}

TEST(compile_expression, prefix_inc_src)
{
    ASSERT_TRUE(compile_expression_prefix("my_var"));
    ASSERT_TRUE(compile_expression_prefix("++my_var"));
    ASSERT_TRUE(compile_expression_prefix("++++my_var"));
}

TEST(compile_expression, prefix_dec)
{
    using namespace test;

    auto tokens = make_tokens(
        make_symbol(halberd::lexer::symbol::op_decrement),
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_prefix(std::move(tokens)));
}

TEST(compile_expression, prefix_dec_multiple)
{
    using namespace test;

    auto tokens = make_tokens(
        make_symbol(halberd::lexer::symbol::op_decrement),
        make_symbol(halberd::lexer::symbol::op_decrement),
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_prefix(std::move(tokens)));
}

TEST(compile_expression, prefix_dec_src)
{
    ASSERT_TRUE(compile_expression_prefix("my_var"));
    ASSERT_TRUE(compile_expression_prefix("--my_var"));
    ASSERT_TRUE(compile_expression_prefix("----my_var"));
}

TEST(compile_expression, prefix_both)
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

TEST(compile_expression, prefix_both_src)
{
    ASSERT_TRUE(compile_expression_prefix("my_var"));
    ASSERT_TRUE(compile_expression_prefix("--++my_var"));
    ASSERT_TRUE(compile_expression_prefix("--++--++my_var"));
}

TEST(compile_expression, increment_both)
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

TEST(compile_expression, increment_both_src)
{
    ASSERT_TRUE(compile_expression_prefix("my_var"));
    ASSERT_TRUE(compile_expression_prefix("++my_var++"));
    ASSERT_TRUE(compile_expression_prefix("++++my_var++++"));
}

TEST(compile_expression, multiplicative_passthrough)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_multiplicative(std::move(tokens)));
}

TEST(compile_expression, multiplicative_asterisk)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("a"),
        make_symbol(halberd::lexer::symbol::asterisk),
        make_identifier("b"));

    ASSERT_TRUE(compile_expression_multiplicative(std::move(tokens)));
}

TEST(compile_expression, multiplicative_asterisk_multiple)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("a"),
        make_symbol(halberd::lexer::symbol::asterisk),
        make_identifier("b"),
        make_symbol(halberd::lexer::symbol::asterisk),
        make_identifier("c"));

    ASSERT_TRUE(compile_expression_multiplicative(std::move(tokens)));
}

TEST(compile_expression, multiplicative_slash)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("a"),
        make_symbol(halberd::lexer::symbol::slash),
        make_identifier("b"));

    ASSERT_TRUE(compile_expression_multiplicative(std::move(tokens)));
}

TEST(compile_expression, multiplicative_slash_multiple)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("a"),
        make_symbol(halberd::lexer::symbol::slash),
        make_identifier("b"),
        make_symbol(halberd::lexer::symbol::slash),
        make_identifier("c"));

    ASSERT_TRUE(compile_expression_multiplicative(std::move(tokens)));
}

TEST(compile_expression, multiplicative_percent)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("a"),
        make_symbol(halberd::lexer::symbol::sign_percent),
        make_identifier("b"));

    ASSERT_TRUE(compile_expression_multiplicative(std::move(tokens)));
}

TEST(compile_expression, multiplicative_percent_multiple)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("a"),
        make_symbol(halberd::lexer::symbol::sign_percent),
        make_identifier("b"),
        make_symbol(halberd::lexer::symbol::sign_percent),
        make_identifier("c"));

    ASSERT_TRUE(compile_expression_multiplicative(std::move(tokens)));
}

TEST(compile_expression, multiplicative_associativity)
{
    using namespace test;

    // Ensure the operator associativity is left-to-right
    auto tokens = make_tokens(
        make_identifier("a"),
        make_symbol(halberd::lexer::symbol::asterisk),
        make_identifier("b"),
        make_symbol(halberd::lexer::symbol::slash),
        make_identifier("c"),
        make_symbol(halberd::lexer::symbol::sign_percent),
        make_identifier("d"));

    auto result = compile_expression_multiplicative(std::move(tokens));

    ASSERT_TRUE(result);
    ASSERT_TRUE(result.get());

    const auto& node_rem = *(result.get());
    const auto& op_binary_rem = dynamic_cast<const halberd::syntax::operator_binary&>(node_rem);

    ASSERT_EQ(halberd::syntax::operator_binary_id::remainder, op_binary_rem.operator_id);

    const auto& node_div = op_binary_rem.get_operand_lhs();
    const auto& op_binary_div = dynamic_cast<const halberd::syntax::operator_binary&>(node_div);

    ASSERT_EQ(halberd::syntax::operator_binary_id::division, op_binary_div.operator_id);

    const auto& node_mul = op_binary_div.get_operand_lhs();
    const auto& op_binary_mul = dynamic_cast<const halberd::syntax::operator_binary&>(node_mul);

    ASSERT_EQ(halberd::syntax::operator_binary_id::multiplication, op_binary_mul.operator_id);
}

TEST(compile_expression, multiplicative_src)
{
    ASSERT_TRUE(compile_expression_multiplicative("a"));
    ASSERT_TRUE(compile_expression_multiplicative("a * b"));
    ASSERT_TRUE(compile_expression_multiplicative("a * b / c"));
    ASSERT_TRUE(compile_expression_multiplicative("a * b / c % d"));
}

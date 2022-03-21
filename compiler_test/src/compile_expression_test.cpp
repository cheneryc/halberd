#include "compiler_test_common.h"

// halberd::compiler
#include <halberd/compiler.h>

// halberd::syntax
#include <halberd/operator_assignment.h>
#include <halberd/operator_binary.h>
#include <halberd/operator_unary_prefix.h>
#include <halberd/operator_unary_postfix.h>

#include <gtest/gtest.h>


namespace
{
    namespace ns = halberd::compiler;

    auto compile_expression(const char* src)
    {
        return ns::parse(ns::rule::expression, src);
    }

    auto compile_expression(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
    {
        return ns::parse(ns::rule::expression, std::move(tokens));
    }

    auto compile_expression_terminal(const char* src)
    {
        return ns::parse(ns::rule::expression_terminal, src);
    }

    auto compile_expression_terminal(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
    {
        return ns::parse(ns::rule::expression_terminal, std::move(tokens));
    }

    auto compile_expression_postfix(const char* src)
    {
        return ns::parse(ns::rule::expression_postfix, src);
    }

    auto compile_expression_postfix(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
    {
        return ns::parse(ns::rule::expression_postfix, std::move(tokens));
    }

    auto compile_expression_prefix(const char* src)
    {
        return ns::parse(ns::rule::expression_prefix, src);
    }

    auto compile_expression_prefix(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
    {
        return ns::parse(ns::rule::expression_prefix, std::move(tokens));
    }

    auto compile_expression_multiplicative(const char* src)
    {
        return ns::parse(ns::rule::expression_multiplicative, src);
    }

    auto compile_expression_multiplicative(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
    {
        return ns::parse(ns::rule::expression_multiplicative, std::move(tokens));
    }

    auto compile_expression_additive(const char* src)
    {
        return ns::parse(ns::rule::expression_additive, src);
    }

    auto compile_expression_additive(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
    {
        return ns::parse(ns::rule::expression_additive, std::move(tokens));
    }

    auto compile_expression_assignment(const char* src)
    {
        return ns::parse(ns::rule::expression_assignment, src);
    }

    auto compile_expression_assignment(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
    {
        return ns::parse(ns::rule::expression_assignment, std::move(tokens));
    }
}

TEST(compile_expression, terminal_fractional_value)
{
    using namespace test;

    auto tokens = make_tokens(
        make_literal_fractional(0.12345f));

    auto result = compile_expression_terminal(std::move(tokens));

    ASSERT_TRUE(result);
    ASSERT_TRUE(result.get());

    const auto& node_literal = *(result.get());
    const auto& literal_frac = dynamic_cast<const halberd::syntax::literal&>(node_literal);

    ASSERT_EQ(halberd::syntax::literal_id::fractional, literal_frac.id);
    ASSERT_EQ(0.12345f, literal_frac.get_fractional());
}

TEST(compile_expression, terminal_fractional_src)
{
    ASSERT_TRUE(compile_expression_terminal("0."));
    ASSERT_TRUE(compile_expression_terminal(".0"));
    ASSERT_TRUE(compile_expression_terminal("0.0"));
}

TEST(compile_expression, terminal_integer_value)
{
    using namespace test;

    auto tokens = make_tokens(
        make_literal_integer(123456789));

    auto result = compile_expression_terminal(std::move(tokens));

    ASSERT_TRUE(result);
    ASSERT_TRUE(result.get());

    const auto& node_literal = *(result.get());
    const auto& literal_int = dynamic_cast<const halberd::syntax::literal&>(node_literal);

    ASSERT_EQ(halberd::syntax::literal_id::integer, literal_int.id);
    ASSERT_EQ(123456789, literal_int.get_integer());
}

TEST(compile_expression, terminal_integer_src)
{
    ASSERT_TRUE(compile_expression_terminal("1"));
    ASSERT_TRUE(compile_expression_terminal("12"));
    ASSERT_TRUE(compile_expression_terminal("123"));
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

TEST(compile_expression, additive_passthrough)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_additive(std::move(tokens)));
}

TEST(compile_expression, additive_plus)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("a"),
        make_symbol(halberd::lexer::symbol::sign_plus),
        make_identifier("b"));

    ASSERT_TRUE(compile_expression_additive(std::move(tokens)));
}

TEST(compile_expression, additive_plus_multiple)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("a"),
        make_symbol(halberd::lexer::symbol::sign_plus),
        make_identifier("b"),
        make_symbol(halberd::lexer::symbol::sign_plus),
        make_identifier("c"));

    ASSERT_TRUE(compile_expression_additive(std::move(tokens)));
}

TEST(compile_expression, additive_minus)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("a"),
        make_symbol(halberd::lexer::symbol::sign_minus),
        make_identifier("b"));

    ASSERT_TRUE(compile_expression_additive(std::move(tokens)));
}

TEST(compile_expression, additive_minus_multiple)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("a"),
        make_symbol(halberd::lexer::symbol::sign_minus),
        make_identifier("b"),
        make_symbol(halberd::lexer::symbol::sign_minus),
        make_identifier("c"));

    ASSERT_TRUE(compile_expression_additive(std::move(tokens)));
}

TEST(compile_expression, additive_associativity)
{
    using namespace test;

    // Ensure the operator associativity is left-to-right
    auto tokens = make_tokens(
        make_identifier("a"),
        make_symbol(halberd::lexer::symbol::sign_plus),
        make_identifier("b"),
        make_symbol(halberd::lexer::symbol::sign_minus),
        make_identifier("c"));

    auto result = compile_expression_additive(std::move(tokens));

    ASSERT_TRUE(result);
    ASSERT_TRUE(result.get());

    const auto& node_sub = *(result.get());
    const auto& op_binary_sub = dynamic_cast<const halberd::syntax::operator_binary&>(node_sub);

    ASSERT_EQ(halberd::syntax::operator_binary_id::subtraction, op_binary_sub.operator_id);

    const auto& node_add = op_binary_sub.get_operand_lhs();
    const auto& op_binary_add = dynamic_cast<const halberd::syntax::operator_binary&>(node_add);

    ASSERT_EQ(halberd::syntax::operator_binary_id::addition, op_binary_add.operator_id);
}

TEST(compile_expression, additive_src)
{
    ASSERT_TRUE(compile_expression_additive("a"));
    ASSERT_TRUE(compile_expression_additive("a + b"));
    ASSERT_TRUE(compile_expression_additive("a + b - c"));
}

TEST(compile_expression, assignment_passthrough)
{
    using namespace test;

    auto tokens = make_tokens(
        make_identifier("my_var"));

    ASSERT_TRUE(compile_expression_assignment(std::move(tokens)));
}

//TODO: add the remaining tests for assignment

TEST(compile_expression, assignment_associativity)
{
    using namespace test;

    // Ensure the operator associativity is right-to-left
    auto tokens = make_tokens(
        make_identifier("a"),
        make_symbol(halberd::lexer::symbol::sign_equals),
        make_identifier("b"),
        make_symbol(halberd::lexer::symbol::assign_plus),
        make_identifier("c"),
        make_symbol(halberd::lexer::symbol::assign_minus),
        make_identifier("d"));

    auto result = compile_expression_assignment(std::move(tokens));

    ASSERT_TRUE(result);
    ASSERT_TRUE(result.get());

    const auto& node_eq = *(result.get());
    const auto& op_assign = dynamic_cast<const halberd::syntax::operator_assignment&>(node_eq);

    ASSERT_EQ(halberd::syntax::operator_assignment_id::assignment, op_assign.operator_id);

    const auto& node_add = op_assign.get_operand_rhs();
    const auto& op_assign_add = dynamic_cast<const halberd::syntax::operator_assignment&>(node_add);

    ASSERT_EQ(halberd::syntax::operator_assignment_id::compound_addition, op_assign_add.operator_id);

    const auto& node_sub = op_assign_add.get_operand_rhs();
    const auto& op_assign_sub = dynamic_cast<const halberd::syntax::operator_assignment&>(node_sub);

    ASSERT_EQ(halberd::syntax::operator_assignment_id::compound_subtraction, op_assign_sub.operator_id);
}

TEST(compile_expression, assignment_src)
{
    ASSERT_TRUE(compile_expression_assignment("a"));
    ASSERT_TRUE(compile_expression_assignment("a = b"));
    ASSERT_TRUE(compile_expression_assignment("a = b = c"));
    ASSERT_TRUE(compile_expression_assignment("a += b"));
    ASSERT_TRUE(compile_expression_assignment("a += b += c"));
    ASSERT_TRUE(compile_expression_assignment("a -= b"));
    ASSERT_TRUE(compile_expression_assignment("a -= b -= c"));
    ASSERT_TRUE(compile_expression_assignment("a *= b"));
    ASSERT_TRUE(compile_expression_assignment("a *= b *= c"));
    ASSERT_TRUE(compile_expression_assignment("a /= b"));
    ASSERT_TRUE(compile_expression_assignment("a /= b /= c"));
    ASSERT_TRUE(compile_expression_assignment("a %= b"));
    ASSERT_TRUE(compile_expression_assignment("a %= b %= c"));
}

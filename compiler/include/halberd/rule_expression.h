#pragma once

#include "rule_common.h"
#include "transform/transform_operator_binary.h"
#include "transform/transform_operator_unary_postfix.h"
#include "transform/transform_operator_unary_prefix.h"

// halberd::lexer
#include <halberd/symbol.h>

// halberd::parser
#include <halberd/combinator_function.h>
#include <halberd/combinator_optional.h>
#include <halberd/combinator_operators.h>
#include <halberd/index_tag.h>
#include <halberd/parse_result.h>
#include <halberd/source.h>

// halberd::syntax
#include <halberd/expression.h>

#include <iterator> // std::make_move_iterator
#include <memory> // std::unique_ptr
#include <tuple> // std::tuple, std::tie
#include <vector> // std::vector



namespace halberd
{
namespace compiler
{
    // Parser forward declarations

    template<typename T, typename R>
    constexpr parser::parse_result<std::unique_ptr<syntax::expression>> parse_expression(parser::source<T, R>& source);

    template<typename T, typename R>
    constexpr auto parser_expression_v = parser::combinator_function_v<decltype(parse_expression<T, R>), &parse_expression<T, R>>;

    // Parser rules

    /*
        <expression_terminal> ::= IDENTIFIER | LITERAL_FRACTIONAL | LITERAL_INTEGER
     */

    constexpr auto parser_expression_terminal_v = (
        filter_identifier_v |
        filter_literal_fractional_v |
        filter_literal_integer_v) >> syntax::expression_transform(); //TODO: implement a literal syntax node and transform type

    /*
        <expression_parentheses> ::= SYMBOL('(') <expression> SYMBOL(')')
     */

    template<typename T, typename R>
    constexpr auto parser_expression_parentheses_v =
        match_symbol_v<lexer::symbol::bracket_round_open> +
        parser_expression_v<T, R> +
        match_symbol_v<lexer::symbol::bracket_round_close>;

    /*
        <expression_primary> ::= <expression_terminal> | <expression_parentheses>
     */

    template<typename T, typename R>
    constexpr auto parser_expression_primary_v =
        parser_expression_terminal_v |
        parser_expression_parentheses_v<T, R>[parser::index_tag_v<1U>];

    /*
        <expression_postfix> ::= <expression_postfix> + SYMBOL('++') | <expression_postfix> + SYMBOL('--') | <expression_primary>

        // Remove left recursion by using the 'repeat' operator

        <operator_postfix> ::= SYMBOL('++') | SYMBOL('--')
        <expression_postfix> ::= <expression_primary> + *<operator_postfix>
     */

    namespace detail
    {
        constexpr auto parser_operator_postfix_v =
            match_symbol_v<lexer::symbol::op_increment> |
            match_symbol_v<lexer::symbol::op_decrement>;
    }

    template<typename T, typename R>
    constexpr auto parser_expression_postfix_v = (parser_expression_primary_v<T, R> + *detail::parser_operator_postfix_v) >> transform::transform_operator_unary_postfix();

    /*
        <expression_prefix> ::= SYMBOL('++') + <expression_prefix> | SYMBOL('--') + <expression_prefix> | <expression_postfix>

        // Remove left recursion by using the 'repeat' operator

        <operator_prefix> ::= SYMBOL('++') | SYMBOL('--')
        <expression_prefix> ::= *<operator_prefix> + <expression_postfix>
     */

    namespace detail
    {
        constexpr auto parser_operator_prefix_v =
            match_symbol_v<lexer::symbol::op_increment> |
            match_symbol_v<lexer::symbol::op_decrement>;
    }

    template<typename T, typename R>
    constexpr auto parser_expression_prefix_v = (*detail::parser_operator_postfix_v + parser_expression_postfix_v<T, R>) >> transform::transform_operator_unary_prefix();

    /*
        <expression_multiplicative> ::= <expression_multiplicative> + SYMBOL('*') + <expression_prefix> |
                                        <expression_multiplicative> + SYMBOL('/') + <expression_prefix> |
                                        <expression_multiplicative> + SYMBOL('%') + <expression_prefix> |
                                        <expression_prefix>

        // Remove left recursion by using the 'repeat' operator

        <operator_multiplicative> ::= SYMBOL('*') | SYMBOL('/') | SYMBOL('%')
        <operator_multiplicative_expression_rhs> ::= <operator_multiplicative> + <expression_prefix>
        <expression_multiplicative> ::= <expression_prefix> + *<operator_multiplicative_expression_rhs>
     */

    namespace detail
    {
        constexpr auto parser_operator_multiplicative_v =
            match_symbol_v<lexer::symbol::asterisk> |
            match_symbol_v<lexer::symbol::slash> |
            match_symbol_v<lexer::symbol::sign_percent>;

        template<typename T, typename R>
        constexpr auto parser_operator_multiplicative_expression_rhs_v = parser_operator_multiplicative_v + parser_expression_prefix_v<T, R>;
    }

    template<typename T, typename R>
    constexpr auto parser_expression_multiplicative_v = (parser_expression_prefix_v<T, R> + *detail::parser_operator_multiplicative_expression_rhs_v<T, R>) >> transform::transform_operator_binary();

    /*
        <expression_additive> ::= <expression_additive> + SYMBOL('+') + <expression_multiplicative> |
                                  <expression_additive> + SYMBOL('-') + <expression_multiplicative> |
                                  <expression_multiplicative>

        // Remove left recursion by using the 'repeat' operator

        <operator_additive> ::= SYMBOL('+') | SYMBOL('-')
        <operator_additive_expression_rhs> ::= <operator_additive> + <expression_multiplicative>
        <expression_additive> ::= <expression_multiplicative> + *<operator_additive_expression_rhs>
     */

    namespace detail
    {
        constexpr auto parser_operator_additive_v =
            match_symbol_v<lexer::symbol::sign_plus> |
            match_symbol_v<lexer::symbol::sign_minus>;

        template<typename T, typename R>
        constexpr auto parser_operator_additive_expression_rhs_v = parser_operator_additive_v + parser_expression_multiplicative_v<T, R>;
    }

    template<typename T, typename R>
    constexpr auto parser_expression_additive_v = (parser_expression_multiplicative_v<T, R> + *detail::parser_operator_additive_expression_rhs_v<T, R>) >> transform::transform_operator_binary();

    // Parser function definitions

    /*
        <expression> ::= <expression_additive>
     */

    template<typename T, typename R>
    constexpr parser::parse_result<std::unique_ptr<syntax::expression>> parse_expression(parser::source<T, R>& source)
    {
        return parser_expression_additive_v<T, R>.apply(source);
    }

    // Parser factories

    template<typename T, typename R>
    constexpr auto make_parser_expression()
    {
        return parser::combinator_function_v<decltype(parse_expression<T, R>), &parse_expression<T, R>> + parser_end_v;
    }

    template<typename T, typename R>
    constexpr auto make_parser_expression_postfix()
    {
        return parser_expression_postfix_v<T, R> + parser_end_v;
    }

    template<typename T, typename R>
    constexpr auto make_parser_expression_prefix()
    {
        return parser_expression_prefix_v<T, R> + parser_end_v;
    }

    template<typename T, typename R>
    constexpr auto make_parser_expression_multiplicative()
    {
        return parser_expression_multiplicative_v<T, R> + parser_end_v;
    }

    template<typename T, typename R>
    constexpr auto make_parser_expression_additive()
    {
        return parser_expression_additive_v<T, R> + parser_end_v;
    }
}
}

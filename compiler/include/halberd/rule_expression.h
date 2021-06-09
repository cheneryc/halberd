#pragma once

#include "rule_common.h"
#include "expression_postfix_transform.h"

// halberd::lexer
#include <halberd/symbol.h>

// halberd::parser
#include <halberd/combinator_function.h>
#include <halberd/combinator_operators.h>
#include <halberd/index_tag.h>
#include <halberd/parse_result.h>
#include <halberd/source.h>

// halberd::syntax
#include <halberd/expression.h>

#include <memory> // std::unique_ptr


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
        filter_literal_integer_v) >> syntax::expression_transform();

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
    constexpr auto parser_expression_postfix_v = (parser_expression_primary_v<T, R> + *detail::parser_operator_postfix_v) >> expression_postfix_transform();

    // Parser function definitions

    /*
        <expression> ::= <expression_postfix>
     */

    template<typename T, typename R>
    constexpr parser::parse_result<std::unique_ptr<syntax::expression>> parse_expression(parser::source<T, R>& source)
    {
        return parser_expression_postfix_v<T, R>.apply(source);
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
}
}

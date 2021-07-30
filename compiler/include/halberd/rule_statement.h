#pragma once

#include "rule_common.h"
#include "rule_expression.h"
#include "rule_variable_declaration.h"
#include "transform/transform_declaration_statement.h"
#include "transform/transform_expression_statement.h"
#include "transform/transform_statement_list.h"

// halberd::lexer
#include <halberd/symbol.h>

// halberd::parser
#include <halberd/combinator_operators.h>
#include <halberd/index_tag.h>

// halberd::syntax
#include <halberd/variable_declaration.h>
#include <halberd/variable_declaration_statement.h>


namespace halberd
{
namespace compiler
{
    // Parser rules

    /*
        <statement_declaration> ::= <variable_declaration> + SYMBOL(';')
     */

    namespace detail
    {
        constexpr transform::transform_declaration_statement<syntax::variable_declaration, syntax::variable_declaration_statement> transform_variable_declaration_statement_v;
    }

    constexpr auto parser_statement_declaration_v = (parser_variable_declaration_v + match_symbol_v<lexer::symbol::semicolon>)[parser::index_tag_v<0U>] >> detail::transform_variable_declaration_statement_v;

    /*
        <statement_expression> ::= <expression> + SYMBOL(';')
     */

    template<typename T, typename R>
    constexpr auto parser_statement_expression_v = (parser_expression_v<T, R> + match_symbol_v<lexer::symbol::semicolon>)[parser::index_tag_v<0U>] >> transform::transform_expression_statement();

    /*
        <statement> ::= <statement_declaration> | <statement_expression>
     */

    template<typename T, typename R>
    constexpr auto parser_statement_v =
        parser_statement_declaration_v |
        parser_statement_expression_v<T, R>;

    /*
        <statement_list> ::= <statement> | <statement_list> + <statement> | E

        // Remove recursion by using the 'repeat' operator

        <statement_list> ::= *<statement>
     */

    template<typename T, typename R>
    constexpr auto parser_statement_list_v = *parser_statement_v<T, R> >> transform::transform_statement_list();

    // Parser factories

    template<typename T, typename R>
    constexpr auto make_parser_statement()
    {
        return parser_statement_v<T, R> + parser_end_v;
    }

    template<typename T, typename R>
    constexpr auto make_parser_statement_list()
    {
        return parser_statement_list_v<T, R> + parser_end_v;
    }
}
}

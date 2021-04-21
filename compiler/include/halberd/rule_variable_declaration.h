#pragma once

#include "rule_common.h"

// halberd::lexer
#include <halberd/keyword.h>

// halberd::parser
#include <halberd/combinator_operators.h>

// halberd::syntax
#include <halberd/variable_declaration.h>


namespace halberd
{
namespace compiler
{
    // Parser rules

    /*
        <type_integer> ::= KEYWORD("i8")  |
                           KEYWORD("i16") |
                           KEYWORD("i32") |
                           KEYWORD("i64")
     */

    constexpr auto parser_type_integer_v =
        match_keyword_v<lexer::keyword::strict_i8>  |
        match_keyword_v<lexer::keyword::strict_i16> |
        match_keyword_v<lexer::keyword::strict_i32> |
        match_keyword_v<lexer::keyword::strict_i64>;

    /*
        <variable_declaration> ::= KEYWORD('var') <type_integer> IDENTIFIER
     */

    constexpr auto parser_variable_declaration_v = (
        match_keyword_v<lexer::keyword::strict_var> +
        parser_type_integer_v +
        match_identifier_v +
        parser_end_v) >> syntax::variable_declaration_transform();

    // Parser factory

    constexpr auto make_parser_variable_declaration()
    {
        return parser_variable_declaration_v;
    }
}
}

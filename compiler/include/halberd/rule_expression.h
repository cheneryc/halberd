#pragma once

#include "rule_common.h"
#include "expression_postfix_transform.h"

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
#include <tuple> // std::tie
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

        // Remove left recursion by addition of a 'tail' nonterminal

        <expression_postfix_tail> ::= SYMBOL('++') + <expression_postfix_tail> | SYMBOL('--') + <expression_postfix_tail> | E
        <expression_postfix> ::= <expression_primary> + <expression_postfix_tail>
     */

    namespace detail
    {
        template<typename T, typename R>
        constexpr parser::parse_result<std::vector<lexer::symbol>> parse_expression_postfix_tail(parser::source<T, R>& source)
        {
            // Required for recursive parsing
            constexpr auto parser_expression_postfix_tail = parser::combinator_function_v<decltype(parse_expression_postfix_tail<T, R>), &parse_expression_postfix_tail<T, R>>;

            // Making the rule optional is equivalent to accepting the empty string
            constexpr auto parser = parser::make_optional((match_symbol_v<lexer::symbol::op_increment> + parser_expression_postfix_tail) |
                                                          (match_symbol_v<lexer::symbol::op_decrement> + parser_expression_postfix_tail));

            lexer::symbol symbol;

            std::vector<lexer::symbol> symbols;
            std::vector<lexer::symbol> symbols_rec;

            auto result = parser.apply(source);

            if (auto& result_opt = result.get()) // combinator_optional always succeeds so no need to test the result for success
            {
                std::tie(symbol, symbols_rec) = std::move(result_opt.get());

                symbols.reserve(symbols_rec.size() + 1U);
                symbols.insert(symbols.end(), symbol);
                symbols.insert(symbols.end(),
                    std::make_move_iterator(symbols_rec.begin()),
                    std::make_move_iterator(symbols_rec.end()));
            }

            return { std::move(symbols) };
        }

        template<typename T, typename R>
        constexpr auto parser_expression_postfix_tail_v = parser::combinator_function_v<decltype(parse_expression_postfix_tail<T, R>), &parse_expression_postfix_tail<T, R>>;
    }

    template<typename T, typename R>
    constexpr auto parser_expression_postfix_v = (parser_expression_primary_v<T, R> + detail::parser_expression_postfix_tail_v<T, R>) >> expression_postfix_transform();

    // Parser function definitions

    /*
        <expression> ::= <expression_postfix>
     */

    template<typename T, typename R>
    constexpr parser::parse_result<std::unique_ptr<syntax::expression>> parse_expression(parser::source<T, R>& source)
    {
        return parser_expression_postfix_v<T, R>.apply(source);
    }

    // Parser factory

    template<typename T, typename R>
    constexpr auto make_parser_expression()
    {
        return parser::combinator_function_v<decltype(parse_expression<T, R>), &parse_expression<T, R>> + parser_end_v;
    }
}
}

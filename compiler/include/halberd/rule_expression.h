#pragma once

#include "rule_common.h"
#include "expression_multiplicative_transform.h"
#include "expression_postfix_transform.h"
#include "expression_prefix_transform.h"

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
    constexpr auto parser_expression_prefix_v = (*detail::parser_operator_postfix_v + parser_expression_postfix_v<T, R>) >> expression_prefix_transform();

    /*
        <expression_multiplicative> ::= <expression_multiplicative> + SYMBOL('*') + <expression_prefix> |
                                        <expression_multiplicative> + SYMBOL('/') + <expression_prefix> |
                                        <expression_multiplicative> + SYMBOL('%') + <expression_prefix> |
                                        <expression_prefix>

        // Remove left recursion by addition of a 'tail' nonterminal

        <expression_multiplicative_tail> ::= SYMBOL('*') + <expression_prefix> + <expression_multiplicative_tail> |
                                             SYMBOL('/') + <expression_prefix> + <expression_multiplicative_tail> |
                                             SYMBOL('%') + <expression_prefix> + <expression_multiplicative_tail> |
                                             E
        <expression_multiplicative> ::= <expression_prefix> + <expression_multiplicative_tail>
     */

    namespace detail
    {
        template<typename T, typename R>
        constexpr parser::parse_result<std::vector<std::tuple<lexer::symbol, std::unique_ptr<syntax::expression>>>> parse_expression_multiplicative_tail(parser::source<T, R>& source)
        {
            // Required for recursive parsing
            constexpr auto parser_expression_multiplicative_tail = parser::combinator_function_v<decltype(parse_expression_multiplicative_tail<T, R>), &parse_expression_multiplicative_tail<T, R>>;

            // Making the rule optional is equivalent to accepting the empty string
            constexpr auto parser = parser::make_optional((match_symbol_v<lexer::symbol::asterisk>     + parser_expression_prefix_v<T, R> + parser_expression_multiplicative_tail) |
                                                          (match_symbol_v<lexer::symbol::slash>        + parser_expression_prefix_v<T, R> + parser_expression_multiplicative_tail) |
                                                          (match_symbol_v<lexer::symbol::sign_percent> + parser_expression_prefix_v<T, R> + parser_expression_multiplicative_tail));

            using sym_exp_pair_t = std::tuple<lexer::symbol, std::unique_ptr<syntax::expression>>;

            std::vector<sym_exp_pair_t> sym_exp_pairs;
            std::vector<sym_exp_pair_t> sym_exp_pairs_rec;

            auto result = parser.apply(source);

            if (auto& result_opt = result.get()) // combinator_optional always succeeds so no need to test the result for success
            {
                sym_exp_pair_t sym_exp_pair;

                std::tie(std::get<0U>(sym_exp_pair),
                         std::get<1U>(sym_exp_pair),
                         sym_exp_pairs_rec) = std::move(result_opt.get());

                sym_exp_pairs.reserve(sym_exp_pairs_rec.size() + 1U);
                sym_exp_pairs.insert(sym_exp_pairs.end(), std::move(sym_exp_pair));
                sym_exp_pairs.insert(sym_exp_pairs.end(),
                    std::make_move_iterator(sym_exp_pairs_rec.begin()),
                    std::make_move_iterator(sym_exp_pairs_rec.end()));
            }

            return { std::move(sym_exp_pairs) };
        }

        template<typename T, typename R>
        constexpr auto parser_expression_multiplicative_tail_v = parser::combinator_function_v<decltype(parse_expression_multiplicative_tail<T, R>), &parse_expression_multiplicative_tail<T, R>>;
    }

    template<typename T, typename R>
    constexpr auto parser_expression_multiplicative_v = (parser_expression_prefix_v<T, R> + detail::parser_expression_multiplicative_tail_v<T, R>) >> expression_multiplicative_transform();

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
    constexpr auto parser_expression_additive_v = (parser_expression_multiplicative_v<T, R> + *detail::parser_operator_additive_expression_rhs_v<T, R>) >> expression_multiplicative_transform();

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

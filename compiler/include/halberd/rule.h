#pragma once

#include "rule_expression.h"
#include "rule_variable_declaration.h"

// halberd::parser
#include <halberd/combinator_any.h>
#include <halberd/source.h>

// halberd::syntax
#include <halberd/node.h>

#include <memory> // std::unique_ptr
#include <stdexcept> // std::logic_error


namespace halberd
{
namespace compiler
{
    enum class rule
    {
        expression,
        expression_terminal,
        expression_postfix,
        expression_prefix,
        expression_multiplicative,
        expression_additive,
        expression_assignment,
        variable_declaration
    };

    template<typename T, typename R>
    parser::combinator_any<parser::source<T, R>, std::unique_ptr<syntax::node>> make_rule_parser(rule r)
    {
        switch(r)
        {
            case rule::expression:
                return make_parser_expression<T, R>();
            case rule::expression_terminal:
                return make_parser_expression_terminal();
            case rule::expression_postfix:
                return make_parser_expression_postfix<T, R>();
            case rule::expression_prefix:
                return make_parser_expression_prefix<T, R>();
            case rule::expression_multiplicative:
                return make_parser_expression_multiplicative<T, R>();
            case rule::expression_additive:
                return make_parser_expression_additive<T, R>();
            case rule::expression_assignment:
                return make_parser_expression_assignment<T, R>();
            case rule::variable_declaration:
                return make_parser_variable_declaration();
        }

        throw std::logic_error("unexpected value for halberd::compiler::rule enumeration");
    }
}
}

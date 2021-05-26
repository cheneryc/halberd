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
    //TODO: add rule for expression_postfix and update relevant unit tests to use it
    enum class rule
    {
        expression,
        variable_declaration
    };

    template<typename T, typename R>
    parser::combinator_any<parser::source<T, R>, std::unique_ptr<syntax::node>> make_rule_parser(rule r)
    {
        switch(r)
        {
            case rule::expression:
                return make_parser_expression<T, R>();
            case rule::variable_declaration:
                return make_parser_variable_declaration();
        }

        throw std::logic_error("unexpected value for halberd::compiler::rule enumeration");
    }
}
}

#include <halberd/compiler.h>

// halberd::lexer
#include <halberd/lexer.h>
#include <halberd/scanner.h>

// halberd::parser
#include <halberd/source.h>

// halberd::syntax
#include <halberd/identifier_expression.h>
#include <halberd/operator_assignment.h>
#include <halberd/operator_binary.h>
#include <halberd/operator_unary_postfix.h>
#include <halberd/operator_unary_prefix.h>
#include <halberd/variable_declaration.h>
#include <halberd/variable_identifier.h>
#include <halberd/visit.h>
#include <halberd/visitor_antecedent.h>
#include <halberd/visitor_function.h>
#include <halberd/visitor_reset.h>

#include <map> // std::map
#include <string> // std::string
#include <iterator> // std::make_move_iterator
#include <type_traits> // std::underlying_type_t


namespace
{
    namespace ns = halberd::compiler;

    std::string debug_token(halberd::lexer::token* token)
    {
        std::stringstream ss;

        if (!token)
        {
            ss << "NULL";
        }
        else if (auto token_id = dynamic_cast<halberd::lexer::token_identifier*>(token))
        {
            ss << "IDENTIFIER, " << token_id->_identifier;
        }
        else if (auto token_res = dynamic_cast<halberd::lexer::token_identifier_reserved*>(token))
        {
            ss << "KEYWORD, " << static_cast<std::underlying_type_t<halberd::lexer::keyword>>(token_res->_keyword);
        }
        else if (auto token_frac = dynamic_cast<halberd::lexer::token_literal_fractional*>(token))
        {
            ss << "FRACTIONAL, " << token_frac->_value;
        }
        else if (auto token_int = dynamic_cast<halberd::lexer::token_literal_integer*>(token))
        {
            ss << "INTEGER, " << token_int->_value;
        }
        else if (auto token_sym = dynamic_cast<halberd::lexer::token_symbol*>(token))
        {
            ss << "SYMBOL, " << static_cast<std::underlying_type_t<halberd::lexer::symbol>>(token_sym->_symbol);
        }
        else
        {
            ss << "UNKNOWN";
        }

        return ss.str();
    }
}

bool ns::compile(const char* src)
{
    auto result = parse(rule::statement_list, src);

    if (result)
    {
        auto& node = *result.get();

        check_identifiers(node);
        check_types(node);

        return true;
    }

    return false;
}

bool ns::compile(std::vector<std::unique_ptr<lexer::token>> tokens)
{
    auto result = parse(rule::statement_list, std::move(tokens));

    if (result)
    {
        auto& node = *result.get();

        check_identifiers(node);
        check_types(node);

        return true;
    }

    return false;
}

halberd::parser::parse_result<std::unique_ptr<halberd::syntax::node>> ns::parse(rule r, const char* src)
{
    lexer::scanner scanner(lexer::get_smv_union(), src);

    auto token_source = parser::make_source(
        [&scanner]()
    {
        return scanner.scan();
    },
        [](const std::unique_ptr<lexer::token>& token_ptr)
    {
        return token_ptr.get();
    });

    using token_t = std::unique_ptr<lexer::token>;
    using token_ptr_t = lexer::token*;

    return make_rule_parser<token_t, token_ptr_t>(r).apply(token_source);
}

halberd::parser::parse_result<std::unique_ptr<halberd::syntax::node>> ns::parse(rule r, std::vector<std::unique_ptr<lexer::token>> tokens)
{
    auto token_source = parser::make_source(
        [it     = std::make_move_iterator(tokens.begin()),
         it_end = std::make_move_iterator(tokens.end())]
        () mutable
    {
        return (it != it_end) ? *(it++) : nullptr;
    },
        [](const std::unique_ptr<lexer::token>& token_ptr)
    {
        return token_ptr.get();
    });

    using token_t = std::unique_ptr<lexer::token>;
    using token_ptr_t = lexer::token*;

    return make_rule_parser<token_t, token_ptr_t>(r).apply(token_source);
}

void ns::check_identifiers(syntax::node& node)
{
    // Performs name binding - associating identifiers with the relevant entity
    std::map<std::string, syntax::variable_declaration*> identifiers;

    syntax::visitor_antecedent* visitor_ptr = nullptr;

    auto visitor = syntax::make_visitor_function<syntax::visitor_antecedent>(
        [&identifiers](syntax::variable_declaration& dec) mutable
    {
        auto result = identifiers.emplace(dec.get_name(), &dec);

        if (!result.second)
        {
            throw std::exception(); // Error, duplicate identifier
        }
    },
        [&identifiers, &visitor_ptr](syntax::identifier_expression& exp)
    {
        auto it = identifiers.find(exp.get_identifier());

        if (it == identifiers.end())
        {
            throw std::exception(); // Error, unknown identifier
        }

        syntax::visitor_reset<syntax::expression> visitor_reset(exp, std::make_unique<syntax::variable_identifier>(it->second));
        syntax::visit(visitor_reset, visitor_ptr->get_antecedent());
    });

    visitor_ptr = &visitor;

    syntax::visit(visitor, node);
}

void ns::check_types(syntax::node& node)
{
    auto visitor = syntax::make_visitor_function(
        [](syntax::operator_assignment& op, syntax::visit_end_tag)
    {
        auto& lhs_exp = op.get_operand_lhs();
        auto  lhs_type = lhs_exp.get_type();

        auto& rhs_exp = op.get_operand_rhs();
        auto  rhs_type = rhs_exp.get_type();

        if (lhs_type != rhs_type)
        {
            throw std::exception(); // Error, type mismatch
        }

        op.set_type(lhs_type);
    },
        [](syntax::operator_binary& op, syntax::visit_end_tag)
    {
        auto& lhs_exp = op.get_operand_lhs();
        auto  lhs_type = lhs_exp.get_type();

        auto& rhs_exp = op.get_operand_rhs();
        auto  rhs_type = rhs_exp.get_type();

        if (lhs_type != rhs_type)
        {
            throw std::exception(); // Error, type mismatch
        }

        op.set_type(lhs_type);
    },
        [](syntax::operator_unary_prefix& op, syntax::visit_end_tag)
    {
        op.set_type(op.get_operand().get_type()); // Propagate the operand's type to the operator
    },
        [](syntax::operator_unary_postfix& op, syntax::visit_end_tag)
    {
        op.set_type(op.get_operand().get_type()); // Propagate the operand's type to the operator
    });

    syntax::visit(visitor, node);
}

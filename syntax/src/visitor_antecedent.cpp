#include <halberd/visitor_antecedent.h>
#include <halberd/expression_statement.h>
#include <halberd/operator_assignment.h>
#include <halberd/operator_binary.h>
#include <halberd/operator_unary_postfix.h>
#include <halberd/operator_unary_prefix.h>
#include <halberd/statement_list.h>
#include <halberd/variable_declaration_statement.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::node& ns::visitor_antecedent::get_antecedent()
{
    return _stack.top().get();
}

void ns::visitor_antecedent::visit_begin(expression_statement& s)
{
    _stack.push(std::ref(s));
}

void ns::visitor_antecedent::visit_end(expression_statement&)
{
    _stack.pop();
}

void ns::visitor_antecedent::visit_begin(operator_assignment& op)
{
    _stack.push(std::ref(op));
}

void ns::visitor_antecedent::visit_end(operator_assignment&)
{
    _stack.pop();
}

void ns::visitor_antecedent::visit_begin(operator_binary& op)
{
    _stack.push(std::ref(op));
}

void ns::visitor_antecedent::visit_end(operator_binary&)
{
    _stack.pop();
}

void ns::visitor_antecedent::visit_begin(operator_unary_postfix& op)
{
    _stack.push(std::ref(op));
}

void ns::visitor_antecedent::visit_end(operator_unary_postfix&)
{
    _stack.pop();
}

void ns::visitor_antecedent::visit_begin(operator_unary_prefix& op)
{
    _stack.push(std::ref(op));
}

void ns::visitor_antecedent::visit_end(operator_unary_prefix&)
{
    _stack.pop();
}

void ns::visitor_antecedent::visit_begin(statement_list& s)
{
    _stack.push(std::ref(s));
}

void ns::visitor_antecedent::visit_end(statement_list&)
{
    _stack.pop();
}

void ns::visitor_antecedent::visit_begin(variable_declaration_statement& s)
{
    _stack.push(std::ref(s));
}

void ns::visitor_antecedent::visit_end(variable_declaration_statement&)
{
    _stack.pop();
}

#include <halberd/visitor_traversal.h>
#include <halberd/visit.h>
#include <halberd/expression.h>
#include <halberd/expression_statement.h>
#include <halberd/operator_assignment.h>
#include <halberd/operator_binary.h>
#include <halberd/operator_unary_postfix.h>
#include <halberd/operator_unary_prefix.h>
#include <halberd/statement.h>
#include <halberd/statement_list.h>
#include <halberd/variable_declaration.h>
#include <halberd/variable_declaration_statement.h>


namespace
{
    namespace ns = halberd::syntax;
}

void ns::visitor_traversal::visit(expression_statement& es)
{
    visit_begin(es);
    ns::visit(*this, es.get_expression());
    visit_end(es);
}

void ns::visitor_traversal::visit(operator_assignment& op)
{
    visit_begin(op);
    ns::visit(*this, op.get_operand_lhs());
    ns::visit(*this, op.get_operand_rhs());
    visit_end(op);
}

void ns::visitor_traversal::visit(operator_binary& op)
{
    visit_begin(op);
    ns::visit(*this, op.get_operand_lhs());
    ns::visit(*this, op.get_operand_rhs());
    visit_end(op);
}

void ns::visitor_traversal::visit(operator_unary_postfix& op)
{
    visit_begin(op);
    ns::visit(*this, op.get_operand());
    visit_end(op);
}

void ns::visitor_traversal::visit(operator_unary_prefix& op)
{
    visit_begin(op);
    ns::visit(*this, op.get_operand());
    visit_end(op);
}

void ns::visitor_traversal::visit(statement_list& list)
{
    visit_begin(list);

    for (auto& list_item : list)
    {
        ns::visit(*this, *list_item);
    }

    visit_end(list);
}

void ns::visitor_traversal::visit(variable_declaration_statement& vds)
{
    visit_begin(vds);
    ns::visit(*this, vds.get_variable_declaration());
    visit_end(vds);
}

void ns::visitor_traversal::visit_begin(expression_statement&) {}
void ns::visitor_traversal::visit_end(expression_statement&) {}

void ns::visitor_traversal::visit_begin(operator_assignment&) {}
void ns::visitor_traversal::visit_end(operator_assignment&) {}

void ns::visitor_traversal::visit_begin(operator_binary&) {}
void ns::visitor_traversal::visit_end(operator_binary&) {}

void ns::visitor_traversal::visit_begin(operator_unary_postfix&) {}
void ns::visitor_traversal::visit_end(operator_unary_postfix&) {}

void ns::visitor_traversal::visit_begin(operator_unary_prefix&) {}
void ns::visitor_traversal::visit_end(operator_unary_prefix&) {}

void ns::visitor_traversal::visit_begin(statement_list&) {}
void ns::visitor_traversal::visit_end(statement_list&) {}

void ns::visitor_traversal::visit_begin(variable_declaration_statement&) {}
void ns::visitor_traversal::visit_end(variable_declaration_statement&) {}

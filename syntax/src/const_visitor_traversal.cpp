#include <halberd/const_visitor_traversal.h>
#include <halberd/const_visit.h>
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

void ns::const_visitor_traversal::visit(const expression_statement& es)
{
    visit_begin(es);
    ns::visit(*this, es.get_expression());
    visit_end(es);
}

void ns::const_visitor_traversal::visit(const operator_assignment& op)
{
    visit_begin(op);
    ns::visit(*this, op.get_operand_lhs());
    ns::visit(*this, op.get_operand_rhs());
    visit_end(op);
}

void ns::const_visitor_traversal::visit(const operator_binary& op)
{
    visit_begin(op);
    ns::visit(*this, op.get_operand_lhs());
    ns::visit(*this, op.get_operand_rhs());
    visit_end(op);
}

void ns::const_visitor_traversal::visit(const operator_unary_postfix& op)
{
    visit_begin(op);
    ns::visit(*this, op.get_operand());
    visit_end(op);
}

void ns::const_visitor_traversal::visit(const operator_unary_prefix& op)
{
    visit_begin(op);
    ns::visit(*this, op.get_operand());
    visit_end(op);
}

void ns::const_visitor_traversal::visit(const statement_list& list)
{
    visit_begin(list);

    for (const auto& list_item : list)
    {
        ns::visit(*this, *list_item);
    }

    visit_end(list);
}

void ns::const_visitor_traversal::visit(const variable_declaration_statement& vds)
{
    visit_begin(vds);
    ns::visit(*this, vds.get_variable_declaration());
    visit_end(vds);
}

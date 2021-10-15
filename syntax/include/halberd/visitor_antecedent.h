#pragma once

#include "visitor_traversal.h"

#include <stack> // std::stack
#include <functional> // std::reference_wrapper


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class node;

    class visitor_antecedent : public visitor_traversal
    {
    public:
        node& get_antecedent();

    protected:
        void visit_begin(expression_statement&) override;
        void visit_end(expression_statement&) override;

        void visit_begin(operator_assignment&) override;
        void visit_end(operator_assignment&) override;

        void visit_begin(operator_binary&) override;
        void visit_end(operator_binary&) override;

        void visit_begin(operator_unary_postfix&) override;
        void visit_end(operator_unary_postfix&) override;

        void visit_begin(operator_unary_prefix&) override;
        void visit_end(operator_unary_prefix&) override;

        void visit_begin(statement_list&) override;
        void visit_end(statement_list&) override;

        void visit_begin(variable_declaration_statement&) override;
        void visit_end(variable_declaration_statement&) override;

    private:
        std::stack<std::reference_wrapper<node>> _stack;
    };
}
}

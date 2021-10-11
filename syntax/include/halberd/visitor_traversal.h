#pragma once

#include "visitor.h"


namespace halberd
{
namespace syntax
{
    class visitor_traversal : public visitor
    {
    public:
        void visit(expression_statement&) override;
        void visit(operator_assignment&) override;
        void visit(operator_binary&) override;
        void visit(operator_unary_postfix&) override;
        void visit(operator_unary_prefix&) override;
        void visit(statement_list&) override;
        void visit(variable_declaration_statement&) override;

    protected:
        virtual void visit_begin(expression_statement&) = 0;
        virtual void visit_end(expression_statement&) = 0;

        virtual void visit_begin(operator_assignment&) = 0;
        virtual void visit_end(operator_assignment&) = 0;

        virtual void visit_begin(operator_binary&) = 0;
        virtual void visit_end(operator_binary&) = 0;

        virtual void visit_begin(operator_unary_postfix&) = 0;
        virtual void visit_end(operator_unary_postfix&) = 0;

        virtual void visit_begin(operator_unary_prefix&) = 0;
        virtual void visit_end(operator_unary_prefix&) = 0;

        virtual void visit_begin(statement_list&) = 0;
        virtual void visit_end(statement_list&) = 0;

        virtual void visit_begin(variable_declaration_statement&) = 0;
        virtual void visit_end(variable_declaration_statement&) = 0;
    };
}
}

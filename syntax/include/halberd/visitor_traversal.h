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

    class const_visitor_traversal : public const_visitor
    {
    public:
        void visit(const expression_statement&) override;
        void visit(const operator_assignment&) override;
        void visit(const operator_binary&) override;
        void visit(const operator_unary_postfix&) override;
        void visit(const operator_unary_prefix&) override;
        void visit(const statement_list&) override;
        void visit(const variable_declaration_statement&) override;

    protected:
        virtual void visit_begin(const expression_statement&) = 0;
        virtual void visit_end(const expression_statement&) = 0;

        virtual void visit_begin(const operator_assignment&) = 0;
        virtual void visit_end(const operator_assignment&) = 0;

        virtual void visit_begin(const operator_binary&) = 0;
        virtual void visit_end(const operator_binary&) = 0;

        virtual void visit_begin(const operator_unary_postfix&) = 0;
        virtual void visit_end(const operator_unary_postfix&) = 0;

        virtual void visit_begin(const operator_unary_prefix&) = 0;
        virtual void visit_end(const operator_unary_prefix&) = 0;

        virtual void visit_begin(const statement_list&) = 0;
        virtual void visit_end(const statement_list&) = 0;

        virtual void visit_begin(const variable_declaration_statement&) = 0;
        virtual void visit_end(const variable_declaration_statement&) = 0;
    };
}
}

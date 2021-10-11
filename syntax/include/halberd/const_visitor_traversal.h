#pragma once

#include "const_visitor.h"


namespace halberd
{
namespace syntax
{
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

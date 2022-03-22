#pragma once


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class expression_statement;
    class identifier_expression;
    class literal;
    class operator_assignment;
    class operator_binary;
    class operator_unary_postfix;
    class operator_unary_prefix;
    class statement_list;
    class variable_declaration_statement;
    class variable_declaration;
    class variable_identifier;

    class visitor
    {
    public:
        virtual ~visitor() = default;

        virtual void visit(expression_statement&) = 0;
        virtual void visit(identifier_expression&) = 0;
        virtual void visit(literal&) = 0;
        virtual void visit(operator_assignment&) = 0;
        virtual void visit(operator_binary&) = 0;
        virtual void visit(operator_unary_postfix&) = 0;
        virtual void visit(operator_unary_prefix&) = 0;
        virtual void visit(statement_list&) = 0;
        virtual void visit(variable_declaration_statement&) = 0;
        virtual void visit(variable_declaration&) = 0;
        virtual void visit(variable_identifier&) = 0;
    };
}
}

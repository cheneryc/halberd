#pragma once

#include "visitor.h"

#include "expression_statement.h"
#include "operator_assignment.h"
#include "operator_binary.h"
#include "operator_unary_postfix.h"
#include "operator_unary_prefix.h"

#include <memory> // std::unique_ptr, std::addressof
#include <functional> // std::reference_wrapper, std::ref
#include <stdexcept> // std::logic_error
#include <type_traits> // std::is_base_of, std::integral_constant, std::true_type, std::false_type
#include <utility> // std::forward


namespace halberd
{
namespace syntax
{
    namespace detail
    {
        template<typename T>
        bool ref_equals(const T& lhs, const T& rhs) noexcept
        {
            return std::addressof(lhs) == std::addressof(rhs);
        }

        template<typename Fn1, typename Fn2>
        constexpr auto static_if(std::true_type, Fn1&& fn_true, Fn2&& fn_false)
        {
            return std::forward<Fn1>(fn_true);
        }

        template<typename Fn1, typename Fn2>
        constexpr auto static_if(std::false_type, Fn1&& fn_true, Fn2&& fn_false)
        {
            return std::forward<Fn2>(fn_false);
        }

        template<bool B, typename Fn1, typename Fn2>
        constexpr auto static_if(Fn1&& fn_true, Fn2&& fn_false)
        {
            return static_if(std::integral_constant<bool, B>(), std::forward<Fn1>(fn_true), std::forward<Fn2>(fn_false));
        }
    }

    template<typename N>
    class visitor_reset : public visitor
    {
        static_assert(std::is_base_of<node, N>::value, "visitor_reset: template parameter N must derive from syntax::node");

    public:
        visitor_reset(N& node_old, std::unique_ptr<N>&& node_new) :
            _node_old(std::ref(node_old)),
            _node_new(std::move(node_new))
            
        {
        }

        void visit(expression_statement& s) override
        {
            auto fn = detail::static_if<std::is_base_of<expression, N>::value>(
                [&s](expression& exp_old, std::unique_ptr<expression>&& exp_new)
            {
                if (!detail::ref_equals(exp_old, s.get_expression()))
                {
                    throw std::logic_error("invalid state - specified node is not an expression of the visited syntax::expression_statement instance");
                }

                s.reset_expression(std::move(exp_new));
            },
                [](N&, std::unique_ptr<N>&&)
            {
                throw std::logic_error("invalid state - template parameter N must derive from syntax::expression");
            });

            fn(_node_old.get(), std::move(_node_new));
        }

        void visit(identifier_expression&) override
        {
            throw std::logic_error("invalid state - syntax::identifier_expression is a terminal type");
        }

        void visit(literal&) override
        {
            throw std::logic_error("invalid state - syntax::literal is a terminal type");
        }

        void visit(operator_assignment& op) override
        {
            auto fn = detail::static_if<std::is_base_of<expression, N>::value>(
                [&op](expression& exp_old, std::unique_ptr<expression>&& exp_new)
            {
                if (detail::ref_equals(exp_old, op.get_operand_lhs()))
                {
                    op.reset_operand_lhs(std::move(exp_new));
                }

                if (detail::ref_equals(exp_old, op.get_operand_rhs()))
                {
                    op.reset_operand_rhs(std::move(exp_new));
                }
            },
                [](N&, std::unique_ptr<N>&&)
            {
                throw std::logic_error("invalid state - template parameter N must derive from syntax::expression");
            });

            fn(_node_old.get(), std::move(_node_new));

            if (_node_new)
            {
                throw std::logic_error("invalid state - specified node is not an operand of the visited syntax::operator_assignment instance");
            }
        }

        void visit(operator_binary& op) override
        {
            auto fn = detail::static_if<std::is_base_of<expression, N>::value>(
                [&op](expression& exp_old, std::unique_ptr<expression>&& exp_new)
            {
                if (detail::ref_equals(exp_old, op.get_operand_lhs()))
                {
                    op.reset_operand_lhs(std::move(exp_new));
                }

                if (detail::ref_equals(exp_old, op.get_operand_rhs()))
                {
                    op.reset_operand_rhs(std::move(exp_new));
                }
            },
                [](N&, std::unique_ptr<N>&&)
            {
                throw std::logic_error("invalid state - template parameter N must derive from syntax::expression");
            });

            fn(_node_old.get(), std::move(_node_new));

            if (_node_new)
            {
                throw std::logic_error("invalid state - specified node is not an operand of the visited syntax::operator_binary instance");
            }
        }

        void visit(operator_unary_postfix& op) override
        {
            auto fn = detail::static_if<std::is_base_of<expression, N>::value>(
                [&op](expression& exp_old, std::unique_ptr<expression>&& exp_new)
            {
                if (!detail::ref_equals(exp_old, op.get_operand()))
                {
                    throw std::logic_error("invalid state - specified node is not an operand of the visited syntax::operator_unary_postfix instance");
                }

                op.reset_operand(std::move(exp_new));
            },
                [](N&, std::unique_ptr<N>&&)
            {
                throw std::logic_error("invalid state - template parameter N must derive from syntax::expression");
            });

            fn(_node_old.get(), std::move(_node_new));
        }

        void visit(operator_unary_prefix& op) override
        {
            auto fn = detail::static_if<std::is_base_of<expression, N>::value>(
                [&op](expression& exp_old, std::unique_ptr<expression>&& exp_new)
            {
                if (!detail::ref_equals(exp_old, op.get_operand()))
                {
                    throw std::logic_error("invalid state - specified node is not an operand of the visited syntax::operator_unary_prefix instance");
                }

                op.reset_operand(std::move(exp_new));
            },
                [](N&, std::unique_ptr<N>&&)
            {
                throw std::logic_error("invalid state - template parameter N must derive from syntax::expression");
            });

            fn(_node_old.get(), std::move(_node_new));
        }

        void visit(statement_list&) override
        {
            throw std::exception(); //TODO: implement
        }

        void visit(variable_declaration_statement&) override
        {
            throw std::exception(); //TODO: implement
        }

        void visit(variable_declaration&) override
        {
            throw std::logic_error("invalid state - syntax::variable_declaration is a terminal type");
        }

        void visit(variable_identifier&) override
        {
            throw std::logic_error("invalid state - syntax::variable_identifier is a terminal type");
        }

    private:
        std::reference_wrapper<N> _node_old;
        std::unique_ptr<N> _node_new;
    };
}
}

#pragma once

#include "visitor_traversal.h"

// halberd::util
#include <halberd/util/overload.h>

#include <utility> // std::forward, std::declval


namespace halberd
{
namespace syntax
{
    namespace detail
    {
        struct priority_low {};
        struct priority_high : priority_low {};

        // The overload called if the callable type can be invoked with the specified arguments
        template<typename Fn, typename... Args>
        auto try_invoke_impl(priority_high, Fn&& fn, Args&&... args) -> decltype(std::declval<Fn>()(std::declval<Args>()...))
        {
            return fn(std::forward<Args>(args)...);
        }

        // The 'do nothing' overload called if the invoke operation is not well formed
        template<typename Fn, typename... Args>
        auto try_invoke_impl(priority_low, Fn&&, Args&&...) -> void
        {
        }

        // Calls the invocable type if the operation is well formed.
        template<typename Fn, typename... Args>
        void try_invoke(Fn&& fn, Args&&... args)
        {
            try_invoke_impl(priority_high(), std::forward<Fn>(fn), std::forward<Args>(args)...);
        }
    }

    constexpr struct visit_begin_tag
    {
    } visit_begin_tag_v;

    constexpr struct visit_end_tag
    {
    } visit_end_tag_v;

    template<typename Fn>
    class visitor_function : public visitor_traversal
    {
    public:
        template<typename A>
        visitor_function(A&& arg) noexcept : _fn(std::forward<A>(arg))
        {
        }

    private:
        void visit(identifier_expression& exp) override
        {
            detail::try_invoke(_fn, exp);
        }

        void visit(literal& l) override
        {
            detail::try_invoke(_fn, l);
        }

        void visit(variable_declaration& dec) override
        {
            detail::try_invoke(_fn, dec);
        }

        void visit_begin(expression_statement& s) override
        {
            detail::try_invoke(_fn, s, visit_begin_tag_v);
        }

        void visit_begin(operator_assignment& op) override
        {
            detail::try_invoke(_fn, op, visit_begin_tag_v);
        }

        void visit_begin(operator_binary& op) override
        {
            detail::try_invoke(_fn, op, visit_begin_tag_v);
        }

        void visit_begin(operator_unary_postfix& op) override
        {
            detail::try_invoke(_fn, op, visit_begin_tag_v);
        }

        void visit_begin(operator_unary_prefix& op) override
        {
            detail::try_invoke(_fn, op, visit_begin_tag_v);
        }

        void visit_begin(statement_list& op) override
        {
            detail::try_invoke(_fn, op, visit_begin_tag_v);
        }

        void visit_begin(variable_declaration_statement& s) override
        {
            detail::try_invoke(_fn, s, visit_begin_tag_v);
        }

        void visit_end(expression_statement& s) override
        {
            detail::try_invoke(_fn, s, visit_end_tag_v);
        }

        void visit_end(operator_assignment& op) override
        {
            detail::try_invoke(_fn, op, visit_end_tag_v);
        }

        void visit_end(operator_binary& op) override
        {
            detail::try_invoke(_fn, op, visit_end_tag_v);
        }

        void visit_end(operator_unary_postfix& op) override
        {
            detail::try_invoke(_fn, op, visit_end_tag_v);
        }

        void visit_end(operator_unary_prefix& op) override
        {
            detail::try_invoke(_fn, op, visit_end_tag_v);
        }

        void visit_end(statement_list& op) override
        {
            detail::try_invoke(_fn, op, visit_end_tag_v);
        }

        void visit_end(variable_declaration_statement& op) override
        {
            detail::try_invoke(_fn, op, visit_end_tag_v);
        }

        Fn _fn;
    };

    template<typename... Fs>
    auto make_visitor_function(Fs&&... funcs)
    {
        auto fn = util::overload(std::forward<Fs>(funcs)...);

        return visitor_function<decltype(fn)> {
            std::move(fn)
        };
    }
}
}

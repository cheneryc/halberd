#pragma once

// halberd::syntax
#include <halberd/declaration.h>
#include <halberd/statement.h>

#include <memory> // std::unique_ptr
#include <tuple> // std::tuple
#include <type_traits> // std::is_base_of


namespace halberd
{
namespace compiler
{
namespace transform
{
    template<typename D, typename S>
    struct transform_declaration_statement
    {
        static_assert(std::is_base_of<syntax::declaration, D>::value, "transform_declaration_statement: template parameter D must derive from syntax::declaration");
        static_assert(std::is_base_of<syntax::statement, S>::value, "transform_declaration_statement: template parameter S must derive from syntax::statement");

        std::unique_ptr<syntax::statement> operator()(std::tuple<std::unique_ptr<D>> value) const
        {
            return std::make_unique<S>(std::get<0U>(std::move(value)));
        }
    };
}
}
}

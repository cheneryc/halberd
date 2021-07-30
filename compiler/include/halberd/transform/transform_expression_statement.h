#pragma once

// halberd::syntax
#include <halberd/expression.h>
#include <halberd/statement.h>

#include <memory> // std::unique_ptr
#include <tuple> // std::tuple


namespace halberd
{
namespace compiler
{
namespace transform
{
    struct transform_expression_statement
    {
        std::unique_ptr<syntax::statement> operator()(std::tuple<std::unique_ptr<syntax::expression>> value) const;
    };
}
}
}

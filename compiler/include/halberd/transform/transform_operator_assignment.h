#pragma once

// halberd::lexer
#include <halberd/symbol.h>

// halberd::syntax
#include <halberd/expression.h>

#include <memory> // std::unique_ptr
#include <tuple> // std::tuple


namespace halberd
{
namespace compiler
{
namespace transform
{
    struct transform_operator_assignment
    {
        std::unique_ptr<syntax::expression> operator()(std::tuple<std::unique_ptr<syntax::expression>, lexer::symbol, std::unique_ptr<syntax::expression>> value) const;
    };
}
}
}

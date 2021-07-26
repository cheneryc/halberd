#pragma once

// halberd::syntax
#include <halberd/literal.h>

#include <tuple> // std::tuple
#include <memory> // std::unique_ptr


namespace halberd
{
namespace compiler
{
namespace transform
{
    struct transform_literal
    {
        std::unique_ptr<syntax::literal> operator()(std::tuple<int> value) const;
        std::unique_ptr<syntax::literal> operator()(std::tuple<float> value) const;
    };
}
}
}

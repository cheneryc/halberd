#pragma once

// halberd::syntax
#include <halberd/statement.h>
#include <halberd/statement_list.h>

#include <memory> // std::unique_ptr
#include <tuple> // std::tuple
#include <vector> // std::vector


namespace halberd
{
namespace compiler
{
namespace transform
{
    struct transform_statement_list
    {
        std::unique_ptr<syntax::statement_list> operator()(std::tuple<std::vector<std::unique_ptr<syntax::statement>>> value) const;
    };
}
}
}

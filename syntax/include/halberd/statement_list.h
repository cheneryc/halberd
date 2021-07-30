#pragma once

#include "list.h"
#include "statement.h"

#include <memory> // std::unique_ptr
#include <vector> // std::vector


namespace halberd
{
namespace syntax
{
    class statement_list : public list<statement>
    {
    public:
        statement_list(std::vector<std::unique_ptr<statement>> statements) noexcept;
    };
}
}

#pragma once

#include "list.h"
#include "statement.h"

#include <memory> // std::unique_ptr
#include <vector> // std::vector


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class visitor;
    class const_visitor;

    class statement_list : public list<statement>
    {
    public:
        statement_list(std::vector<std::unique_ptr<statement>> statements) noexcept;

        void accept(visitor& v) override;
        void accept(const_visitor& cv) const override;
    };
}
}

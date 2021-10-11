#pragma once

#include "expression.h"

#include <string> // std::string


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class visitor;
    class const_visitor;

    class identifier_expression : public expression
    {
    public:
        identifier_expression(std::string id) noexcept;

        void accept(visitor& v) override;
        void accept(const_visitor& cv) const override;

    private:
        std::string _id;
    };
}
}

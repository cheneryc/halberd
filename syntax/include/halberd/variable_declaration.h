#pragma once

#include "declaration.h"
#include "type.h"

#include <string> // std::string


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class visitor;
    class const_visitor;

    class variable_declaration : public declaration
    {
    public:
        variable_declaration(type t, std::string name) noexcept;

        void accept(visitor& v) override;
        void accept(const_visitor& cv) const override;

        type get_type() const;

    private:
        type _type;
    };
}
}

#pragma once

#include "node.h"

#include <string> // std::string


namespace halberd
{
namespace syntax
{
    class declaration : public node
    {
    public:
        const char* get_name() const noexcept;

    protected:
        declaration(std::string name) noexcept;

    private:
        std::string _name;
    };
}
}

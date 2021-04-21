#pragma once

#include "tree.h"

#include <memory> // std::unique_ptr


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class node;

    class tree_test final : public tree
    {
    public:
        tree_test(std::unique_ptr<node> root_node) : _root_node(std::move(root_node))
        {
        }

    private:
        std::unique_ptr<node> _root_node;
    };
}
}

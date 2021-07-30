#pragma once

#include "node.h"

#include <memory> // std::unique_ptr
#include <vector> // std::vector
#include <type_traits> // std::is_base_of


namespace halberd
{
namespace syntax
{
    template<typename T>
    class list : public node
    {
        static_assert(std::is_base_of<node, T>::value, "list: template parameter T must derive from syntax::node");

    protected:
        list(std::vector<std::unique_ptr<T>> list_items) noexcept : _list_items(std::move(list_items))
        {
        }

    private:
        std::vector<std::unique_ptr<T>> _list_items;
    };
}
}

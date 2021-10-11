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

    public:
        using iterator = typename std::vector<std::unique_ptr<T>>::iterator;

        iterator begin()
        {
            return _list_items.begin();
        }

        iterator end()
        {
            return _list_items.end();
        }

        using const_iterator = typename std::vector<std::unique_ptr<T>>::const_iterator;

        const_iterator begin() const
        {
            return _list_items.begin();
        }

        const_iterator end() const
        {
            return _list_items.end();
        }

    protected:
        list(std::vector<std::unique_ptr<T>> list_items) noexcept : _list_items(std::move(list_items))
        {
        }

    private:
        std::vector<std::unique_ptr<T>> _list_items;
    };
}
}

#pragma once


namespace halberd
{
namespace syntax
{
    class node
    {
    public:
        // Ensure all derived types are non-copyable
        node(const node&) = delete;
        node& operator=(const node&) = delete;

        virtual ~node() = 0;

    protected:
        node() = default;
    };
}
}

#pragma once


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class visitor;
    class const_visitor;

    class node
    {
    public:
        // Ensure all derived types are non-copyable
        node(const node&) = delete;
        node& operator=(const node&) = delete;

        virtual ~node() = 0;

        virtual void accept(visitor& v) = 0;
        virtual void accept(const_visitor& cv) const = 0;

    protected:
        node() = default;
    };
}
}

#pragma once


namespace halberd
{
namespace syntax
{
    class tree
    {
    public:
        // Ensure all derived types are non-copyable
        tree(const tree&) = delete;
        tree& operator=(const tree&) = delete;

        virtual ~tree() = 0;

    protected:
        tree() = default;
    };
}
}

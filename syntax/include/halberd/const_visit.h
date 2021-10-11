#pragma once


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class node;
    class const_visitor;

    void visit(const_visitor& cv, const node& n);
}
}

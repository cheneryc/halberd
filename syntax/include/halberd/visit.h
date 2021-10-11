#pragma once


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class node;
    class visitor;
    class const_visitor;

    void visit(visitor& v, node& n);
    void visit(const_visitor& cv, const node& n);
}
}

#pragma once


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class node;
    class visitor;

    void visit(visitor& v, node& n);
}
}

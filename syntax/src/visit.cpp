#include <halberd/visit.h>
#include <halberd/visitor.h>
#include <halberd/node.h>


namespace
{
    namespace ns = halberd::syntax;
}

void ns::visit(visitor& v, node& n)
{
    n.accept(v);
}

#include <halberd/const_visit.h>
#include <halberd/const_visitor.h>
#include <halberd/node.h>


namespace
{
    namespace ns = halberd::syntax;
}

void ns::visit(const_visitor& cv, const node& n)
{
    n.accept(cv);
}

#include <halberd/statement_list.h>
#include <halberd/visitor.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::statement_list::statement_list(std::vector<std::unique_ptr<statement>> statements) noexcept : list<statement>(std::move(statements))
{
}

void ns::statement_list::accept(visitor& v)
{
    v.visit(*this);
}

void ns::statement_list::accept(const_visitor& cv) const
{
    cv.visit(*this);
}

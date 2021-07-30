#include <halberd/statement_list.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::statement_list::statement_list(std::vector<std::unique_ptr<statement>> statements) noexcept : list<statement>(std::move(statements))
{
}

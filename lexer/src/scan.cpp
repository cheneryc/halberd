#include <halberd/scan.h>

#include <halberd/scanner.h>


namespace
{
    namespace ns = halberd::lexer;
}

std::vector<std::unique_ptr<ns::token>> ns::scan(ns::state_machine_runner<char, ns::lexer_tag> smr, const char* src)
{
    std::vector<std::unique_ptr<token>> tokens;

    scanner s(smr, src);

    while (auto token = s.scan())
    {
        tokens.push_back(std::move(token));
    }

    return tokens;
}

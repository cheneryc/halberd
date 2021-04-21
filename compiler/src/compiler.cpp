#include <halberd/compiler.h>

// halberd::lexer
#include <halberd/lexer.h>
#include <halberd/scanner.h>

// halberd::parser
#include <halberd/source.h>

#include <iterator> // std::make_move_iterator


namespace
{
    namespace ns = halberd::compiler;
}

bool ns::compile(const char* src)
{
    return compile_rule(rule::variable_declaration, src);
}

bool ns::compile(std::vector<std::unique_ptr<lexer::token>> tokens)
{
    return compile_rule(rule::variable_declaration, std::move(tokens));
}

bool ns::compile_rule(rule r, const char* src)
{
    lexer::scanner scanner(lexer::get_smv_union(), src);

    auto token_source = parser::make_source(
        [&scanner]()
    {
        return scanner.scan();
    },
        [](const std::unique_ptr<lexer::token>& token_ptr)
    {
        return token_ptr.get();
    });

    using token_t = std::unique_ptr<lexer::token>;
    using token_ptr_t = lexer::token*;

    return make_rule_parser<token_t, token_ptr_t>(r).apply(token_source).operator bool();
}

bool ns::compile_rule(rule r, std::vector<std::unique_ptr<lexer::token>> tokens)
{
    auto token_source = parser::make_source(
        [it     = std::make_move_iterator(tokens.begin()),
         it_end = std::make_move_iterator(tokens.end())]
        () mutable
    {
        return (it != it_end) ? *(it++) : nullptr;
    },
        [](const std::unique_ptr<lexer::token>& token_ptr)
    {
        return token_ptr.get();
    });

    using token_t = std::unique_ptr<lexer::token>;
    using token_ptr_t = lexer::token*;

    return make_rule_parser<token_t, token_ptr_t>(r).apply(token_source).operator bool();
}

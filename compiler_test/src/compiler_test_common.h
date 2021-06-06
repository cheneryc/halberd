#pragma once

#include <halberd/compiler.h>

#include <iterator> // std::begin, std::end, std::make_move_iterator
#include <string> // std::string
#include <utility> // std::forward


namespace test
{
    namespace ns = halberd::compiler;

    inline std::unique_ptr<halberd::lexer::token_identifier_reserved> make_keyword(halberd::lexer::keyword keyword)
    {
        return std::make_unique<halberd::lexer::token_identifier_reserved>(keyword);
    }

    inline std::unique_ptr<halberd::lexer::token_symbol> make_symbol(halberd::lexer::symbol symbol)
    {
        return std::make_unique<halberd::lexer::token_symbol>(symbol);
    }

    inline std::unique_ptr<halberd::lexer::token_identifier> make_identifier(std::string identifier)
    {
        return std::make_unique<halberd::lexer::token_identifier>(std::move(identifier));
    }

    template<typename... Ts>
    std::vector<std::unique_ptr<halberd::lexer::token>> make_tokens(Ts&&... t)
    {
        // Cannot use move semantics with std::initializer_list so use an array as an intermediary instead
        std::unique_ptr<halberd::lexer::token> tokens_init[] = {
            std::forward<Ts>(t)...
        };

        std::vector<std::unique_ptr<halberd::lexer::token>> tokens = {
            std::make_move_iterator(std::begin(tokens_init)),
            std::make_move_iterator(std::end(tokens_init)),
        };

        return tokens;
    }
}

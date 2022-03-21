#pragma once

#include "rule.h"

// halberd::lexer
#include <halberd/token.h>

// halberd::parser
#include <halberd/parse_result.h>

// halberd::syntax
#include <halberd/node.h>

#include <memory> // std::unique_ptr
#include <vector> // std::vector


namespace halberd
{
namespace compiler
{
    //TODO: return type should be parse_result<std::unique_ptr<syntax::tree>>
    bool compile(const char* src);
    bool compile(std::vector<std::unique_ptr<lexer::token>> tokens);

    // Performs lexical analysis by converting the program source into a sequence of tokens
    //std::vector<std::unique_ptr<lexer::token>> tokenize(const char* src);

    // Performs syntactic analysis by parsing the sequence of tokens according to the specified grammar rule (having tokenized the program source if necessary)
    parser::parse_result<std::unique_ptr<syntax::node>> parse(rule r, const char* src);
    parser::parse_result<std::unique_ptr<syntax::node>> parse(rule r, std::vector<std::unique_ptr<lexer::token>> tokens);

    // Perform various forms of semantic analysis required to construct a valid abstract syntax tree
    void check_identifiers(syntax::node& node);
    void check_types(syntax::node& node);
}
}

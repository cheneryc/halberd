#pragma once

#include "combinator.h"
#include "combinator_choice.h"
#include "combinator_end.h"
#include "combinator_filter.h"
#include "combinator_one.h"
#include "combinator_select.h"
#include "combinator_sequence.h"
#include "combinator_transform.h"


namespace halberd
{
namespace parser
{
    /*
        Language grammar in BNF:

        <type_integer> ::= [reserved_identifier: "i8"] |
                [reserved_identifier: "i16"] |
                [reserved_identifier: "i32"] |
                [reserved_identifier: "i64"]

        <type_float> ::= [reserved_identifier: "f16"] |
                [reserved_identifier: "f32"] |
                [reserved_identifier: "f64"]

        // Note: the inclusion of [identifier] here, validation of user defined type names is deferred to semantic analysis stage
        <type_identifier> ::= <type_integer> | <type_float> | [identifier]

        <variable_type> ::= [reserved_identifier: "var"] | [reserved_identifier: "const"]

        <variable_declaration> ::= <variable_type> <type_identifier> [identifier] "=" <variable_initializer> ";"

        <variable_initializer> ::= <initializer_list> |
                    <expression> |
                    [reserved_identifier: "default"]

        <initializer_list> ::= "{" <expression_list> "}"

        <expression_list> ::= epsilon | <expression_list_not_empty>
        <expression_list_not_empty> ::= <expression> | <expression> "," <expression_list_not_empty>

        <expression_primary> ::= [identifier] |
                    [literal_integer] |
                    [literal_fractional] |
                    "(" <expression> ")"

        <expression_unary> ::= <operator_unary> <expression>
    */

    void parse();
}
}

#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "tokens.h"
#include "fn_registry.h"

class Lexer {
public:
    std::string expression;
    std::vector<Tokens::Token> tokens;

    Lexer() = default;
    explicit Lexer(std::string expression);

    void lex();

private:
    std::size_t _cursor = 0;

    void _get_char(char ch);
    void _get_number(char ch);
    void _get_alpha(char ch);
};


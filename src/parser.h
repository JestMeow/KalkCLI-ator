#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "tokens.h"
#include "tree.h"

class Parser {
public:
    bool allow_implicit_mul = true;
    std::vector<Tokens::Token> tokens;

    Parser() = default;
    explicit Parser(std::vector<Tokens::Token> tokens);

    std::unique_ptr<Tree::Node> parse();

    std::unique_ptr<Tree::Node> parse_expr();
    std::unique_ptr<Tree::Node> parse_expr_tail(std::unique_ptr<Tree::Node> left);
    std::unique_ptr<Tree::Node> parse_term();
    std::unique_ptr<Tree::Node> parse_term_tail(std::unique_ptr<Tree::Node> left);
    std::unique_ptr<Tree::Node> parse_power();
    std::unique_ptr<Tree::Node> parse_factor();
    std::unique_ptr<Tree::Node> parse_function_call();

    void o_parse_expr();
    void o_parse_expr_prime();

private:
    std::size_t _cursor = 0;

    const Tokens::Token& peek();
    Tokens::Token consume();
    Tokens::Token match(Tokens::Kind expected_token_kind);
    void printify();
};


#include "parser.h"

#include <iostream>
#include <stdexcept>
#include <utility>

Parser::Parser(std::vector<Tokens::Token> tokens)
    : tokens(std::move(tokens)) {}


const Tokens::Token& Parser::peek() {
    if (_cursor >= tokens.size()) {
        static const Tokens::Token eof_token{"", Tokens::Kind::EndOfFile};
        return eof_token;
    }
    return tokens[_cursor];
}

Tokens::Token Parser::consume() {
    if (_cursor >= tokens.size()) {
        return peek(); 
    }
    return tokens[_cursor++];
}

Tokens::Token Parser::match(Tokens::Kind expected_token_kind) {
    if (peek().kind == expected_token_kind) {
        return consume();
    }
    throw std::runtime_error("Unexpected Token: " + peek().value);
}

void Parser::printify() {
    std::cout << peek().value << '\n';
}

// Tests
void Parser::o_parse_expr() {
    match(Tokens::Kind::Number);
    std::cout << "Wow numer\n";
    o_parse_expr_prime();
}

void Parser::o_parse_expr_prime() {
    if (peek().kind == Tokens::Kind::AddOp) {
        match(Tokens::Kind::AddOp);
        match(Tokens::Kind::Number);
        std::cout << "parsed addition\n";
        o_parse_expr_prime();
    }
    else return;
}

// Not tests
std::unique_ptr<Tree::Node> Parser::parse_factor() {
    if (peek().kind == Tokens::Kind::Number ||
            peek().kind == Tokens::Kind::PIVal ||
            peek().kind == Tokens::Kind::eVal ||
            peek().kind == Tokens::Kind::radUnit ||
            peek().kind == Tokens::Kind::degUnit) {

        auto number_token = match(peek().kind);
        return std::make_unique<Tree::NumberNode>(number_token.value);
    }
    else if (peek().kind == Tokens::Kind::LeftParen) {
        match(Tokens::Kind::LeftParen);
        auto node = parse_expr();
        match(Tokens::Kind::RightParen);
        return node; 
    }
    else if (peek().kind == Tokens::Kind::Fn || peek().kind == Tokens::Kind::Fn2) {
        return parse_function_call();
    }

    throw std::runtime_error("Syntax Error: Expected number, '(', or function");
}

std::unique_ptr<Tree::Node> Parser::parse_function_call() {
    if (peek().kind == Tokens::Kind::Fn) {
        auto fn_token = match(Tokens::Kind::Fn);
        std::vector<std::unique_ptr<Tree::Node>> args;

        args.push_back(parse_expr());

        return std::make_unique<Tree::FunctionCallNode>(fn_token, std::move(args));
    }
    else if (peek().kind == Tokens::Kind::Fn2) {
        auto fn_token = match(Tokens::Kind::Fn2);
        std::vector<std::unique_ptr<Tree::Node>> args;

        bool prev_state = allow_implicit_mul;
        allow_implicit_mul = false;

        args.push_back(parse_power());

        allow_implicit_mul = prev_state;

        args.push_back(parse_power());

        return std::make_unique<Tree::FunctionCallNode>(fn_token, std::move(args));
    }

    throw std::runtime_error("Syntax Error: Expected function name");
}

std::unique_ptr<Tree::Node> Parser::parse_power() {
    auto left = parse_factor();

    if (peek().kind == Tokens::Kind::PowOp) {
        auto op = match(Tokens::Kind::PowOp);
        auto right = parse_power();

        return std::make_unique<Tree::BinaryNode>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Tree::Node> Parser::parse_term_tail(std::unique_ptr<Tree::Node> left) {
    if (peek().kind == Tokens::Kind::MulOp || peek().kind == Tokens::Kind::DivOp) {
        auto op = match(peek().kind);
        auto right = parse_power();

        auto node = std::make_unique<Tree::BinaryNode>(op, std::move(left), std::move(right));

        return parse_term_tail(std::move(node));
    }
    else if (allow_implicit_mul &&
            peek().kind == Tokens::Kind::Number || 
            peek().kind == Tokens::Kind::PIVal ||
            peek().kind == Tokens::Kind::eVal ||
            peek().kind == Tokens::Kind::degUnit ||
            peek().kind == Tokens::Kind::radUnit ||
            peek().kind == Tokens::Kind::LeftParen || 
            peek().kind == Tokens::Kind::Fn || 
            peek().kind == Tokens::Kind::Fn2) {

        Tokens::Token implicit_mul{"*", Tokens::Kind::MulOp};
        auto right = parse_power();

        auto node = std::make_unique<Tree::BinaryNode>(implicit_mul, std::move(left), std::move(right));

        return parse_term_tail(std::move(node));
    }

    return left; 
}

std::unique_ptr<Tree::Node> Parser::parse_term() {
    auto left = parse_power();
    return parse_term_tail(std::move(left));
}

std::unique_ptr<Tree::Node> Parser::parse_expr_tail(std::unique_ptr<Tree::Node> left) {
    if (peek().kind == Tokens::Kind::AddOp || peek().kind == Tokens::Kind::SubOp) {
        auto op = match(peek().kind);
        auto right = parse_term();

        auto node = std::make_unique<Tree::BinaryNode>(op, std::move(left), std::move(right));

        return parse_expr_tail(std::move(node));
    }

    return left;
}

std::unique_ptr<Tree::Node> Parser::parse_expr() {
    auto left = parse_term();
    return parse_expr_tail(std::move(left));
}

std::unique_ptr<Tree::Node> Parser::parse() {
    auto ast_root = parse_expr();

    if (peek().kind == Tokens::Kind::EndOfFile) {
        return ast_root;
    } else {
        throw std::runtime_error("Syntax Error: Unexpected tokens after expression");
    }
}


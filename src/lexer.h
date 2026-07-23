#pragma once

#include <cctype>
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "tokens.h"
#include "fn_registry.h"



class Lexer {
private:
    size_t _cursor = 0;


    void _get_char(char ch) {
        switch (ch) {
            case '+': tokens.emplace_back(std::string(1, ch), Tokens::Kind::AddOp); break;
            case '-': tokens.emplace_back(std::string(1, ch), Tokens::Kind::SubOp); break;
            case '*': tokens.emplace_back(std::string(1, ch), Tokens::Kind::MulOp); break;
            case '/': tokens.emplace_back(std::string(1, ch), Tokens::Kind::DivOp); break;
            case '^': tokens.emplace_back(std::string(1, ch), Tokens::Kind::PowOp); break;
            case '(': tokens.emplace_back(std::string(1, ch), Tokens::Kind::LeftParen); break;
            case ')': tokens.emplace_back(std::string(1, ch), Tokens::Kind::RightParen); break;
            default: break;
        }
    }


    void _get_number(char ch) {
        if (std::isdigit(ch) || ch == '.') {
            bool has_dot = false;
            std::string buf;

            if (ch == '.') buf.push_back('0');

            while (std::isdigit(ch) || ch == '.') {
                if (ch == '.') {
                    if (has_dot) break;
                    has_dot = true;
                }

                buf.push_back(ch);
                _cursor++;
                ch = expression[_cursor];
            }

            tokens.emplace_back(buf, Tokens::Kind::Number);
            _cursor--;
        }
    }


    void _get_alpha(char ch) {
        if (std::isalpha(ch)) {
            std::string buf;

            while (_cursor < expression.length() && std::isalpha(expression[_cursor])) {
                buf.push_back(expression[_cursor]);
                _cursor++;
            }

            auto it = FUNCTION_REGISTRY.find(buf);
            if (it != FUNCTION_REGISTRY.end()) {
                Tokens::Kind kind = (it->second.arity == 1) ? Tokens::Kind::Fn : Tokens::Kind::Fn2;
                tokens.emplace_back(buf, kind);
            }
            else if (buf == "deg") tokens.emplace_back(buf, Tokens::Kind::degUnit);
            else if (buf == "rad") tokens.emplace_back(buf, Tokens::Kind::radUnit);
            else if (buf == "PI" || buf == "pi") tokens.emplace_back(buf, Tokens::Kind::PIVal);
            else if (buf == "e"  || buf == "E")  tokens.emplace_back(buf, Tokens::Kind::eVal);

            _cursor--;
        }
    }


public:
    std::string expression;
    std::vector<Tokens::Token> tokens;

    Lexer() = default;

    Lexer(std::string expression): expression(expression) {}

    void lex() {
        char ch;
        std::string buf;
        Tokens::Kind token_type;

        while (_cursor <= expression.size()) {
            buf.clear();
            ch = expression[_cursor];
 
            _get_number(ch);
            _get_char(ch);
            _get_alpha(ch);

            _cursor++;
        }

        tokens.emplace_back("", Tokens::Kind::EndOfFile);
    }
};

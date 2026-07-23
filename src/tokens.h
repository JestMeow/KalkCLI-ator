#pragma once

#include <string>

class Tokens {
public:
    enum Kind {
        None = 0,
        EndOfFile,

        Number,

        AddOp,
        SubOp,
        MulOp,
        DivOp,
        ModOp,
        PowOp,

        Fn,
        Fn2,
        
        degUnit,
        radUnit,

        PIVal,
        eVal,

        LeftParen,
        RightParen
    } kind;

    struct Token {
        std::string value;
        Kind kind;
        
        Token() = default;

        Token(std::string buf, Kind token_kind): value(buf), kind(token_kind) {}
    };
};

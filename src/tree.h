#pragma once

#include <memory>
#include <vector>
#include <string>

#include "tokens.h"
#include "fn_registry.h"

class Tree {
public:
    struct Node {
        virtual ~Node() = default;
        virtual void print(int depth = 0) const = 0; 
        virtual double eval() const = 0;
    };

    struct NumberNode : Node {
        std::string value;
        
        explicit NumberNode(std::string value);
        void print(int depth = 0) const override;
        double eval() const override;
    };

    struct BinaryNode : Node {
        Tokens::Token op;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        BinaryNode(Tokens::Token op, std::unique_ptr<Node> left, std::unique_ptr<Node> right);
        void print(int depth = 0) const override;
        double eval() const override;
    };

    struct FunctionCallNode : Node {
        Tokens::Token fn_token;
        std::vector<std::unique_ptr<Node>> args;

        FunctionCallNode(Tokens::Token fn_token, std::vector<std::unique_ptr<Node>> args);
        void print(int depth = 0) const override;
        double eval() const override;
    };

private:
    static double snap_to_zero(double val, double threshold = 1e-15);
};


#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <functional>
#include <unordered_map>

#include "tokens.h"
#include "fn_registry.h"

class Tree {

static double snap_to_zero(double val, double threshold = 1e-15) {
    return (std::abs(val) < threshold) ? 0.0 : val;
}
public:
    struct Node {
        virtual ~Node() = default;
        virtual void print(int depth = 0) const = 0; 
        virtual double eval() const = 0;
    };

    struct NumberNode : Node {
        std::string value;
        NumberNode(std::string value) : value(value) {}

        void print(int depth = 0) const override {
            std::string indent(depth * 2, ' ');
            std::cout << indent << "NumberNode(" << value << ")\n";
        }

        double eval() const override {
            if (value == "PI" || value == "pi") {
                return M_PI;
            }
            else if (value == "e") {
                return M_E;
            }
            else if (value == "deg") {
                return M_PI / 180.0;
            }
            else if (value == "rad") {
                return 1.0;
            }

            try {
                return std::stod(value);
            }
            catch (...) {
                throw std::runtime_error("Unknown variable or number: " + value);
            }
        }
    };

    struct BinaryNode : Node {
        Tokens::Token op;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        BinaryNode(Tokens::Token op, std::unique_ptr<Node> left, std::unique_ptr<Node> right) 
            : op(op), left(std::move(left)), right(std::move(right)) {}

        void print(int depth = 0) const override {
            std::string indent(depth * 2, ' ');
            std::cout << indent << "BinaryNode(" << op.value << ")\n";
            if (left)  left->print(depth + 1);
            if (right) right->print(depth + 1);
        }

        double eval() const override {
            double l = left->eval();
            double r = right->eval();

            if (op.value == "+") return l + r;
            if (op.value == "-") return l - r;
            if (op.value == "*") return l * r;
            if (op.value == "/") {
                if (r == 0.0) throw std::runtime_error("Math Error: Division by zero");
                return l / r;
            }
            if (op.value == "^") return std::pow(l, r);

            throw std::runtime_error("Unknown operator: " + op.value);
        }
    };

    struct FunctionCallNode : Node {
        Tokens::Token fn_token;
        std::vector<std::unique_ptr<Node>> args;

        FunctionCallNode(Tokens::Token fn_token, std::vector<std::unique_ptr<Node>> args)
            : fn_token(fn_token), args(std::move(args)) {}

        void print(int depth = 0) const override {
            std::string indent(depth * 2, ' ');
            std::cout << indent << "FunctionCallNode(" << fn_token.value << ")\n";
            for (const auto& arg : args) {
                if (arg) arg->print(depth + 1);
            }
        }

        double eval() const override {
            auto it = FUNCTION_REGISTRY.find(fn_token.value);
            if (it == FUNCTION_REGISTRY.end()) {
                throw std::runtime_error("Unknown function: " + fn_token.value);
            }

            const auto& info = it->second;

            if (args.size() != info.arity) {
                throw std::runtime_error("'" + fn_token.value + "' expects " + 
                        std::to_string(info.arity) + " argument(s), but got " + 
                        std::to_string(args.size()));
            }

            std::vector<double> evaluated_args;
            evaluated_args.reserve(args.size());
            for (const auto& arg : args) {
                evaluated_args.push_back(arg->eval());
            }

            return info.fn(evaluated_args);
        }
    };
};

#include "tree.h"

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <functional>
#include <unordered_map>

// Helper for trig functions outputting insanely small numbers
double Tree::snap_to_zero(double val, double threshold) {
    return (std::abs(val) < threshold) ? 0.0 : val;
}

// NumberNode
Tree::NumberNode::NumberNode(std::string value) 
    : value(std::move(value)) {}

void Tree::NumberNode::print(int depth) const {
    std::string indent(depth * 2, ' ');
    std::cout << indent << "NumberNode(" << value << ")\n";
}

double Tree::NumberNode::eval() const {
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

Tree::BinaryNode::BinaryNode(Tokens::Token op, std::unique_ptr<Node> left, std::unique_ptr<Node> right) 
    : op(std::move(op)), left(std::move(left)), right(std::move(right)) {}

    void Tree::BinaryNode::print(int depth) const {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "BinaryNode(" << op.value << ")\n";
        if (left)  left->print(depth + 1);
        if (right) right->print(depth + 1);
    }

double Tree::BinaryNode::eval() const {
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

Tree::FunctionCallNode::FunctionCallNode(Tokens::Token fn_token, std::vector<std::unique_ptr<Node>> args)
    : fn_token(std::move(fn_token)), args(std::move(args)) {}

    void Tree::FunctionCallNode::print(int depth) const {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "FunctionCallNode(" << fn_token.value << ")\n";
        for (const auto& arg : args) {
            if (arg) arg->print(depth + 1);
        }
    }

double Tree::FunctionCallNode::eval() const {
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


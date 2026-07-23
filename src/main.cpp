#include <cstddef>
#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "fn_registry.h"



int main(int argc, char* argv[]) {
    std::string arg1 = argv[1];

    if (arg1 == "list") {
        for (const auto& entry : FUNCTION_REGISTRY) {
            std::cout << entry.first << '\n';
        }
    }
    else if (arg1 == "debug") {
        Lexer lexer(argv[2]);
        lexer.lex();

        for (const auto& token : lexer.tokens) {
            if (token.kind != 0) std::cout << token.value << '\n';
            else std::cout << "NONE\n";
        }
        Parser parser(lexer.tokens);
        auto root = parser.parse();

        try {
            if (root) {
                std::cout << "\n--- Abstract Syntax Tree ---\n";
                root->print();

                std::cout << "\n--- Result ---\n";
                double result = root->eval();
                std::cout << " = " << result << "\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
            return 1;
        }
    }
    else {
        std::string input;

        for (size_t i = 1; i < argc; ++i) {
            input += argv[i];
            input += ' ';
        }

        Lexer lexer(input);
        lexer.lex();

        Parser parser(lexer.tokens);
        auto root = parser.parse();

        try {
            if (root) {
                double result = root->eval();
                std::cout << " = " << result << "\n";
            }
        }

        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
            return 1;
        }
    }

    return 0;
}

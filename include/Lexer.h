#pragma once
#include "AST.h"
#include <string>
#include <vector>

class Lexer {
private:
    std::string input;
    size_t pos;
    char current();
    void advance();

public:
    Lexer(const std::string& text);
    std::vector<Token> tokenize();
};
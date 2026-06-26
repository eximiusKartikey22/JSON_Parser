#include "../include/Lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& text) : input(text), pos(0) {}

char Lexer::current() { return pos < input.length() ? input[pos] : '\0'; }
void Lexer::advance() { pos++; }

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (current() != '\0') {
        if (isspace(current())) { advance(); continue; }
        if (current() == '{') { tokens.push_back({TokenType::LBRACE, "{"}); advance(); }
        else if (current() == '}') { tokens.push_back({TokenType::RBRACE, "}"}); advance(); }
        else if (current() == '[') { tokens.push_back({TokenType::LBRACKET, "["}); advance(); }
        else if (current() == ']') { tokens.push_back({TokenType::RBRACKET, "]"}); advance(); }
        else if (current() == ':') { tokens.push_back({TokenType::COLON, ":"}); advance(); }
        else if (current() == ',') { tokens.push_back({TokenType::COMMA, ","}); advance(); }
        else if (current() == '"') {
            advance(); 
            std::string str;
            while (current() != '"' && current() != '\0') { str += current(); advance(); }
            advance(); 
            tokens.push_back({TokenType::STRING, str});
        }
        else if (isdigit(current()) || current() == '-') {
            std::string num;
            while (isdigit(current()) || current() == '.' || current() == '-') { num += current(); advance(); }
            tokens.push_back({TokenType::NUMBER, num});
        }
        else if (isalpha(current())) {
            std::string keyword;
            while (isalpha(current())) { keyword += current(); advance(); }
            if (keyword == "true" || keyword == "false") tokens.push_back({TokenType::BOOLEAN, keyword});
            else if (keyword == "null") tokens.push_back({TokenType::NULL_TYPE, "null"});
            else throw std::runtime_error("Invalid keyword: " + keyword);
        }
        else { throw std::runtime_error(std::string("Unknown char: ") + current()); }
    }
    tokens.push_back({TokenType::END_OF_FILE, ""});
    return tokens;
}
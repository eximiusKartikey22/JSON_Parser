#pragma once
#include <string>
#include <vector>
#include <map>

enum class TokenType { LBRACE, RBRACE, LBRACKET, RBRACKET, STRING, NUMBER, BOOLEAN, NULL_TYPE, COLON, COMMA, END_OF_FILE };

struct Token {
    TokenType type;
    std::string value;
};

enum class JsonType { Null, Bool, Number, String, Array, Object };

class JsonNode {
public:
    JsonType type;
    std::string string_val;
    double number_val;
    bool bool_val;
    std::vector<JsonNode> array_val;
    std::map<std::string, JsonNode> object_val;

    JsonNode() : type(JsonType::Null) {}
    JsonNode(bool b) : type(JsonType::Bool), bool_val(b) {}
    JsonNode(double n) : type(JsonType::Number), number_val(n) {}
    JsonNode(const std::string& s) : type(JsonType::String), string_val(s) {}
    JsonNode(const std::vector<JsonNode>& a) : type(JsonType::Array), array_val(a) {}
    JsonNode(const std::map<std::string, JsonNode>& o) : type(JsonType::Object), object_val(o) {}
};
#include "../include/Lexer.h"
// --- PHASE 3: THE PARSER ---
class Parser {
    std::vector<Token> tokens;
    size_t pos = 0;

    Token current() { return tokens[pos]; }
    void advance() { pos++; }
    Token consume(TokenType expected, const std::string& err) {
        if (current().type == expected) {
            Token tok = current();
            advance();
            return tok;
        }
        throw std::runtime_error(err);
    }

public:
    Parser(const std::vector<Token>& toks) : tokens(toks) {}

    JsonNode parseValue() {
        Token tok = current();
        if (tok.type == TokenType::STRING) { advance(); return JsonNode(tok.value); }
        if (tok.type == TokenType::NUMBER) { advance(); return JsonNode(std::stod(tok.value)); }
        if (tok.type == TokenType::BOOLEAN) { advance(); return JsonNode(tok.value == "true"); }
        if (tok.type == TokenType::NULL_TYPE) { advance(); return JsonNode(); }
        if (tok.type == TokenType::LBRACE) { return parseObject(); }
        if (tok.type == TokenType::LBRACKET) { return parseArray(); }
        throw std::runtime_error("Unexpected token in parseValue");
    }

    JsonNode parseObject() {
        std::map<std::string, JsonNode> obj;
        consume(TokenType::LBRACE, "Expected '{'");
        while (current().type != TokenType::RBRACE) {
            std::string key = consume(TokenType::STRING, "Expected string key").value;
            consume(TokenType::COLON, "Expected ':' after key");
            JsonNode val = parseValue();
            obj[key] = val;
            if (current().type == TokenType::COMMA) advance();
        }
        consume(TokenType::RBRACE, "Expected '}'");
        return JsonNode(obj);
    }

    JsonNode parseArray() {
        std::vector<JsonNode> arr;
        consume(TokenType::LBRACKET, "Expected '['");
        while (current().type != TokenType::RBRACKET) {
            arr.push_back(parseValue());
            if (current().type == TokenType::COMMA) advance();
        }
        consume(TokenType::RBRACKET, "Expected ']'");
        return JsonNode(arr);
    }
};

// --- PHASE 4: THE SERIALIZER ---
class Serializer {
public:
    static std::string stringify(const JsonNode& node, bool pretty = false, int indentLevel = 0) {
        std::string indent = pretty ? std::string(indentLevel * 4, ' ') : "";
        std::string nextIndent = pretty ? std::string((indentLevel + 1) * 4, ' ') : "";
        std::string newline = pretty ? "\n" : "";
        std::string space = pretty ? " " : "";

        if (node.type == JsonType::Null) return "null";
        if (node.type == JsonType::Bool) return node.bool_val ? "true" : "false";
        if (node.type == JsonType::Number) return std::to_string(node.number_val);
        if (node.type == JsonType::String) return "\"" + node.string_val + "\"";
        
        if (node.type == JsonType::Array) {
            if (node.array_val.empty()) return "[]";
            std::string res = "[" + newline;
            for (size_t i = 0; i < node.array_val.size(); i++) {
                res += nextIndent + stringify(node.array_val[i], pretty, indentLevel + 1);
                if (i < node.array_val.size() - 1) res += ",";
                res += newline;
            }
            return res + indent + "]";
        }
        
        if (node.type == JsonType::Object) {
            if (node.object_val.empty()) return "{}";
            std::string res = "{" + newline;
            size_t count = 0;
            for (const auto& pair : node.object_val) {
                res += nextIndent + "\"" + pair.first + "\":" + space + stringify(pair.second, pretty, indentLevel + 1);
                if (++count < node.object_val.size()) res += ",";
                res += newline;
            }
            return res + indent + "}";
        }
        return "";
    }
};

// --- PHASE 5: SCHEMA VALIDATOR ---
class SchemaValidator {
public:
    // Schema defines key -> Expected Type (e.g., "id" -> JsonType::Number)
    static std::vector<std::string> validate(const JsonNode& data, const std::map<std::string, JsonType>& schema) {
        std::vector<std::string> errors;
        
        if (data.type != JsonType::Object) {
            errors.push_back("Root element must be a JSON Object.");
            return errors;
        }

        for (const auto& requirement : schema) {
            const std::string& requiredKey = requirement.first;
            JsonType expectedType = requirement.second;

            if (data.object_val.find(requiredKey) == data.object_val.end()) {
                errors.push_back("Missing required field: '" + requiredKey + "'");
            } else if (data.object_val.at(requiredKey).type != expectedType) {
                errors.push_back("Type mismatch for field '" + requiredKey + "'.");
            }
        }
        return errors;
    }
};

// --- MAIN EXECUTION ---
int main() {
    // 1. Raw Data from the Network
    std::string rawJson = R"({
        "id": 101,
        "name": "Kartikey",
        "isStudent": true,
        "items": ["laptop", "book"]
    })";

    try {
        // Step A: Tokenize
        Lexer lexer(rawJson);
        std::vector<Token> tokens = lexer.tokenize();

        // Step B: Parse into AST
        Parser parser(tokens);
        JsonNode document = parser.parseValue();

        // Step C: Validate against Schema
        std::map<std::string, JsonType> mySchema = {
            {"id", JsonType::Number},
            {"items", JsonType::Array},
            {"age", JsonType::Number} // This will trigger an error (missing)
        };

        std::cout << "--- Validation Results ---\n";
        std::vector<std::string> errors = SchemaValidator::validate(document, mySchema);
        if (errors.empty()) {
            std::cout << "JSON is Valid!\n";
        } else {
            for (const auto& err : errors) {
                std::cout << "ERROR: " << err << "\n";
            }
        }

        // Step D: Serialize (Minified vs Pretty)
        std::cout << "\n--- Minified Output ---\n";
        std::cout << Serializer::stringify(document, false) << "\n";

        std::cout << "\n--- Pretty Print Output ---\n";
        std::cout << Serializer::stringify(document, true) << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Fatal Syntax Error: " << e.what() << "\n";
    }

    return 0;
}

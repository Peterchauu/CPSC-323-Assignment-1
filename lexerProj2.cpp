#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>

// Definitions for separators, operators, and keywords
std::set<std::string> separators = { "(", ")", ",", "{", "}", "|", ";" };
std::set<std::string> operators = { "==", "!=", ">", "<", "<=", "=>", "**", "*", "/", "+", "-", "=", "@" };
std::set<std::string> keywords = { "if", "else", "while", "return", "put", "get", "fi", "function",
                                   "integer", "boolean", "real", "true", "false" };

// Token struct
struct token {
    std::string type;
    std::string lexeme;
    int line_number;
};

// Corrected Identifier FSM
std::vector<std::vector<int>> identifier_states = {
    {1, -1, -1}, // State 0
    {1, 2, 3},   // State 1
    {1, 2, 3},   // State 2
    {-1, -1, -1} // State 3 (Accepting state)
};

bool is_identifier(const std::string& word) {
    int currentState = 0;
    int selected_char;
    std::string input = word + "$";

    for (const char& chr : input) {
        if (isalpha(chr)) {
            selected_char = 0; // Letter
        }
        else if (isdigit(chr)) {
            selected_char = 1; // Digit
        }
        else if (chr == '$') {
            selected_char = 2; // End of input
        }
        else {
            return false;
        }
        currentState = identifier_states[currentState][selected_char];
        if (currentState == -1) {
            return false;
        }
    }
    return currentState == 3; // Accepting state is state 3
}

// Corrected Integer FSM
std::vector<std::vector<int>> integer_states = {
    {1, -1}, // State 0
    {1, 2},  // State 1
    {-1, -1} // State 2 (Accepting state)
};

bool is_integer(const std::string& word) {
    int currentState = 0;
    int selected_char;
    std::string input = word + "$";

    for (const char& chr : input) {
        if (isdigit(chr)) {
            selected_char = 0; // Digit
        }
        else if (chr == '$') {
            selected_char = 1; // End of input
        }
        else {
            return false;
        }
        currentState = integer_states[currentState][selected_char];
        if (currentState == -1) {
            return false;
        }
    }
    return currentState == 2; // Accepting state is state 2
}

// Corrected Real Number FSM
std::vector<std::vector<int>> real_states = {
    {1, -1, -1}, // State 0
    {1, 2, -1},  // State 1
    {-1, 3, -1}, // State 2
    {3, 4, -1},  // State 3
    {-1, -1, -1} // State 4 (Accepting state)
};

bool is_real(const std::string& word) {
    int currentState = 0;
    int selected_char;
    std::string input = word + "$";

    for (const char& chr : input) {
        if (isdigit(chr)) {
            selected_char = 0; // Digit
        }
        else if (chr == '.') {
            selected_char = 1; // Dot
        }
        else if (chr == '$') {
            selected_char = 2; // End of input
        }
        else {
            return false;
        }
        currentState = real_states[currentState][selected_char];
        if (currentState == -1) {
            return false;
        }
    }
    return currentState == 4; // Accepting state is state 4
}

// Lexer function
std::vector<token> lexer(const std::string& expression, int line_number) {
    std::vector<token> token_list;
    std::string buffer;
    char chr;
    int iter = 0;
    bool comment_mode = false;

    while (iter < expression.size()) {
        chr = expression[iter];

        // Handle comments
        if (chr == '[' && expression.substr(iter, 2) == "[*") {
            comment_mode = true;
            iter += 2;
            continue;
        }
        else if (chr == '*' && expression.substr(iter, 2) == "*]") {
            comment_mode = false;
            iter += 2;
            continue;
        }

        if (comment_mode) {
            iter++;
            continue;
        }

        // Handle separators
        if (separators.count(std::string(1, chr)) > 0) {
            if (!buffer.empty()) {
                // Process buffer
                token new_token;
                new_token.lexeme = buffer;
                new_token.line_number = line_number;

                if (keywords.count(buffer) > 0) {
                    new_token.type = "Keyword";
                }
                else if (is_identifier(buffer)) {
                    new_token.type = "Identifier";
                }
                else if (is_integer(buffer)) {
                    new_token.type = "Integer";
                }
                else if (is_real(buffer)) {
                    new_token.type = "Real";
                }
                else {
                    new_token.type = "Unknown";
                }
                token_list.push_back(new_token);
                buffer.clear();
            }

            // Add separator to token list
            token new_token;
            new_token.lexeme = chr;
            new_token.type = "Separator";
            new_token.line_number = line_number;
            token_list.push_back(new_token);
            iter++;
        }
        // Handle operators
        else if (operators.count(std::string(1, chr)) > 0 ||
            (iter + 1 < expression.size() && operators.count(expression.substr(iter, 2)) > 0)) {
            if (!buffer.empty()) {
                // Process buffer
                token new_token;
                new_token.lexeme = buffer;
                new_token.line_number = line_number;

                if (keywords.count(buffer) > 0) {
                    new_token.type = "Keyword";
                }
                else if (is_identifier(buffer)) {
                    new_token.type = "Identifier";
                }
                else if (is_integer(buffer)) {
                    new_token.type = "Integer";
                }
                else if (is_real(buffer)) {
                    new_token.type = "Real";
                }
                else {
                    new_token.type = "Unknown";
                }
                token_list.push_back(new_token);
                buffer.clear();
            }

            // Check for multi-character operators
            std::string op;
            if (iter + 1 < expression.size() && operators.count(expression.substr(iter, 2)) > 0) {
                op = expression.substr(iter, 2);
                iter += 2;
            }
            else {
                op = chr;
                iter++;
            }

            // Add operator to token list
            token new_token;
            new_token.lexeme = op;
            new_token.type = "Operator";
            new_token.line_number = line_number;
            token_list.push_back(new_token);
        }
        // Accumulate characters
        else if (!isspace(chr)) {
            buffer += chr;
            iter++;
        }
        else {
            if (!buffer.empty()) {
                // Process buffer
                token new_token;
                new_token.lexeme = buffer;
                new_token.line_number = line_number;

                if (keywords.count(buffer) > 0) {
                    new_token.type = "Keyword";
                }
                else if (is_identifier(buffer)) {
                    new_token.type = "Identifier";
                }
                else if (is_integer(buffer)) {
                    new_token.type = "Integer";
                }
                else if (is_real(buffer)) {
                    new_token.type = "Real";
                }
                else {
                    new_token.type = "Unknown";
                }
                token_list.push_back(new_token);
                buffer.clear();
            }
            iter++;
        }
    }

    // Process any remaining buffer
    if (!buffer.empty()) {
        token new_token;
        new_token.lexeme = buffer;
        new_token.line_number = line_number;

        if (keywords.count(buffer) > 0) {
            new_token.type = "Keyword";
        }
        else if (is_identifier(buffer)) {
            new_token.type = "Identifier";
        }
        else if (is_integer(buffer)) {
            new_token.type = "Integer";
        }
        else if (is_real(buffer)) {
            new_token.type = "Real";
        }
        else {
            new_token.type = "Unknown";
        }
        token_list.push_back(new_token);
    }

    return token_list;
}

// Parser variables
std::vector<token> tokens;
int currentTokenIndex = 0;
std::ofstream output;

void syntax_error(const std::string& error_message) {
    if (currentTokenIndex < tokens.size()) {
        token currentToken = tokens[currentTokenIndex];
        std::cerr << "Syntax Error at line " << currentToken.line_number << ": " << error_message
            << " Token: " << currentToken.type << " Lexeme: " << currentToken.lexeme << std::endl;
    }
    else {
        std::cerr << "Syntax Error: " << error_message << std::endl;
    }
    exit(1);
}

token peek() {
    if (currentTokenIndex < tokens.size()) {
        return tokens[currentTokenIndex];
    }
    else {
        return { "EOF", "", -1 };
    }
}

void advance() {
    currentTokenIndex++;
}

void print_production(const std::string& production) {
    output << production << std::endl;
    std::cout << production << std::endl;
}

void print_token(const token& tok) {
    output << "Token: " << tok.type << " Lexeme: " << tok.lexeme << std::endl;
    std::cout << "Token: " << tok.type << " Lexeme: " << tok.lexeme << std::endl;
}

// Grammar rules implementations

void Assign();
void Expression();
void ExpressionPrime();
void Term();
void TermPrime();
void Factor();

void Assign() {
    if (peek().type == "Identifier") {
        print_token(peek()); // Token: Identifier Lexeme: a
        advance();

        if (peek().lexeme == "=") {
            print_production("<Statement> -> <Assign>");
            print_production("<Assign> -> <Identifier> = <Expression> ;");

            print_token(peek()); // Token: Operator Lexeme: =
            advance();

            Expression();

            if (peek().lexeme == ";") {
                print_token(peek()); // Token: Separator Lexeme: ;
                advance();
            }
            else {
                syntax_error("Expected ';'");
            }
        }
        else {
            syntax_error("Expected '='");
        }
    }
    else {
        syntax_error("Expected Identifier");
    }
}

void Expression() {
    print_production("<Expression> -> <Term> <Expression Prime>");
    Term();
    ExpressionPrime();
}

void ExpressionPrime() {
    if (peek().lexeme == "+") {
        print_token(peek()); // Token: Operator Lexeme: +
        advance();

        print_production("<Expression Prime> -> + <Term> <Expression Prime>");

        Term();
        ExpressionPrime();
    }
    else {
        print_production("<Expression Prime> -> epsln");
    }
}

void Term() {
    print_production("<Term> -> <Factor> <Term Prime>");
    Factor();
    TermPrime();
}

void TermPrime() {
    print_production("<Term Prime> -> epsln");
}

void Factor() {
    if (peek().type == "Identifier") {
        print_production("<Factor> -> <Identifier>");
        print_token(peek()); // Token: Identifier Lexeme: b or c
        advance();
    }
    else {
        syntax_error("Expected Identifier in <Factor>");
    }
}

int main() {
    std::string file_input;
    std::string file_output;

    std::cout << "Please enter your file name within this directory (please include extension): ";
    std::cin >> file_input;

    std::cout << "Name your output file for your token outputs: ";
    std::cin >> file_output;

    std::ifstream input(file_input);
    output.open(file_output);

    std::string line;
    std::vector<token> token_list;
    int line_number = 1;

    if (!input.is_open()) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }

    while (std::getline(input, line)) {
        // Remove potential carriage return character
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);
        }

        token_list = lexer(line, line_number);
        for (const token& tok : token_list) {
            tokens.push_back(tok);
        }
        line_number++;
    }

    currentTokenIndex = 0;
    Assign();

    std::cout << "Thank you for using our program, your token outputs have been saved to " << file_output << std::endl;

    output.close();
    input.close();

    return 0;
}

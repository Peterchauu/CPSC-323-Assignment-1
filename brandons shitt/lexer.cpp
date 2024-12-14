#include "lexer.h"

 std::set<std::string> separators = {
    "(",
    ")",
    ",",
    "{",
    "}",
    "|",
    ";"
};

std::set<std::string> operators = {
    "==",
    "!=",
    ">",
    "<",
    "<=",
    "=>",
    "**",
    "*",
    "/",
    "+",
    "-",
    "=",
    "@"
};

std::set<std::string> keywords = {
    "if",
    "else",
    "while",
    "return",
    "put",
    "get",
    "fi",
    "function",  
    "integer",
    "true",
    "false",
    "boolean"
};

bool comment_track = false;  

 std::vector<std::string> read_file(const std::string& input_file_name) {
    std::ifstream text(input_file_name);
    std::string lineStr;
    std::vector<std::string> unparsed_words;

    while (std::getline(text, lineStr)) {
        std::istringstream stream(lineStr);
        std::string word;
        while (stream >> word) {
            unparsed_words.push_back(word);
        }
    }
    text.close();
    return unparsed_words;
}

 
static std::vector<std::vector<int>> identifier_states = {
    {1, -1, -1},
    {1,  2, 42},
    {1,  2, -1}
};

bool is_identifier(std::string& word) {
    int currentState = 0;
    int selected_char;

    word += "$";  
    for (char chr : word) {
        if (isalpha(chr))       selected_char = 0;
        else if (isdigit(chr))  selected_char = 1;
        else if (chr == '$')    selected_char = 2;
        else {
            word.pop_back();
            return false;
        }
        currentState = identifier_states[currentState][selected_char];
        if (currentState == -1) {
            word.pop_back();
            return false;
        }
    }
    word.pop_back();
    return (currentState == 42);
}

 static std::vector<std::vector<int>> real_states = {
    {1,  3,  2, -1},
    {-1, -1, 3,  42},
    {4,  4, -1, -1},
    {3,  3,  2,  42},
    {4,  4, -1,  42}
};

bool is_real(std::string& real) {
    int currentState = 0;
    int selected_char;

    real += "$";
    for (char chr : real) {
        if (chr == '0')   selected_char = 0;
        else if (isdigit(chr)) selected_char = 1;
        else if (chr == '.')   selected_char = 2;
        else if (chr == '$')   selected_char = 3;
        else {
            real.pop_back();
            return false;
        }
        currentState = real_states[currentState][selected_char];
        if (currentState == -1) {
            real.pop_back();
            return false;
        }
    }
    real.pop_back();
    return (currentState == 42);
}

 static std::vector<std::vector<int>> integer_states = {
    {1, 2, -1},
    {-1,-1, 42},
    {2, 2, 42}
};

bool is_integer(std::string& integer) {
    int currentState = 0;
    int selected_char;

    integer += "$";
    for (char chr : integer) {
        if (chr == '0')   selected_char = 0;
        else if (isdigit(chr)) selected_char = 1;
        else if (chr == '$')   selected_char = 2;
        else {
            integer.pop_back();
            return false;
        }
        currentState = integer_states[currentState][selected_char];
        if (currentState == -1) {
            integer.pop_back();
            return false;
        }
    }
    integer.pop_back();
    return (currentState == 42);
}

 Lexer::Lexer() {
    std::string input_file;
    std::cout << "Please enter your file name within this directory (include extension): ";
    std::cin >> input_file;

    std::cout << "Name your output file for token listing (and final assembly): ";
    std::cin >> output_filename;  

    output.open(output_filename);
    std::ifstream input(input_file);

    std::string lineStr, expression;
    while (std::getline(input, lineStr)) {
        std::istringstream stream(lineStr);
        while (stream >> expression) {
            getLexer(expression);
        }
        this->line++;  
    }
    input.close();
}

void Lexer::PrintAndWriteToken(std::vector<token>::const_iterator& token_obj) {
    std::cout << "\nToken: " << std::left << std::setw(20) << token_obj->type
        << "Lexeme: " << token_obj->lexeme << "\n";

    if (output.is_open()) {
        output << "\nToken: " << std::left << std::setw(20) << token_obj->type
            << "Lexeme: " << token_obj->lexeme << "\n";
    }
}

void Lexer::PrintAndWriteToken(std::string rule) {
    // No-op: previously used for grammar rules, now suppressed
}

bool Lexer::DirtyString(std::string s) {
    return (operators.count(s) > 0 || separators.count(s) > 0);
}

void Lexer::getLexer(std::string& expression) {
    token token_obj;
    std::string buffer, chr, temp;

    for (unsigned int i = 0; i < expression.size(); i++) {
        chr = expression[i];
        buffer += chr;
        if (!DirtyString(chr)) {
            temp += chr;
        }
        if (!comment_track && keywords.count(expression) > 0) {
            token_obj.lexeme = expression;
            token_obj.type = "Keyword";
            token_obj.line_number = line;
            token_list.push_back(token_obj);
            buffer.clear();
            break;
        }
        else if (!comment_track && (DirtyString(chr) || i == expression.size() - 1)) {
            if (keywords.count(temp) > 0) {
                token_obj.lexeme = temp;
                token_obj.type = "Keyword";
                token_obj.line_number = line;
                token_list.push_back(token_obj);
                buffer.clear();
                temp.clear();
            }
        }

        if (!comment_track && is_identifier(expression)) {
            token_obj.lexeme = expression;
            token_obj.type = "Identifier";
            token_obj.line_number = line;
            token_list.push_back(token_obj);
            buffer.clear();
            break;
        }
        else if (!comment_track && (DirtyString(chr) || i == expression.size() - 1)) {
            if (is_identifier(temp)) {
                token_obj.lexeme = temp;
                token_obj.type = "Identifier";
                token_obj.line_number = line;
                token_list.push_back(token_obj);
                buffer.clear();
                temp.clear();
            }
        }

        if (!comment_track && is_integer(expression)) {
            token_obj.lexeme = expression;
            token_obj.type = "integer";
            token_obj.line_number = line;
            token_list.push_back(token_obj);
            buffer.clear();
            break;
        }
        else if (!comment_track && (DirtyString(chr) || i == expression.size() - 1)) {
            if (is_integer(temp)) {
                token_obj.lexeme = temp;
                token_obj.type = "integer";
                token_obj.line_number = line;
                token_list.push_back(token_obj);
                buffer.clear();
                temp.clear();
            }
        }

        if (!comment_track && is_real(expression)) {
            token_obj.lexeme = expression;
            token_obj.type = "Real";
            token_obj.line_number = line;
            token_list.push_back(token_obj);
            buffer.clear();
            break;
        }
        else if (!comment_track && (DirtyString(chr) || i == expression.size() - 1)) {
            if (is_real(temp)) {
                token_obj.lexeme = temp;
                token_obj.type = "Real";
                token_obj.line_number = line;
                token_list.push_back(token_obj);
                buffer.clear();
                temp.clear();
            }
        }

        if (expression.substr(i, 2) == "[*" || expression == "[*") {
            comment_track = true;
            buffer.clear();
        }
        if (expression.substr(i, 2) == "*]" || expression == "*]") {
            comment_track = false;
            buffer.clear();
            break;
        }

        if (separators.count(chr) > 0 && !comment_track) {
            token_obj.type = "seperator";
            token_obj.lexeme = chr;
            token_obj.line_number = line;
            token_list.push_back(token_obj);
            buffer.clear();
        }

        if ((!comment_track && operators.count(chr) > 0) ||
            (!comment_track && operators.count(expression.substr(i, 2)) > 0)) {
            token_obj.type = "operator";
            if (expression.length() > 1 && chr != "-") {
                token_obj.lexeme = expression.substr(i, 2);
                token_obj.line_number = line;
                token_list.push_back(token_obj);
                buffer.clear();
                i += 1;
            }
            else {
                token_obj.lexeme = chr;
                token_obj.line_number = line;
                token_list.push_back(token_obj);
                buffer.clear();
            }
        }
    }

    if (!comment_track && !buffer.empty()) {
        token_obj.lexeme = expression;
        token_obj.type = "Error";
        token_obj.line_number = line;
        token_list.push_back(token_obj);
    }
}

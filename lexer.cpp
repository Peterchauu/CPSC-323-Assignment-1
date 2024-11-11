#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>


// Declaration for all the necessary separators, operators, and keywords for the Rat24f language

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
    "false"
};


// Function for reading file and outputs a list of every word within the file separated by whitespace

std::vector<std::string> read_file(const std::string& input_file_name) {
    std::ifstream text (input_file_name);
    std::string line;
    std::string word;
    std::vector<std::string> unparsed_words;

    while (std::getline(text, line)) {

        std::istringstream stream(line);
        while (stream >> word) {
            unparsed_words.push_back(word);
        }
    }
    text.close();
    return unparsed_words;
}




// Implementation of Identifier FSM

std::vector<std::vector<int>> identifier_states = {

    {1, -1, -1},
    {1, 2, 42},
    {1, 2, -1} 
};

bool is_identifier(std::string& word) {
    int currentState = 0;
    int selected_char;

    word += "$";

    for (const char& chr :word) {
        if (isalpha(chr)) {
            selected_char = 0;
        } else if (isdigit(chr)) {
            selected_char = 1;
        } else if (chr == '$') {
            selected_char = 2;
        } else {
            word.pop_back();
            return false;
        }
        switch(selected_char) {
            case 0:
                currentState = identifier_states[currentState][selected_char];
                break;
            case 1:
                currentState = identifier_states[currentState][selected_char];
                break;
            case 2:
                currentState = identifier_states[currentState][selected_char];
                break;
        }
        if (currentState == -1) {
            word.pop_back();
            return false;
        }
    }
    word.pop_back();
    return currentState == 42;
}




// Implementation of Real Numbers FSM

std::vector<std::vector<int>> real_states = {
    {1, 3, 2, -1},
    {-1,-1,3, 42},
    {4, 4,-1, -1},         
    {3, 3, 2, 42},          
    {4, 4,-1, 42}               
};

bool is_real(std::string& real) {
    int currentState = 0;
    int selected_char;

    real += "$";
    for (const char& chr : real) {
        if (chr == '0') {
            selected_char = 0;
        } else if (isdigit(chr)) {
            selected_char = 1;
        } else if (chr == '.') {
            selected_char = 2;
        } else if (chr == '$'){
            selected_char = 3;
        } else {
            real.pop_back();
            return false;
        }
        switch(selected_char) {
            case 0:
                currentState = real_states[currentState][selected_char];
                break;
            case 1:
                currentState = real_states[currentState][selected_char];
                break;
            case 2:
                currentState = real_states[currentState][selected_char];
                break;
            case 3:
                currentState = real_states[currentState][selected_char];
                break;
        }
        if (currentState == -1) {
            real.pop_back();
            return false;
        }
    }
    real.pop_back();
    return currentState == 42;
}




// Implementation of Integer FSM

std::vector<std::vector<int>> integer_states = {
    {1, 2, -1},
    {-1,-1, 42},
    {2, 2, 42}

};

bool is_integer(std::string& integer) {
    int currentState = 0;
    int selected_char;
    integer += "$";

    for (const char& chr : integer) {
        if (chr == '0') {
            selected_char = 0;
        } else if (isdigit(chr)) {
            selected_char = 1;
        } else if (chr == '$') {
            selected_char = 2;
        } else {
            integer.pop_back();
            return false;
        }
        switch(selected_char) {
            case 0:
                currentState = integer_states[currentState][selected_char];
                break;
            case 1:
                currentState = integer_states[currentState][selected_char];
                break;
            case 2:
                currentState = integer_states[currentState][selected_char];
                break;
        }
        if (currentState == -1) {
            integer.pop_back();
            return false;
        }
    }
    integer.pop_back();
    return currentState == 42;
}

bool comment_track = false;





// Creating a token struct to store specific token types and their respective lexeme

struct token {
    std::string type;
    std::string lexeme;
    int line_number;
};

// Implementation of core Lexer function, takes input of a single expression.
class Lexer{

public:

    std::vector<token> token_list;

    Lexer() {

        std::string input_file;
        std::string output_file;

        std::cout << "Please enter your file name within this directory (please include extension): ";
        std::cin >> input_file;

        std::cout << "Name your output file for your token and production rule outputs: ";
        std::cin >> output_file;

        output.open(output_file);
        std::ifstream input (input_file);

        std::string line;
        std::string expression;

        while (std::getline(input, line)) {

            std::istringstream stream(line);
            while (stream >> expression) {
                getLexer(expression);
            }
            Lexer::line++;
        }

        std::ofstream output (output_file);

    }

    void PrintAndWriteToken(std::vector<token>::const_iterator& token_obj) {
        if (output.is_open()) {
            output << "\n" << std::left << "Token: " << std::setw(20) << token_obj->type << "Lexeme: " << token_obj->lexeme << "\n\n";
        }
        std::cout << "\n" << std::left << "Token: " << std::setw(20) << token_obj->type << "Lexeme: " << token_obj->lexeme << "\n\n";
        token_obj++;
    }

    void PrintAndWriteToken(std::string rule) {
        if (output.is_open()) {
            output << rule << std::endl;
        }
        std::cout << rule << std::endl;
    }
    
private:

    std::ofstream output;
    int line = 1;

    void getLexer(std::string& expression) {

        token token_obj;
        std::string buffer;
        std::string chr;
        std::string temp;

        for (unsigned int iter = 0; iter < expression.size(); iter++) {
            chr = expression[iter];
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

            } else if (!comment_track && (DirtyString(chr) || iter == expression.size() - 1)) {
                
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
                
            } else if (!comment_track && (DirtyString(chr) || iter == expression.size() - 1)) {

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

            } else if (!comment_track && (DirtyString(chr) || iter == expression.size() - 1)) {
                
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

            } else if (!comment_track && (DirtyString(chr) || iter == expression.size() - 1)) {

                if (is_real(temp)) {
                    token_obj.lexeme = temp;
                    token_obj.type = "Real";
                    token_obj.line_number = line;

                    token_list.push_back(token_obj);
                    buffer.clear();
                    temp.clear();
                }
            }

            if (expression.substr(iter, 2) == "[*" || expression == "[*") {
                comment_track = true;

                buffer.clear();
            }

            if (expression.substr(iter, 2) == "*]" || expression == "*]") {
                comment_track = false;

                buffer.clear();
                break;
            }

            if (separators.count(chr) > 0 && !comment_track ) {
                token_obj.type = "seperator";
                token_obj.lexeme = chr;
                token_obj.line_number = line;

                token_list.push_back(token_obj);

                buffer.clear();
            }

            if ((operators.count(chr) > 0 && !comment_track) || (operators.count(expression.substr(iter, 2)) > 0 && !comment_track)) {

                token_obj.type = "operator";
                if (expression.length() > 1 && chr != "-") {
                    token_obj.lexeme = expression.substr(iter, 2);
                    token_obj.line_number = line;

                    token_list.push_back(token_obj);

                    buffer.clear();

                    iter += 1;

                } else {
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

    bool DirtyString(std::string string) {
        return (operators.count(string) > 0 || separators.count(string) > 0);
    }
};
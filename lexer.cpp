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




// Creating a token struct to store specific token types and their respective lexeme

struct token {
    std::string type;
    std::string lexeme;
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
    {-1,-1,2, 42},
    {-1, -1,-1, -1},
    {3, 3, 4, 42},
    {4, 4,-1, 42}
};

bool is_real(std::string& real) {
    int currentState = 0;
    int selected_char;

    real += "$";
    for (const char& chr :real) {
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




// Implementation of core Lexer function, takes input of a single expression.

std::vector<token> lexer(std::string& expression) {
    std::string buffer;
    token new_token;
    std::string chr;
    std::vector<token> token_list;
    for (unsigned int iter = 0; iter < expression.size(); iter++) {
        chr = expression[iter];
        buffer += chr;
        if (expression.substr(iter, 2) == "[*") {
            comment_track = true;

        } else if (expression.substr(iter, 2) == "*]") {
            comment_track = false;

            buffer.clear();
            iter += 1;

        } else if (separators.count(chr) > 0 && !comment_track) {
            new_token.type = "seperator";
            new_token.lexeme = chr;

            token_list.push_back(new_token);

            buffer.pop_back();

        } else if ((operators.count(chr) > 0 && !comment_track) || (operators.count(expression.substr(iter, 2)) > 0 && !comment_track)) {

            new_token.type = "operator";
            std::cout << "This is the entire operator: " << expression.substr(iter, 2) << std::endl;
            if (expression.length() > 1) {
                buffer = expression.substr(iter, 2);
                new_token.lexeme = buffer;

                token_list.push_back(new_token);

                buffer.clear();

                iter += 1;

            } else {
                new_token.lexeme = chr;

                token_list.push_back(new_token);

                buffer.clear();
            }
        }
    }
    if (!comment_track && keywords.count(buffer) > 0){
        new_token.lexeme = buffer;
        new_token.type = "Keyword";

        token_list.push_back(new_token);

    } else if (!comment_track && is_identifier(buffer)) {
        new_token.lexeme = buffer;
        new_token.type = "Identifier";

        token_list.push_back(new_token);

    } else if (!comment_track && is_integer(buffer)) {
        new_token.lexeme = buffer;
        new_token.type = "Integer";

        token_list.push_back(new_token); 

    } else if (!comment_track && is_real(buffer)) {
        new_token.lexeme = buffer;
        new_token.type = "Real";

        token_list.push_back(new_token);

    } else if (!comment_track && !buffer.empty()) {
        new_token.lexeme = buffer;
        new_token.type = "Error";

        token_list.push_back(new_token);
        
    }
    return token_list;
} 

int main() {
    std::string file_input;
    std::string file_output;

    std::cout << "Please enter your file name within this directory (please include extension): ";
    std::cin >> file_input;

    std::cout << "Name your output file for your token outputs: ";
    std::cin >> file_output;

    std::ifstream input (file_input);
    std::ofstream output (file_output);

    std::string line;
    std::string expression;
    std::vector<token> token_list;

    if (output.is_open()) {
        output << std::setw(20) << std::left << "token" << std::setw(20) << "Lexeme" << std::endl;
        output << std::setw(40) << std::setfill('~') << "";
        output << std::setw(40) << std::setfill(' ') << std::endl;
    }
    while (std::getline(input, line)) {
        
        std::istringstream stream(line);
        while (stream >> expression) {
            token_list = lexer(expression);
            for (token token : token_list) {
                std::cout << std::left 
                << "Token Type: " << std::setw(15) << token.type
                << "Lexeme: " << token.lexeme << std::endl;
                output << std::setw(20) << token.type << std::setw(20) << token.lexeme << std::endl;
            }
        }
    }
    
    std::cout << "Thank you for using our program, your token outputs have been saved to " << file_output << std::endl;

    return 0;
}
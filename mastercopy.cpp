#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>

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
    "=",
    "!",
    ">",
    "<",
    "*",
    "/",
    "+",
    "-"

};

std::set<std::string> keywords = {
    "if",
    "else",
    "while",
    "return",
    "put",
    "get",
    "fi"
};



struct token {
    std::string type;
    std::string lexeme;
};


std::vector<std::string> read_file(const std::string input_file_name) {
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

void write_to_file(const std::string output_file_name, const std::vector<token> token_list) {
    std::ofstream text (output_file_name);
    if (text.is_open()) {
        text << std::setw(20) << std::left << "token" << std::setw(20) << "Lexeme" << std::endl;
        text << std::setw(40) << std::setfill('~') << "";
        text << std::setw(40) << std::setfill(' ') << std::endl;
        for (const token& token : token_list) {
            text << std::setw(20) << std::left << token.type << std::setw(20) << token.lexeme << std::endl;

        }
    }
}

std::vector<std::vector<int>> identifier_states = {
    {1, -1, -1},
    {1, 2, 42},
    {1, 1, -1}
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


std::vector<std::vector<int>> real_states = {

//   0 1-9 .  $
    {1, 3, 2, -1},
    {-1,-1,2, 42},
    {3, 3,-1, -1},
    {3, 3, 2, 42},
};

bool is_real(std::string& word) {
    int currentState = 0;
    int selected_char;

    word += "$";
    for (const char& chr :word) {
        if (chr == '0') {
            selected_char = 0;
        } else if (isdigit(chr)) {
            selected_char = 1;
        } else if (chr == '.') {
            selected_char = 2;
        } else if (chr == '$'){
            selected_char = 3;
        } else {
            word.pop_back();
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
            word.pop_back();
            return false;
        }
    }
    word.pop_back();
    return currentState == 42;
}


std::vector<std::vector<int>> integer_states = {
    {1, 2, -1},
    {-1,-1, 42},
    {2, 2, 42}

};

bool is_integer(std::string& word) {
    int currentState = 0;
    int selected_char;
    word += "$";

    for (const char& chr :word) {
        if (chr == '0') {
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
            word.pop_back();
            return false;
        }
    }
    word.pop_back();
    return currentState == 42;
}


std::vector<token> filter_words(std::vector<std::string> word_list) {
    std::string buffer;
    token new_token;
    bool comment_track = false;
    std::vector<token> token_list;
    std::string chr;


    for (std::string word : word_list) {
        buffer.clear();
        for (unsigned int iter = 0; iter < word.size(); iter++) {
            chr = word[iter];
            buffer += chr;
            if (word.substr(iter, 2) == "[*") {
                comment_track = true;
            } else if (word.substr(iter, 2) == "*]") {
                comment_track = false;
                buffer.clear();
                iter += 1;
            } else if (separators.count(chr) > 0 && !comment_track) {
                new_token.type = "seperator";
                new_token.lexeme = chr;
                token_list.push_back(new_token);
                buffer.erase(iter);
            } else if (operators.count(chr) > 0 && !comment_track) {
                new_token.type = "operator";
                if (operators.count(&word[iter + 1]) > 0) {
                    buffer += word[iter + 1];
                    new_token.lexeme = buffer;
                    token_list.push_back(new_token);
                    buffer.erase(iter);
                    break;
                } else {
                    new_token.lexeme = chr;
                    token_list.push_back(new_token);
                    buffer.erase(iter);
                    break;
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
    }
    return token_list;

}

int main() {
    std::string file_input;
    std::string file_output;

    std::cout << "Please enter your file name within this directory: ";
    std::cin >> file_input;

    std::cout << "Enter a designated file name for your token output (Press 'Enter' to skip): ";
    std::cin >> file_output;

    std::vector<std::string> unfiltered_words = read_file(file_input);
    std::vector<token> token_list = filter_words(unfiltered_words);
    write_to_file(file_output, token_list);

    return 0;
}
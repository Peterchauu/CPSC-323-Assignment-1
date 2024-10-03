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
    "==",
    "!=",
    ">",
    "<",
    "<=",
    "=>",
    "*",
    "/",
    "+",
    "-",
    "="

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

std::set<std::string> comment_lexeme = {
    "[*",
    "*]"
};


struct token {
    std::string type;
    std::string lexeme;
};

//incorporate our finite state machine which returns the token and the lexeme associated with it.
/*struct token lexer(std::string word) {
    token* token_record;
    if (word == "!") {
        token_record->lexeme = word;
        token_record->token_type = "comment";
    }
    return *token_record;
};*/

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
};

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

std::vector<token> filter_words(std::vector<std::string> word_list) {
    std::string buffer;
    token new_token;
    bool comment_track = false;
    std::string comment_text;
    std::vector<token> token_list;
    std::string chr;


    for (std::string word : word_list) {
        new_token.lexeme = buffer;
        if (!comment_track && keywords.count(buffer) > 0){
            new_token.type = "Keyword";
            token_list.push_back(new_token);
            std::cout << new_token.type << " " << new_token.lexeme << std::endl;
            buffer.clear();

        //Every other lexer type has been verified for the entire word string, could be a integer, real or identifier, implement the finite state machines here
        } else if (!comment_track && !new_token.lexeme.empty()) {
            buffer.append("$");
            new_token.type = "Identifier";
            token_list.push_back(new_token);
            std::cout << new_token.type << " " << new_token.lexeme << std::endl;
            buffer.clear();
        }
        for (unsigned int iter = 0; iter < word.size(); iter++) {
            chr = word[iter];
            buffer += chr;
            std::cout << "On this letter: " << chr << std::endl;
            if (comment_lexeme.count(chr) > 0) {
                comment_track = true;
                break;
            } else if (comment_lexeme.count(chr) > 0) {
                comment_track = false;
                iter += 2;
            } else if (separators.count(chr) > 0 && !comment_track) {
                new_token.type = "seperator";
                new_token.lexeme = chr;
                token_list.push_back(new_token);
                std::cout << new_token.type << " " << new_token.lexeme << std::endl;
                buffer.clear();
            } else if (operators.count(chr) > 0 && !comment_track) {
                new_token.type = "operator";
                if (operators.count(&word[iter + 1]) > 0) {
                    buffer += word[iter + 1];
                    new_token.lexeme = buffer;
                    token_list.push_back(new_token);
                    buffer.clear();
                    std::cout << new_token.type << " " << new_token.lexeme << std::endl;
                    break;
                } else {
                    new_token.lexeme = chr;
                    token_list.push_back(new_token);
                    std::cout << new_token.type << " " << new_token.lexeme << std::endl;
                    buffer.clear();
                }
            }
            std::cout << "Buffer: " << buffer << " Chr: " << chr << std::endl;

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
};
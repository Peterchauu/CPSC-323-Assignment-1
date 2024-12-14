#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>

extern std::set<std::string> separators;
extern std::set<std::string> operators;
extern std::set<std::string> keywords;

extern bool comment_track;   

 struct token {
    std::string type;
    std::string lexeme;
    int line_number;
};

 class Lexer {
public:
    std::vector<token> token_list;
    std::string output_filename;  

    Lexer();
    void PrintAndWriteToken(std::vector<token>::const_iterator& token_obj);
    void PrintAndWriteToken(std::string rule);  

private:
    std::ofstream output;
    int line = 1;   

    void getLexer(std::string& expression);
    bool DirtyString(std::string s);
};

 std::vector<std::string> read_file(const std::string& input_file_name);
bool is_identifier(std::string& word);
bool is_integer(std::string& integer);
 bool is_real(std::string& real);

#endif

#include <iostream>

#include "lexer.cpp"

Lexer lexer_obj;
std::vector<token>::const_iterator token_iter = lexer_obj.token_list.cbegin();

bool output_rule = false;


int main() {

    Rat24F();
    
    return 0;
}

void Match(std::string desired_string, std::vector<token>::const_iterator token_obj) {
    if (desired_string != token_obj->lexeme) {
        std::cout << "Encountered Error, current lexeme {" << token_obj->lexeme << "} does not match with desired lexeme {" << desired_string << "}... Terminating" << std::endl;
        exit(1);
    } else if (desired_string != token_obj->type) {
        std::cout << "Encountered Error, current lexeme {" << token_obj->type << "} does not match with desired lexeme {" << desired_string << "}... Terminating" << std::endl;
        exit(1);
    } else {
        token_obj++;
    }
}

void MatchToken(std::string desired_token, std::string curr_token) {
    if (desired_token != curr_token) {
        std::cout << "Encountered Error, current lexeme {" << curr_token << "} does not match with desired lexeme {" << desired_token << "}... Terminating" << std::endl;
        exit(1);
    }
}

void Rat24F() {

    std::string response;

    std::cout << "Turn on parse rule outputs? (Y/N): ";
    std::cin >> response;

    if (response == "Y" || response == "y") {
        output_rule = true;
    }

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Rat24F> ::= <Opt Function Definitions> @ <Opt Declaration List> <Statement List> @");
    }

    OptFuncDef();

    if (token_iter->lexeme == "@") {
        lexer_obj.PrintAndWriteToken(token_iter);
        token_iter++;
    } else {
        std::cout << "Expected @ after Function Definitions... Terminating" << std::endl;
        return;
    }

    OptDeclList();
    StatementsList();

    if (token_iter->lexeme == "@") {
        lexer_obj.PrintAndWriteToken(token_iter);
    } else {
        std::cout << "Expected @ after Statement List... Terminating" << std::endl;
        return;
    }

}

void OptFuncDef() {
    
    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Opt Function Definitions> ::= <Function Definitions> | <Empty>");
    }

    if (token_iter->lexeme != "function") {
        return;
    }

    FuncDef();

}

void FuncDef() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

    Function();
    FuncDefPrime();

}

void FuncDefPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

    FuncDef();

}

void Function() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

    Match("function", token_iter);
    Match("Identifier", token_iter);
    Match("(", token_iter);
    OptParamList();
    Match(")", token_iter);
    OptDeclList();
    Body();

}

void OptParamList() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }


}

void ParamList() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void ParamListPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Parameter() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Qualifier() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Body() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void OptDeclList() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void DeclList() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void DeclListPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Declaration() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Idents() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void IdentsPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void StatementsList() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void StatementsListPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Statement() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Compound() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Assign() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void IfCondition() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void IfPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void ReturnStuff() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void ReturnPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void PrintStuff() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void ScanStuff() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void WhileCondition() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Condition() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Relop() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Expression() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void ExpressionPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Term() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void TermPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Factor() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}

void Primary() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("");
    }

}
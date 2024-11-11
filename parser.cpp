#include <iostream>

#include "lexer.cpp"


Lexer lexer_obj;
std::vector<token>::const_iterator token_iter = lexer_obj.token_list.cbegin();

bool output_rule = false;

bool Match(std::string, std::vector<token>::const_iterator);
void Rat24F();
void OptFuncDef();
void FuncDef();
void FuncDefPrime();
void Function();
void OptParamList();
void ParamList();
void ParamListPrime();
void Parameter();
void Qualifier();
void Body();
void OptDeclList();
void DeclList();
void DeclListPrime();
void Declaration();
void Idents();
void IdentsPrime();
void StatementsList();
void StatementsListPrime();
void Statement();
void Compound();
void Assign();
void IfCondition();
void IfPrime();
void ReturnStuff();
void ReturnPrime();
void PrintStuff();
void ScanStuff();
void WhileCondition();
void Condition();
void Relop();
void Expression();
void ExpressionPrime();
void Term();
void TermPrime();
void Factor();
void Primary();


bool Match(std::string desired_string, std::vector<token>::const_iterator token_obj, bool error_toggle) {
    if (desired_string != token_obj->lexeme && error_toggle) {
        std::cout << "Encountered Error, current lexeme {" << token_obj->lexeme << "} does not match with desired lexeme {" << desired_string << "}... Terminating" << std::endl;
        return false;
    } else if (desired_string != token_obj->type && error_toggle) {
        std::cout << "Encountered Error, current lexeme {" << token_obj->type << "} does not match with desired lexeme {" << desired_string << "}... Terminating" << std::endl;
        return false;
    } else if (desired_string != token_obj->lexeme || desired_string != token_obj->lexeme) {
        return false;
    }

    lexer_obj.PrintAndWriteToken(token_obj);

    token_obj++;
    return true;

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

    if (!Match("@", token_iter, false)) {
        std::cout << "ERROR, line " << token_iter->line_number << ": Expected @ ... Terminating" << std::endl;
        exit(1);
    }

    OptDeclList();
    StatementsList();

    if (!Match("@", token_iter, false)) {
        std::cout << "ERROR, line " << token_iter->line_number << ": Expected @ ... Terminating" << std::endl;
        exit(1);
    }

}

void OptFuncDef() {
    
    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Opt Function Definitions> ::= <Function Definitions> | <Empty>");
    }

    if (token_iter->lexeme == "function") {
        FuncDef();
    }

}

void FuncDef() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Function Definitions> ::= <Function><Function Definitions Prime>");
    }
    
    Function();
    FuncDefPrime();

}

void FuncDefPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Function Definitions Prime> ::= <Function Definitions> | <Empty>");
    }

    if (token_iter->lexeme == "function") {
        FuncDef();
    }

}
void Function() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>");
    }

    Match("function", token_iter, true);
    Match("Identifier", token_iter, true);
    Match("(", token_iter, true);
    OptParamList();
    Match(")", token_iter, true);
    OptDeclList();
    Body();

}
void OptParamList() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Opt Parameter List> ::= <Parameter List> | <Empty>");
    }

    if (token_iter->lexeme != ")") {
        ParamList();
    }
// Kathy: thought this might be needed. If not, then delete it please.
}
void ParamList() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Parameter List> ::= <Parameter> | <Parameter List Prime>");
    }

    Parameter();
    ParamListPrime();

}
void ParamListPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Parameter List Prime> ::= <Parameter List> | <Empty>");
    }
    
    if (Match(",", token_iter, false)) {
        ParamList();
    }
    
    

}
void Parameter() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Parameter> ::= <IDs> <Qualifier>");
    }

    Idents();
    Qualifier();

}
void Qualifier() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Qualifier> ::= integer | boolean | real");
    }

    if (token_iter->lexeme == "integer" || token_iter->lexeme == "boolean" || token_iter->lexeme == "real") {
        lexer_obj.PrintAndWriteToken(token_iter);
        return;
    } else {
        std::cout << "ERROR, Line " << token_iter->line_number << ": Expected type 'integer', 'boolean', or 'real'... Terminating" << std::endl;
        exit(1);
    }

// Kathy: I'm not sure if me typing this might help. Printing the production rule for Qualifer might be different.
// Maybe we use an if statement that checks whether the token type is either integer, boolean, or real
// If it's any one of those three, then it prints out either one of those three.
// If not, then maybe it prints out an error message?
// It's also possible there are more that print similar to this.

}
void Body() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Body> ::= { <Statement List> }");
    }

    Match("{", token_iter, true);
    StatementsList();
    Match("}", token_iter, true);

}
void OptDeclList() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Opt Declaration List> ::= <Declaration List> | <Empty>");
    }

    DeclList();

}
void DeclList() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Declaration List> ::= <Declaration>;<Declaration List Prime>");
    }

    Declaration();
    Match(";", token_iter, true);
    DeclListPrime();

}
void DeclListPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Declaration List Prime> ::= <Declaration List> | <Empty>");
    }

    DeclList();

}
void Declaration() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Declaration> ::= <Qualifier> <IDs>");
    }

    Qualifier();
    Idents();

}
void Idents() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<IDs> ::= <Identifier> | <ID Prime>");
    }

    Match("Identifier", token_iter);
    IdentsPrime();


}
void IdentsPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<ID Prime> ::= <IDs> | <Empty>");
    }

    if(token_iter->type == "Identifier") {
        Idents();
    }

}
void StatementsList() {
    
    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Statement List> ::= <Statement> | <Statement List Prime>");
    }

    Statement();
    StatementsListPrime();
    
}
void StatementsListPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Statement List Prime> ::= <Statement List> | <Empty>");
    }

    StatementsList();

}
void Statement() {
    
    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
    }

    if (token_iter->lexeme == "{" ) {
        Compound();
    } else if (token_iter->type == "Identifier") {
        Assign();
    } else if (token_iter->lexeme == "if") {
        IfCondition();
    } else if (token_iter->lexeme == "return") {
        ReturnStuff();
    } else if (token_iter->lexeme == "put") {
        PrintStuff();
    } else if (token_iter->lexeme == "get") {
        ScanStuff();
    } else if (token_iter->lexeme == "while") {
        WhileCondition();
    }

}
void Compound() {
    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Compound> ::= { <Statement List> }");
    }

    Match("{", token_iter, true);
    StatementsList(); 
    Match("{", token_iter, true);

}
void Assign() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Assign> ::= <Identifier> = <Expression>");
    }

    if (Match("Identifier", token_iter, true)) {
        Match("=", token_iter, true);
        Expression();
    }

}
void IfCondition() {
    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<If> ::= if ( <Condition> )<Statement> <If Prime> fi");
    }

    Match("if", token_iter, true);
    Match("(", token_iter, true);
    Condition();
    Match(")", token_iter, true);
    Statement();
    IfPrime();
    Match("fi", token_iter, true);

}
void IfPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<If Prime> ::= else <Statement> | <Empty>");
    }

    if (Match("else", token_iter, false)) {
        Statement();
    }

}
void ReturnStuff() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Return> ::= return <Return Prime>;");
    }

    if (token_iter->lexeme == "return") {
        ReturnPrime();
    }

}
void ReturnPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Return Prime> ::= <Expression> | <Empty>");
    }
    if (Match("return", token_iter, false)) {
        Expression();
    }

}
void PrintStuff() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Print> ::= put ( <Expression> );");
    }

    Match("put", token_iter, true);
    Match("(", token_iter, true);
    Expression();
    Match(")", token_iter, true);
    Match(";", token_iter, true);

}
void ScanStuff() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Scan> ::= get ( <IDs> );");
    }

    Match("get", token_iter, true);
    Match("(", token_iter, true);
    Idents();
    Match(")", token_iter, true);
    Match(";", token_iter, true);


}
void WhileCondition() {
    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<While> ::= while ( <Condition> ) <Statement>");
    }

    Match("while", token_iter, true);
    Match("(", token_iter, true);
    Condition();
    Match(")", token_iter, true);
    Statement();

}
void Condition() {
    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Condition> ::= <Expression> <Relop> <Expression>");
    }

    Expression();
    Relop();
    Expression();

}
void Relop() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Relop> ::= == | != | > | < | <= | =>");
    }

    if (token_iter->lexeme == "==") {
        Match("==", token_iter, true);
    }
    else if (token_iter->lexeme == "!=") {
        Match("!=", token_iter, true);
    }
    else if (token_iter->lexeme == ">") {
        Match(">", token_iter, true);
    }
    else if (token_iter->lexeme == "<") {
        Match("<", token_iter, true);
    }
    else if (token_iter->lexeme == "<=") {
        Match("<=", token_iter, true);
    }
    else if (token_iter->lexeme == "=>") {
        Match("=>", token_iter, true);
    } else {
        std::cout << "ERROR, line " << token_iter->line_number << ": Expected == | != | > | < | <= | => ... Terminating" << std::endl;
        exit(1);
    }

}
void Expression() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Expression> ::= <Term><Expression Prime>");
    }

    Term();
    ExpressionPrime();

}
void ExpressionPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Expression Prime> ::= +<Term><Expression Prime> | -<Term><Expression Prime> | <Empty>");
    }

    if (token_iter->lexeme == "+" || token_iter->lexeme == "-") {
        lexer_obj.PrintAndWriteToken(token_iter);
        Term();
        ExpressionPrime();
    }

}
void Term() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Term> ::= <Factor><Term Prime>");
    }

    Factor();
    TermPrime();

}
void TermPrime() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Term Prime> ::= *<Factor><Term Prime> | /<Factor><Term Prime> | <Empty>");
    }
    
    if (Match("*", token_iter, false) || Match("/", token_iter, false)) {
        Factor();
        TermPrime();
    }

}
void Factor() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Factor> ::= -<Primary> | <Primary>");
    }

    if (Match("-", token_iter, true)) {
        Primary();
    } else if (token_iter->type == "Identifier") {
        Primary();
    } else if (token_iter->lexeme == "integer") {
        Primary();
    } else if (token_iter->lexeme == "(") {
        Primary();
    } else if (token_iter->type == "Real") {
        Primary();
    } else if (token_iter->lexeme == "true") {
        Primary();
    } else if (token_iter->lexeme == "false") {
        Primary();
    }
    
}
void Primary() {

    if (output_rule) {
        lexer_obj.PrintAndWriteToken("<Primary> ::= <Identifier> | <Integer> | <Identifier>(<IDs>) | (<Expression>) | <Real> | true | false");
    }

    if (Match("Identifier", token_iter, true)) {
        lexer_obj.PrintAndWriteToken("<Primary> ::= <Identifier>");
        if (Match("(", token_iter, true)) {   
            lexer_obj.PrintAndWriteToken("<Primary> ::= <Identifier>(<IDs>)");
            Idents();
            Match(")", token_iter, true);
        }
    } else if (Match("integer", token_iter, true)) {
        lexer_obj.PrintAndWriteToken("<Primary> ::= <Integer>");
    } else if (Match("(", token_iter, true)) {
        lexer_obj.PrintAndWriteToken("<Primary> ::= (<Expression>)");
        Expression();
        Match(")", token_iter, true);

    } else if (Match("Real", token_iter, true)) {
        lexer_obj.PrintAndWriteToken("<Primary> ::= <Real>");
    } else if (Match("true", token_iter, true)) {
        lexer_obj.PrintAndWriteToken("<Primary> ::= true");
    } else if (Match("false", token_iter, true)) {
        lexer_obj.PrintAndWriteToken("<Primary> ::= false");
    } else {
        std::cout << "ERROR, line " << token_iter->line_number << ": Expected <Identifier> | <Integer> | <Identifier>(<IDs>) | (<Expression>) | <Real> | true | false ... Terminating" << std::endl;
        exit(1);
    }
}


int main() {

    Rat24F();
    
    return 0;
}
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
    std::string productRules;
};




// Function for reading file and outputs a list of every word within the file separated by whitespace

std::vector<std::string> read_file(const std::string& input_file_name) {
    std::ifstream text(input_file_name);
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

    for (const char& chr : word) {
        if (isalpha(chr)) {
            selected_char = 0;
        }
        else if (isdigit(chr)) {
            selected_char = 1;
        }
        else if (chr == '$') {
            selected_char = 2;
        }
        else {
            word.pop_back();
            return false;
        }
        switch (selected_char) {
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
    for (const char& chr : real) {
        if (chr == '0') {
            selected_char = 0;
        }
        else if (isdigit(chr)) {
            selected_char = 1;
        }
        else if (chr == '.') {
            selected_char = 2;
        }
        else if (chr == '$') {
            selected_char = 3;
        }
        else {
            real.pop_back();
            return false;
        }
        switch (selected_char) {
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
        }
        else if (isdigit(chr)) {
            selected_char = 1;
        }
        else if (chr == '$') {
            selected_char = 2;
        }
        else {
            integer.pop_back();
            return false;
        }
        switch (selected_char) {
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
// Product Rule Concatination
void retStrng_OptFunctionDefinitions(std::string& input)
{
    input = input + "<Opt Function Definitions>";
}
void retStrng_OptDeclarationList(std::string& input)
{
    input = input + "<Opt Declaration List>";
}
void retStrng_FunctionDefinitions(std::string& input)
{
    input = input + "<Function Definitions>";
}
void retStrng_Empty(std::string& input)
{
    input = input + "<Empty>";
}
void retStrng_Function(std::string& input)
{
    input = input + "<Function>";
}
void retStrng_FunctionFunctionDefinitions(std::string& input)
{
    input = input + "<Function> <Function Definitions>";
}
void retStrng_functionIdentifierOptParameterListBody(std::string& input)
{
    input = input + "function <Identifier> (<Opt Parameter List>) < Body >";
}
void retStrng_OptParameterList(std::string& input)
{
    input = input + "<Opt Parameter List>";
}
void retStrng_ParameterList(std::string& input)
{
    input = input + "<Parameter List>";
}
void retStrng_Parameter(std::string& input)
{
    input = input + "<Parameter>";
}
void retStrng_ParameterParameterList(std::string& input)
{
    input = input + "<Parameter> , <Parameter List>";
}
void retStrng_IDsQualifier(std::string& input)
{
    input = input + "<IDs > <Qualifier>";
}
void retStrng_Qualifier(std::string& input)
{
    input = input + "<Qualifier>";
}
void retStrng_integer(std::string& input)
{
    input = input + "integer ";
}
void retStrng_boolean(std::string& input)
{
    input = input + "boolean";
}
void retStrng_real(std::string& input)
{
    input = input + "real";
}
void retStrng_Body(std::string& input)
{
    input = input + "<Body>";
}
void retStrng_StatementList(std::string& input)
{
    input = input + "<Statement List>";
}
void retStrng_DeclarationList(std::string& input)
{
    input = input + "<Declaration List>";
}
void retStrng_Declaration(std::string& input)
{
    input = input + "<Declaration>";
}
void retStrng_DeclarationDeclarationList(std::string& input)
{
    input = input + "<Declaration> ; <Declaration List>";
}
void retStrng_QualifierIDs(std::string& input)
{
    input = input + "<Qualifier > <IDs>";
}
void retStrng_IDs(std::string& input)
{
    input = input + "<IDs>";
}
void retStrng_Identifier(std::string& input)
{
    input = input + "<Identifier>";
}
void retStrng_IdentifierIDs(std::string& input)
{
    input = input + "<Identifier>, <IDs>";
}
void retStrng_Statement(std::string& input)
{
    input = input + "<Statement>";
}
void retStrng_StatementStatementList(std::string& input)
{
    input = input + "<Statement> <Statement List>";
}
void retStrng_Compound(std::string& input)
{
    input = input + "<Compound>";
}
void retStrng_Assign(std::string& input)
{
    input = input + "<Assign>";
}
void retStrng_If(std::string& input)
{
    input = input + "<If>";
}
void retStrng_While(std::string& input)
{
    input = input + " <While>";
}
void retStrng_IdentifierEqualsExpression(std::string& input)
{
    input = input + "<Identifier> = <Expression>";
}
void retStrng_ifConditionStatementfi(std::string& input)
{
    input = input + "if ( <Condition> ) <Statement> fi";
}
void retStrng_ifConditionStatementelseStatementFi(std::string& input)
{
    input = input + "if ( <Condition> ) <Statement> else <Statement> fi";
}
void retStrng_Return(std::string& input)
{
    input = input + "<Return>";
}
void retStrng_ReturnExpression(std::string& input)
{
    input = input + "return <Expression>";
}
void retStrng_Print(std::string& input)
{
    input = input + "<Print>";
}
void retStrng_PutExpression(std::string& input)
{
    input = input + "put ( <Expression>)";
}
void retStrng_Scan(std::string& input)
{
    input = input + "<Scan>";
}
void retStrng_GetIDs(std::string& input)
{
    input = input + "get ( <IDs> )";
}
void retStrng_WhileConditionStatement(std::string& input)
{
    input = input + "while ( <Condition> ) <Statement>";
}
void retStrng_Condition(std::string& input)
{
    input = input + "<Condition>";
}
void retStrng_ExpressionRelopExpression(std::string& input)
{
    input = input + "<Expression> <Relop> < Expression >";
}
void retStrng_Relop(std::string& input)
{
    input = input + "<Relop>";
}
void retStrng_doesEq(std::string& input)
{
    input = input + "==";
}
void retStrng_doesNotEq(std::string& input)
{
    input = input + "!=";
}
void retStrng_POINTNEXT(std::string& input)
{
    input = input + "->";
}
void retStrng_greaterThan(std::string& input)
{
    input = input + ">";
}
void retStrng_lessThan(std::string& input)
{
    input = input + "<";
}
void retStrng_lessThanOrEqual(std::string& input)
{
    input = input + "=<";
}
void retStrng_greaterThanOrEqual(std::string& input)
{
    input = input + "=>";
}
void retStrng_Expression(std::string& input)
{
    input = input + "<Expression>";
}
void retStrng_ExpressionPlusTerm(std::string& input)
{
    input = input + "<Expression> + <Term>";
}
void retStrng_ExpressionMinusTerm(std::string& input)
{
    input = input + "<Expression> - <Term>";
}
void retStrng_Term(std::string& input)
{
    input = input + "<Term>";
}
void retStrng_TermMultFactor(std::string& input)
{
    input = input + "<Term> * <Factor>";
}
void retStrng_TermDivFactor(std::string& input)
{
    input = input + "<Term> / <Factor>";
}
void retStrng_Factor(std::string& input)
{
    input = input + "<Factor>";
}
void retStrng_Primary(std::string& input)
{
    input = input + "<Primary>";
}
void retStrng_True(std::string& input)
{
    input = input + "true";
}
void retStrng_False(std::string& input)
{
    input = input + "false";
}
void retStrng_NEXTLINE(std::string& input)
{
    input = input + "\n";
}


// Implementation of product Rules.

void productRuleParser(std::vector<token>& tokens) 
{
    for(int i =0;i<tokens.size();i++)
    {//now write everything to console and file once vector is complete

        if (tokens[i].type == "Identifier")
        {
            bool used = false;
            if ( i != (tokens.size()-1) )
            {
                if (tokens[i + 1].lexeme == "=")
                {
                    retStrng_StatementList(tokens[i].productRules);
                    retStrng_POINTNEXT(tokens[i].productRules);
                    retStrng_Statement(tokens[i].productRules);
                    retStrng_NEXTLINE(tokens[i].productRules);
                    retStrng_Assign(tokens[i].productRules);
                    retStrng_POINTNEXT(tokens[i].productRules);
                    retStrng_IdentifierEqualsExpression(tokens[i].productRules);
                    used = true;
                }
            }
            else if(used==false)
            {
                
            }
            

        }
        else if (tokens[i].type == "operator")
        {

        }
        else if (tokens[i].type == "Keyword")
        {

        }
        else if (tokens[i].type == "seperator")
        {

        }


    }


}

// Implementation of core Lexer function, takes input of a single expression.



std::vector<token> lexer(std::vector<token>& token_list, std::string& expression) {
    std::string buffer;
    token new_token;
    std::string chr;
    

   // new_token.productRules = productRuleParser();
    for (unsigned int iter = 0; iter < expression.size(); iter++) {
        chr = expression[iter];
        buffer += chr;
        if (expression.substr(iter, 2) == "[*") {
            comment_track = true;

        }
        else if (expression.substr(iter, 2) == "*]") {
            comment_track = false;

            buffer.clear();
            iter += 1;

        }
        else if (separators.count(chr) > 0 && !comment_track) {
            new_token.type = "seperator";
            new_token.lexeme = chr;

            token_list.push_back(new_token);

            buffer.pop_back();

        }
        else if ((operators.count(chr) > 0 && !comment_track) || (operators.count(expression.substr(iter, 2)) > 0 && !comment_track)) {

            new_token.type = "operator";
           // std::cout << "This is the entire operator: " << expression.substr(iter, 2) << std::endl;
            if (expression.length() > 1) {
                buffer = expression.substr(iter, 2);
                new_token.lexeme = buffer;

                token_list.push_back(new_token);

                buffer.clear();

                iter += 1;

            }
            else {
                new_token.lexeme = chr;

                token_list.push_back(new_token);

                buffer.clear();
            }
        }
    }
    if (!comment_track && keywords.count(buffer) > 0) {
        new_token.lexeme = buffer;
        new_token.type = "Keyword";

        token_list.push_back(new_token);

    }
    else if (!comment_track && is_identifier(buffer)) {
        new_token.lexeme = buffer;
        new_token.type = "Identifier";

        token_list.push_back(new_token);

    }
    else if (!comment_track && is_integer(buffer)) {
        new_token.lexeme = buffer;
        new_token.type = "Integer";

        token_list.push_back(new_token);

    }
    else if (!comment_track && is_real(buffer)) {
        new_token.lexeme = buffer;
        new_token.type = "Real";

        token_list.push_back(new_token);

    }
    else if (!comment_track && !buffer.empty()) {
        new_token.lexeme = buffer;
        new_token.type = "Error";

        token_list.push_back(new_token);

    }
    return token_list;
}

int main(int argc, char* argv[]) {
    std::string file_input=argv[1];//if ran as./lexer, type ./lexer input.txt output.txt, if coding go to project>properties>debugging>command line
    std::string file_output=argv[2];

  /*  std::cout << "Please enter your file name within this directory (please include extension): ";
    std::cin >> file_input;

    std::cout << "Name your output file for your token outputs: ";
    std::cin >> file_output;*/

    std::ifstream input(file_input);
    std::ofstream output(file_output);

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
        while (stream >> expression) {//catalogs all lexer iterations into vector list given current line
            token_list = lexer(token_list,expression);
        }
        productRuleParser(token_list);

        for (token token : token_list) {//now write everything to console and file once vector is complete

            std::cout << std::left
                << "Token Type: " << std::setw(15) << token.type
                << "Lexeme: " << token.lexeme << "\n" << token.productRules << "\n" << std::endl;
            output << std::setw(20) << token.type << std::setw(20) << token.lexeme << "\n" << token.productRules << "\n" << std::endl;
        }
    }

    std::cout << "Thank you for using our program, your token outputs have been saved to " << file_output << std::endl;

    return 0;
}
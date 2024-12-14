#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "lexer.h"



 Lexer lexer_obj;
std::vector<token>::const_iterator token_iter = lexer_obj.token_list.cbegin();

 
 struct DeclarationRecord {
    std::string keyLexeme;
    int memSlot;
    std::string varKind;  
};

 static std::vector<DeclarationRecord> recordTable;

 static int recordCounter = 9000;

 bool SymbolExists(const std::string& candidate) {
    for (auto& rec : recordTable) {
        if (rec.keyLexeme == candidate) {
            return true;
        }
    }
    return false;
}

 DeclarationRecord& FindRecord(const std::string& candidate) {
    for (auto& rec : recordTable) {
        if (rec.keyLexeme == candidate) {
            return rec;
        }
    }
    static DeclarationRecord placeholder = { "", -1, "" };
    return placeholder;
}

 void RegisterSymbol(const std::string& candidate, const std::string& typeTag) {
    if (SymbolExists(candidate)) {
        std::cout << "SEMANTIC ERROR: Identifier '" << candidate << "' already declared.\n";
        exit(1);
    }
    DeclarationRecord fresh;
    fresh.keyLexeme = candidate;
    fresh.memSlot = recordCounter++;
    fresh.varKind = typeTag;
    recordTable.push_back(fresh);
}

 void EnsureSymbolExists(const std::string& candidate) {
    if (!SymbolExists(candidate)) {
        std::cout << "SEMANTIC ERROR: Identifier '" << candidate << "' not declared.\n";
        exit(1);
    }
}

 static std::vector<std::string> vm_instructions(1000);
static int asm_index = 1;

 void EmitVMInstr(const std::string& opCode, const std::string& operand = "") {
    std::ostringstream morph;
    morph << asm_index++ << " " << opCode;
    if (!operand.empty()) {
        morph << " " << operand;
    }
    vm_instructions[asm_index - 2] = morph.str();
}

 void DumpAssemblyAndRecords() {
     std::cout << "\nGenerated Assembly Code:\n";
    for (int i = 0; i < asm_index - 1; i++) {
        std::cout << vm_instructions[i] << "\n";
    }

    std::cout << "\nSymbol Table\n";
    std::cout << "Identifier    MemoryLocation    Type\n";
    for (auto& rec : recordTable) {
        std::cout << rec.keyLexeme << "           "
            << rec.memSlot << "               "
            << rec.varKind << "\n";
    }

     std::ofstream outFile(lexer_obj.output_filename, std::ios::app);
    if (outFile.is_open()) {
        outFile << "\nGenerated Assembly Code:\n";
        for (int i = 0; i < asm_index - 1; i++) {
            outFile << vm_instructions[i] << "\n";
        }
        outFile << "\nSymbol Table\n";
        outFile << "Identifier    MemoryLocation    Type\n";
        for (auto& rec : recordTable) {
            outFile << rec.keyLexeme << "           "
                << rec.memSlot << "               "
                << rec.varKind << "\n";
        }
        outFile.close();
    }
}

 bool Match(std::string desired, std::vector<token>::const_iterator token_obj, bool error_toggle = true);
void Rat24F();
void OptDeclList();
void DeclList();
void DeclListPrime();
void Declaration();
void IdentsPrime(const std::string& q);
void IdentsPrime();
void StatementsList();
void StatementsListPrime();
void Statement();
void Compound();
void Assign();
void IfCondition();
void IfPrime(int jumpZInstrIndex);
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

 static std::string expr_type = "";

 bool Match(std::string desired_string, std::vector<token>::const_iterator token_obj, bool error_toggle) {
    if (desired_string == token_obj->lexeme || desired_string == token_obj->type) {
        token_iter++;
        return true;
    }
    if (error_toggle) {
        std::cout << "ERROR, line " << token_iter->line_number
            << ": Expected '" << desired_string
            << "', but got " << token_obj->lexeme
            << " (type: " << token_obj->type << ") ... Terminating.\n";
        exit(1);
    }
    return false;
}

 void Rat24F() {
    Match("@", token_iter, true);
    OptDeclList();
    StatementsList();
    Match("@", token_iter, true);

     DumpAssemblyAndRecords();
}

void OptDeclList() {
    if (token_iter->lexeme == "integer" || token_iter->lexeme == "boolean") {
        DeclList();
    }
}

void DeclList() {
    Declaration();
    Match(";", token_iter, true);
    DeclListPrime();
}

void DeclListPrime() {
    if (token_iter->lexeme == "integer" || token_iter->lexeme == "boolean") {
        DeclList();
    }
}

void Declaration() {
    std::string q;
    if (token_iter->lexeme == "integer") {
        q = "integer";
        token_iter++;
    }
    else if (token_iter->lexeme == "boolean") {
        q = "boolean";
        token_iter++;
    }
    else {
        std::cout << "SEMANTIC ERROR: Expected qualifier integer|boolean.\n";
        exit(1);
    }
    // first ID
    if (token_iter->type == "Identifier") {
        RegisterSymbol(token_iter->lexeme, q);   
        token_iter++;
    }
    else {
        std::cout << "SEMANTIC ERROR: Expected <Identifier> after qualifier.\n";
        exit(1);
    }
    IdentsPrime(q);
}

void IdentsPrime(const std::string& q) {
    if (Match(",", token_iter, false)) {
        if (token_iter->type == "Identifier") {
            RegisterSymbol(token_iter->lexeme, q);
            token_iter++;
        }
        else {
            std::cout << "SEMANTIC ERROR: Expected <Identifier> after comma.\n";
            exit(1);
        }
        IdentsPrime(q);
    }
}

void IdentsPrime() { /* no op overload */ }

void StatementsList() {
    Statement();
    StatementsListPrime();
}

void StatementsListPrime() {
    if (token_iter->lexeme != "}" && token_iter->lexeme != "@") {
        StatementsList();
    }
}

void Statement() {
    if (token_iter->lexeme == "{") {
        Compound();
    }
    else if (token_iter->type == "Identifier") {
        Assign();
    }
    else if (token_iter->lexeme == "if") {
        IfCondition();
    }
    else if (token_iter->lexeme == "return") {
        ReturnStuff();
    }
    else if (token_iter->lexeme == "put") {
        PrintStuff();
    }
    else if (token_iter->lexeme == "get") {
        ScanStuff();
    }
    else if (token_iter->lexeme == "while") {
        WhileCondition();
    }
    else {
        std::cout << "ERROR, line " << token_iter->line_number
            << ": Unexpected token for <Statement>. Terminating.\n";
        exit(1);
    }
}

void Compound() {
    Match("{", token_iter, true);
    StatementsList();
    Match("}", token_iter, true);
}

void Assign() {
    std::string lhs_lex = token_iter->lexeme;
    EnsureSymbolExists(lhs_lex); 
    token_iter++;

    Match("=", token_iter, true);

    expr_type.clear();
    Expression();

    DeclarationRecord& lhsRec = FindRecord(lhs_lex);  
    if (lhsRec.varKind != expr_type) {
        std::cout << "SEMANTIC ERROR: Type mismatch in assignment. LHS="
            << lhsRec.varKind << ", RHS=" << expr_type << "\n";
        exit(1);
    }
    EmitVMInstr("POPM", std::to_string(lhsRec.memSlot));

    Match(";", token_iter, true);
}

void IfCondition() {
    Match("if", token_iter, true);
    Match("(", token_iter, true);

    expr_type.clear();
    Condition();
    if (expr_type != "boolean") {
        std::cout << "SEMANTIC ERROR: Condition in if-statement not boolean.\n";
        exit(1);
    }
    Match(")", token_iter, true);

    int jzIndex = asm_index;
    EmitVMInstr("JUMPZ");

    Statement();

    IfPrime(jzIndex);
}

void IfPrime(int jumpZInstrIndex) {
    if (Match("else", token_iter, false)) {
        int jInstrIdx = asm_index;
        EmitVMInstr("JUMP");
        vm_instructions[jumpZInstrIndex - 1] += " " + std::to_string(asm_index);

        Statement();
        Match("fi", token_iter, true);
        vm_instructions[jInstrIdx - 1] += " " + std::to_string(asm_index);
    }
    else if (Match("fi", token_iter, false)) {
        vm_instructions[jumpZInstrIndex - 1] += " " + std::to_string(asm_index);
    }
    else {
        std::cout << "ERROR, line " << token_iter->line_number << ": Expected 'fi'. Terminating.\n";
        exit(1);
    }
}

void ReturnStuff() {
    Match("return", token_iter, true);
    expr_type.clear();
    ReturnPrime();
    Match(";", token_iter, true);
}

void ReturnPrime() {
    if (token_iter->type == "Identifier" || token_iter->type == "integer" ||
        token_iter->lexeme == "(" || token_iter->lexeme == "true" || token_iter->lexeme == "false") {
        Expression();
    }
}

void PrintStuff() {
    Match("put", token_iter, true);
    Match("(", token_iter, true);

    expr_type.clear();
    Expression();
    EmitVMInstr("STDOUT");

    Match(")", token_iter, true);
    Match(";", token_iter, true);
}

void ScanStuff() {
    Match("get", token_iter, true);
    Match("(", token_iter, true);

    if (token_iter->type == "Identifier") {
        EnsureSymbolExists(token_iter->lexeme);
        EmitVMInstr("STDIN");
        DeclarationRecord& rec = FindRecord(token_iter->lexeme);
        EmitVMInstr("POPM", std::to_string(rec.memSlot));
        token_iter++;
    }
    else {
        std::cout << "SEMANTIC ERROR: Expected <Identifier> after 'get('.\n";
        exit(1);
    }
    while (Match(",", token_iter, false)) {
        if (token_iter->type == "Identifier") {
            EnsureSymbolExists(token_iter->lexeme);
            EmitVMInstr("STDIN");
            DeclarationRecord& r2 = FindRecord(token_iter->lexeme);
            EmitVMInstr("POPM", std::to_string(r2.memSlot));
            token_iter++;
        }
        else {
            std::cout << "SEMANTIC ERROR: Expected <Identifier> after comma in get(...).\n";
            exit(1);
        }
    }

    Match(")", token_iter, true);
    Match(";", token_iter, true);
}

void WhileCondition() {
    Match("while", token_iter, true);
    Match("(", token_iter, true);

    int loopLabel = asm_index;
    EmitVMInstr("LABEL");

    expr_type.clear();
    Condition();
    if (expr_type != "boolean") {
        std::cout << "SEMANTIC ERROR: Condition in while-statement not boolean.\n";
        exit(1);
    }
    Match(")", token_iter, true);

    int jzIndex = asm_index;
    EmitVMInstr("JUMPZ");

    Statement();

    EmitVMInstr("JUMP", std::to_string(loopLabel));
    vm_instructions[jzIndex - 1] += " " + std::to_string(asm_index);
}

void Condition() {
    Expression();
    std::string leftType = expr_type;
    Relop();
    Expression();
    if (leftType != expr_type) {
        std::cout << "SEMANTIC ERROR: Type mismatch in condition.\n";
        exit(1);
    }
    expr_type = "boolean";
}

void Relop() {
    std::string lex = token_iter->lexeme;
    if (lex == "==") { EmitVMInstr("EQU"); token_iter++; }
    else if (lex == "!=") { EmitVMInstr("NEQ"); token_iter++; }
    else if (lex == ">") { EmitVMInstr("GRT"); token_iter++; }
    else if (lex == "<") { EmitVMInstr("LES"); token_iter++; }
    else if (lex == "<=") { EmitVMInstr("LEQ"); token_iter++; }
    else if (lex == "=>") { EmitVMInstr("GEQ"); token_iter++; }
    else {
        std::cout << "ERROR, line " << token_iter->line_number << ": Expected relational operator.\n";
        exit(1);
    }
}

void Expression() {
    Term();
    ExpressionPrime();
}

void ExpressionPrime() {
    if (Match("+", token_iter, false)) {
        if (expr_type != "integer") {
            std::cout << "SEMANTIC ERROR: '+' used with non-integer.\n";
            exit(1);
        }
        Term();
        if (expr_type != "integer") {
            std::cout << "SEMANTIC ERROR: '+' used with non-integer.\n";
            exit(1);
        }
        EmitVMInstr("ADD");
        ExpressionPrime();
    }
    else if (Match("-", token_iter, false)) {
        if (expr_type != "integer") {
            std::cout << "SEMANTIC ERROR: '-' used with non-integer.\n";
            exit(1);
        }
        Term();
        if (expr_type != "integer") {
            std::cout << "SEMANTIC ERROR: '-' used with non-integer.\n";
            exit(1);
        }
        EmitVMInstr("SUB");
        ExpressionPrime();
    }
}

void Term() {
    Factor();
    TermPrime();
}

void TermPrime() {
    if (Match("*", token_iter, false)) {
        if (expr_type != "integer") {
            std::cout << "SEMANTIC ERROR: '*' used with non-integer.\n";
            exit(1);
        }
        Factor();
        if (expr_type != "integer") {
            std::cout << "SEMANTIC ERROR: '*' used with non-integer.\n";
            exit(1);
        }
        EmitVMInstr("MUL");
        TermPrime();
    }
    else if (Match("/", token_iter, false)) {
        if (expr_type != "integer") {
            std::cout << "SEMANTIC ERROR: '/' used with non-integer.\n";
            exit(1);
        }
        Factor();
        if (expr_type != "integer") {
            std::cout << "SEMANTIC ERROR: '/' used with non-integer.\n";
            exit(1);
        }
        EmitVMInstr("DIV");
        TermPrime();
    }
}

void Factor() {
    if (Match("-", token_iter, false)) {
        Primary();
        if (expr_type == "boolean") {
            std::cout << "SEMANTIC ERROR: unary '-' used on boolean.\n";
            exit(1);
        }
        EmitVMInstr("PUSHI", "-1");
        EmitVMInstr("MUL");
    }
    else {
        Primary();
    }
}

void Primary() {
    if (Match("Identifier", token_iter, false)) {
        std::string recLex = (token_iter - 1)->lexeme;
        EnsureSymbolExists(recLex);
        DeclarationRecord& symbolRec = FindRecord(recLex);
        expr_type = symbolRec.varKind;
        EmitVMInstr("PUSHM", std::to_string(symbolRec.memSlot));
    }
    else if (Match("integer", token_iter, false)) {
        std::string literalVal = (token_iter - 1)->lexeme;
        expr_type = "integer";
        EmitVMInstr("PUSHI", literalVal);
    }
    else if (Match("(", token_iter, false)) {
        Expression();
        Match(")", token_iter, true);
    }
    else if (Match("true", token_iter, false)) {
        expr_type = "boolean";
        EmitVMInstr("PUSHI", "1");
    }
    else if (Match("false", token_iter, false)) {
        expr_type = "boolean";
        EmitVMInstr("PUSHI", "0");
    }
    else {
        std::cout << "ERROR, line " << token_iter->line_number
            << ": Expected <Identifier> | <Integer> | '(' | true | false ... Terminating.\n";
        exit(1);
    }
}

 int main() {
    Rat24F();
    return 0;
}

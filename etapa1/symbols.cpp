// UFRGS - Marthyna Weber - 2025-1
//
// symbols.cpp - tabela de símbolos
// Código parcialmente desenvolvido em sala de aula pelo professor.

#include "symbols.hpp"
#include "tokens.h"
#include <iostream>
#include <map>

extern int yylex();

int lineNumber = 1;
int running = 1;

using namespace std;

map<string, SYMBOL *> SymbolTable;

const map<int, string> tokenNames = {
    {KW_BYTE, "KW_BYTE"},
    {KW_INT, "KW_INT"},
    {KW_REAL, "KW_REAL"},
    {KW_IF, "KW_IF"},
    {KW_ELSE, "KW_ELSE"},
    {KW_DO, "KW_DO"},
    {KW_WHILE, "KW_WHILE"},
    {KW_READ, "KW_READ"},
    {KW_PRINT, "KW_PRINT"},
    {KW_RETURN, "KW_RETURN"},
    {OPERATOR_LE, "OPERATOR_LE"},
    {OPERATOR_GE, "OPERATOR_GE"},
    {OPERATOR_EQ, "OPERATOR_EQ"},
    {OPERATOR_DIF, "OPERATOR_DIF"},
    {TK_IDENTIFIER, "TK_IDENTIFIER"},
    {LIT_INT, "LIT_INT"},
    {LIT_CHAR, "LIT_CHAR"},
    {LIT_REAL, "LIT_REAL"},
    {LIT_STRING, "LIT_STRING"},
};

SYMBOL *symbolInsert(int type, char *text)
{
    string key(text);
    if (SymbolTable.find(key) != SymbolTable.end())
        return SymbolTable.find(key)->second;

    SYMBOL *newSymbol = new SYMBOL{type, key};
    SymbolTable[key] = newSymbol;
    return newSymbol;
}

SYMBOL *symbolFind(int token)
{
    string key = std::to_string(token);
    auto it = SymbolTable.find(key);
    if (it != SymbolTable.end())
    {
        return it->second;
    }
    return nullptr;
}

void printIntegerLiteral(const string &literal)
{
    for (auto it = literal.rbegin(); it != literal.rend(); ++it)
    {
        cout << *it;
    }
}

void symbolPrintTable(void)
{
    for (auto s : SymbolTable)
    {
        if (s.second->type == LIT_INT)
        {
            cout << "Symbol:[LIT_INT, ";
            printIntegerLiteral(s.second->text);
            cout << "]" << endl;
        }
        else
        {
            cout << "Symbol:[" << tokenNames.at(s.second->type) << ", " << s.second->text << "]" << endl;
        }
    }
}

int getLineNumber(void)
{
    return lineNumber;
}

int isRunning(void)
{
    return running;
}

void initMe(void)
{
    lineNumber = 1;
    running = 1;
    SymbolTable.clear();
}

void processTokens(FILE *file)
{
    while (isRunning())
    {
        int tok = yylex();
        if (!isRunning())
            break;

        auto it = tokenNames.find(tok);
        if (it != tokenNames.end())
        {
            printf("TOKEN: %s\n", it->second.c_str());
        }
        else
        {
            printf("TOKEN: %d\n", tok);
        }
    }
}

// END OF FILE
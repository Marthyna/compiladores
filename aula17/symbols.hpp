// UFRGS - Marthyna Weber - 2025-1
//
// symbols.hpp - tabela de símbolos
// Código parcialmente desenvolvido em sala de aula pelo professor Marcelo Johann.

#ifndef SYMBOLS_HPP
#define SYMBOLS_HPP
#include <string>
#include <map>
#include <cstdio>
#include "ast.hpp"

using namespace std;

enum SymbolTypes
{
    SYMBOL_LIT_INT,
    SYMBOL_LIT_CHAR,
    SYMBOL_LIT_REAL,
    SYMBOL_LIT_STRING,
    SYMBOL_IDENTIFIER,
    SYMBOL_VARDEC,
    SYMBOL_FUNDEC,
    SYMBOL_VECTORDEC,
};

enum SymbolDataTypes
{
    SYMBOL_DATA_TYPE_INT,
    SYMBOL_DATA_TYPE_CHAR,
    SYMBOL_DATA_TYPE_REAL,
    SYMBOL_DATA_TYPE_STRING,
    SYMBOL_DATA_TYPE_BOOL,
    SYMBOL_DATA_TYPE_INVALID,
};

struct SYMBOL
{
    int type;
    std::string text;
    int dataType;
    struct AST *declarationAST = nullptr;

public:
    SYMBOL(int type, string text) : type(type), text(text) {};
};

extern const map<int, string> tokenNames;
extern std::string SymbolDataTypeName[];

SYMBOL *symbolFind(int token);
SYMBOL *symbolInsert(int type, char *text);
void printIntegerLiteral(const string &literal);
void symbolPrintTable(void);
void processTokens(FILE *file);
int getLineNumber(void);
int isRunning(void);
void initMe(void);
void processTokens(FILE *file);
SYMBOL *makeTemp(void);

#endif // SYMBOLS_HPP
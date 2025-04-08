// UFRGS - Marthyna Weber - 2025-1
//
// symbols.hpp - tabela de símbolos
// Código parcialmente desenvolvido em sala de aula pelo professor.

#ifndef SYMBOLS_HPP
#define SYMBOLS_HPP
#include <string>
#include <map>
#include <cstdio>

using namespace std;

struct SYMBOL
{
    int type;
    std::string text;

public:
    SYMBOL(int type, string text) : type(type), text(text) {};
};

extern const map<int, string> tokenNames;
SYMBOL *symbolFind(int token);
SYMBOL *symbolInsert(int type, char *text);
void printIntegerLiteral(const string &literal);
void symbolPrintTable(void);
void processTokens(FILE *file);
int getLineNumber(void);
int isRunning(void);
void initMe(void);
void processTokens(FILE *file);

#endif // SYMBOLS_HPP
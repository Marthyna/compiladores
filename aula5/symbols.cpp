// UFRGS - Marthyna Weber - 2025-1
//
// symbols.cpp - tabela de símbolos
// Código parcialmente desenvolvido em sala de aula pelo professor.

#include "symbols.hpp"
#include <iostream>
#include <map>

using namespace std;

map<string, SYMBOL *> SymbolTable;

SYMBOL *symbolInsert(int type, char *text)
{
    string key(text);
    if (SymbolTable.find(key) != SymbolTable.end())
        return SymbolTable.find(key)->second;

    SYMBOL *newSymbol = new SYMBOL{type, key};
    SymbolTable[key] = newSymbol;
    return newSymbol;
}

void symbolPrintTable(void)
{
    for (auto s : SymbolTable)
    {
        printf("Symbol:[%d,%s]\n", s.second->type, s.second->text.c_str());
    }
}

// END OF FILE
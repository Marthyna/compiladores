// ast.hpp
#ifndef AST_H
#define AST_H

#include <vector>
#include "symbols.hpp"

using namespace std;

enum ASTTypes
{
    AST_UNKNOWN,
    AST_SYMBOL,
    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DIV,
    AST_AND,
    AST_OR,
    AST_NOT,
};

typedef struct AST
{
    int type;
    vector<AST *> children;
    SYMBOL *symbol;

public:
    AST(int t, vector<AST *> c, SYMBOL *s = 0) : type(t), children(c), symbol(s) {};
} AST;

// METHODS

void astPrint(AST *node);

#endif
// END OF FILE
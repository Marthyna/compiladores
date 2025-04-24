// UFRGS - Marthyna Weber - 2025-1
//
// ast.hpp - abstract syntax tree
// Código parcialmente desenvolvido em sala de aula pelo professor Marcelo Johann.

#ifndef AST_HPP
#define AST_HPP
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
    AST_ASS,
    AST_CMDL,
    AST_VARDEC,
    AST_FUNDEC,
    AST_LIT,
    AST_DECL,
    AST_TYPEINT,
    AST_TYPEBYTE,
    AST_TYPEREAL
};

struct AST
{
    int type;
    vector<AST *> children;
    SYMBOL *symbol;

public:
    AST(int t, vector<AST *> c, SYMBOL *s = 0) : type(t), children(c), symbol(s) {};
};

void astPrint(AST *node, int depth = 0);

#endif // AST_HPP
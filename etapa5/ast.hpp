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
    AST_LT,
    AST_GT,
    AST_LE,
    AST_GE,
    AST_EQ,
    AST_NE,
    AST_AND,
    AST_OR,
    AST_NOT,
    AST_ASG,
    AST_CMDL,
    AST_VARDEC,
    AST_FUNDEC,
    AST_VECTORDEC,
    AST_VECTORDEC_INIT,
    AST_LIT,
    AST_LIST,
    AST_DECL,
    AST_TYPEINT,
    AST_TYPEBYTE,
    AST_TYPEREAL,
    AST_IF,
    AST_IF_ELSE,
    AST_WHILE,
    AST_DOWHILE,
    AST_RETURN,
    AST_PRINT,
    AST_READ,
    AST_PARAM,
    AST_BLOCK,
    AST_FUNCALL,
    AST_VECTORACCESS,
    AST_ASG_VECTOR,
    AST_EMPTY,
    AST_IFT
};

struct AST
{
    int type;
    vector<AST *> children;
    int lineNumber;
    struct SYMBOL *symbol;

public:
    AST(int t, vector<AST *> c, int l, struct SYMBOL *s = 0) : type(t), children(c), lineNumber(l), symbol(s) {};
    void add(AST *child);
};

void astPrint(AST *node, int depth);

#endif // AST_HPP
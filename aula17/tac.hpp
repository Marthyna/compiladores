// UFRGS - Marthyna Weber - 2025-1
//
// tac.hpp - three address code
// Código parcialmente desenvolvido em sala de aula pelo professor Marcelo Johann.

#ifndef TAC_HPP
#define TAC_HPP
#include <vector>
#include "symbols.hpp"

using namespace std;

enum TACTypes
{
    TAC_UNKNOWN,
    TAC_SYMBOL,
    TAC_ADD,
    TAC_SUB,
    TAC_MUL,
    TAC_DIV,
    TAC_LT,
    TAC_GT,
    TAC_LE,
    TAC_GE,
    TAC_EQ,
    TAC_NE,
    TAC_AND,
    TAC_OR,
    TAC_NOT,
    TAC_ASG,
    TAC_CMDL,
    TAC_VARDEC,
    TAC_FUNDEC,
    TAC_VECTORDEC,
    TAC_VECTORDEC_INIT,
    TAC_LIT,
    TAC_LIST,
    TAC_DECL,
    TAC_TYPEINT,
    TAC_TYPEBYTE,
    TAC_TYPEREAL,
    TAC_IF,
    TAC_IF_ELSE,
    TAC_WHILE,
    TAC_DOWHILE,
    TAC_RETURN,
    TAC_PRINT,
    TAC_READ,
    TAC_PARAM,
    TAC_BLOCK,
    TAC_FUNCALL,
    TAC_VECTORACCESS,
    TAC_ASG_VECTOR,
    TAC_EMPTY,
    TAC_IFT
};

struct TAC
{
    int type;
    struct SYMBOL *res;
    struct SYMBOL *op1;
    struct SYMBOL *op2;
    TAC *next;
    TAC *prev;
    // int lineNumber;
    // struct SYMBOL *symbol;

public:
    TAC(int t, struct SYMBOL *r, struct SYMBOL *o1, struct SYMBOL *o2)
        : type(t), res(r), op1(o1), op2(o2) { next = prev = 0; };
    // void add(TAC *child);
};

// TAC METHODS

void tacPrintSingle(TAC *tac);
void tacPrintBackwards(TAC *tac);
TAC *tacJoin(TAC *l1, TAC *l2);

// CODE GENERATION

TAC *generateCode(AST *ast);

#endif // TAC_HPP
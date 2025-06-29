// UFRGS - Marthyna Weber - 2025-1
//
// tac.cpp - three address code
// Código parcialmente desenvolvido em sala de aula pelo professor Marcelo Johann.

#include "tac.hpp"
#include <stdio.h>

using namespace std;

int MAX_CHILDREN = 4;

string TACTypeNames[] = {
    "TAC_UNKNOWN",
    "TAC_SYMBOL",
    "TAC_ADD",
    "TAC_SUB",
    "TAC_MUL",
    "TAC_DIV",
    "TAC_AND",
    "TAC_OR",
    "TAC_NOT",
    "TAC_ASG",
    "TAC_CMDL",
    "TAC_VARDEC",
    "TAC_FUNDEC",
    "TAC_VECTORDEC",
    "TAC_VECTORDEC_INIT",
    "TAC_LIT",
    "TAC_LIST",
    "TAC_DECL",
    "TAC_TYPEINT",
    "TAC_TYPEBYTE",
    "TAC_TYPEREAL",
    "TAC_IF",
    "TAC_IF_ELSE",
    "TAC_WHILE",
    "TAC_DOWHILE",
    "TAC_RETURN",
    "TAC_PRINT",
    "TAC_READ",
    "TAC_PARAM",
    "TAC_BLOCK",
    "TAC_LT",
    "TAC_GT",
    "TAC_LE",
    "TAC_GE",
    "TAC_EQ",
    "TAC_NE",
    "TAC_FUNCALL",
    "TAC_VECTORACCESS",
    "TAC_ASG_VECTOR",
    "TAC_EMPTY",
    "TAC_IFT"};

void tacPrintSingle(TAC *tac)
{
    if (!tac)
        return;

    fprintf(stderr, "TAC(%s, %s, %s, %s)", TACTypeNames[tac->type].c_str(),
            tac->res ? tac->res->text.c_str() : "NULL",
            tac->op1 ? tac->op1->text.c_str() : "NULL",
            tac->op2 ? tac->op2->text.c_str() : "NULL");
}

void tacPrintBackwards(TAC *tac)
{
    if (!tac)
        return;

    tacPrintBackwards(tac->next);
    tacPrintSingle(tac);
}

TAC *tacJoin(TAC *l1, TAC *l2)
{
    TAC *t;
    if (!l1)
        return l2;
    if (!l2)
        return l1;

    for (t = l2; t->prev; t = t->prev)
        ;

    t->prev = l1;
    return l2;
}

TAC *generateCode(AST *node)
{
    int i = 0;
    TAC *result = 0;
    TAC *code[MAX_CHILDREN] = {0, 0, 0, 0};
    if (!node)
        return nullptr;

    for (auto child : node->children)
    {
        code[i++] = generateCode(child);
    }

    switch (node->type)
    {
    case AST_SYMBOL:
        result = new TAC(TAC_SYMBOL, node->symbol, 0, 0);
        break;

    case AST_ADD:
        result = tacJoin(
            tacJoin(
                code[0], code[1]),
            new TAC(
                TAC_ADD, makeTemp(),
                code[0] != 0 ? code[0]->res : 0,
                code[1] != 0 ? code[1]->res : 0));
        break;
    default:
        result = tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]), code[3]);
        break;
    }

    return result;
}
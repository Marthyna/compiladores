// ast.cpp
#include <stdio.h>
#include "ast.hpp"

string ASTTypeNames[] =
    {
        "AST_UNKNOWN",
        "AST_SYMBOL",
        "AST_ADD",
        "AST_SUB",
        "AST_MUL",
        "AST_DIV",
        "AST_AND",
        "AST_OR",
        "AST_NOT",
};

// METHODS
void astPrint(AST *node, int depth = 0)
{
    if (!node)
        return;

    for (int i = 0; i < depth; i++)
    {
        fprintf(stderr, "  ");
    }

    fprintf(stderr, "AST: %s\n", ASTTypeNames[node->type].c_str());

    for (auto s : node->children)
    {
        if (s->symbol)
        {
            fprintf(stderr, "  %s\n", s->symbol->text.c_str());
        }
        else
        {
            astPrint(s, depth + 1);
        }
    }
}
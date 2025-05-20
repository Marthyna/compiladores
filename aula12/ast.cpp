// UFRGS - Marthyna Weber - 2025-1
//
// ast.hpp - abstract syntax tree
// Código parcialmente desenvolvido em sala de aula pelo professor Marcelo Johann.

#include "ast.hpp"
#include <stdio.h>

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
        "AST_ASS",
        "AST_CMDL",
        "AST_VARDEC",
        "AST_FUNDEC",
        "AST_LIT",
        "AST_DECL",
        "AST_TYPEINT",
        "AST_TYPEBYTE",
        "AST_TYPEREAL"};

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
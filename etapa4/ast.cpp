// UFRGS - Marthyna Weber - 2025-1
//
// ast.hpp - abstract syntax tree
// Código parcialmente desenvolvido em sala de aula pelo professor Marcelo Johann.

#include "ast.hpp"
#include <stdio.h>

string ASTTypeNames[] = {
    "AST_UNKNOWN",
    "AST_SYMBOL",
    "AST_ADD",
    "AST_SUB",
    "AST_MUL",
    "AST_DIV",
    "AST_AND",
    "AST_OR",
    "AST_NOT",
    "AST_ASG",
    "AST_CMDL",
    "AST_VARDEC",
    "AST_FUNDEC",
    "AST_VECTORDEC",
    "AST_VECTORDEC_INIT",
    "AST_LIT",
    "AST_LIST",
    "AST_DECL",
    "AST_TYPEINT",
    "AST_TYPEBYTE",
    "AST_TYPEREAL",
    "AST_IF",
    "AST_IF_ELSE",
    "AST_WHILE",
    "AST_DOWHILE",
    "AST_RETURN",
    "AST_PRINT",
    "AST_READ",
    "AST_PARAM",
    "AST_BLOCK",
    "AST_LT",
    "AST_GT",
    "AST_LE",
    "AST_GE",
    "AST_EQ",
    "AST_NE",
    "AST_FUNCALL",
    "AST_VECTORACCESS",
    "AST_ASG_VECTOR",
    "AST_EMPTY"};

// METHODS
void astPrint(AST *node, int depth)
{
    if (!node)
        return;

    for (int i = 0; i < depth; i++)
    {
        fprintf(stderr, "  ");
    }

    if (node->type < static_cast<int>(sizeof(ASTTypeNames) / sizeof(string)))
        fprintf(stderr, "AST: %s", ASTTypeNames[node->type].c_str());
    else
        fprintf(stderr, "AST: UNKNOWN_TYPE");

    if (node->symbol)
        fprintf(stderr, " (%s)", node->symbol->text.c_str());

    fprintf(stderr, "\n");

    for (auto child : node->children)
    {
        astPrint(child, depth + 1);
    }
}

void AST::add(AST *child)
{
    this->children.push_back(child);
}
// semantic.cpp - marthyna 2025

#include "semantic.hpp"

void check_and_set_declarations(AST *node)
{
    if (!node)
        return;

    // do whatever i need here
    switch (node->type)
    {
    case AST_VARDEC or
        AST_FUNDEC:
        if (node->symbol)
            if (node->symbol->type == SYMBOL_IDENTIFIER)
            {
                node->symbol->type = (node->type == AST_VARDEC) ? SYMBOL_VARDEC : SYMBOL_FUNDEC;
                if (node->children[0])
                    if (node->children[0]->type == AST_TYPEINT)
                        node->symbol->dataType = SYMBOL_DATA_TYPE_INT;
                    else if (node->children[0]->type == AST_TYPEBYTE)
                        node->symbol->dataType = SYMBOL_DATA_TYPE_CHAR;
                    else if (node->children[0]->type == AST_TYPEREAL)
                        node->symbol->dataType = SYMBOL_DATA_TYPE_REAL;
                    else
                        node->symbol->dataType = SYMBOL_DATA_TYPE_INVALID;
                else
                {
                    fprintf(stderr, "GRAVEEEE Semantic error: variable x not declared\n");
                    exit(999);
                }
            }
            else
            {
                fprintf(stderr, "Semantic error: variable x redeclared\n");
            }
        else
        {
            fprintf(stderr, "GRAVEEEE Semantic error: variable x not declared\n");
            exit(999);
        }
        break;

    case AST_DECL:
        /* code */
        break;

        // $2->type = SYMBOL_FUNDEC;

    default:
        break;
    }
    // top to bottom, children after parent
    for (auto child : node->children)
    {
        astPrint(child);
    }
}

void check_undeclared(AST *node)
{
}
// checks

// END OF FILE
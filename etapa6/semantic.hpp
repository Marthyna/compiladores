// UFRGS - Marthyna Weber - 2025-1
// semantic.hpp
#include <iostream>
#include <string>
#include "ast.hpp"
#include <set>

const std::set<int> typesAllowedInPrint = {
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
    AST_LIT,
    AST_SYMBOL,
    AST_VECTORACCESS,
    AST_FUNCALL};

enum
{
    NO_SPACE_BETWEEN = 0,
    SPACE_BETWEEN = 1,
    COMMA_BETWEEN = 2
};
extern int semantic_errors;

void astToSourceCode(AST *node, std::ostream &out, int level, int spacing = NO_SPACE_BETWEEN);
void checkAndSetDeclarations(AST *node);
void checkUsage(AST *node);
void checkTypes(AST *node, int expectedFuncReturnType = SYMBOL_DATA_TYPE_INVALID);
int getExprType(AST *node);
std::string reverseString(const std::string &str);
bool isCompatible(int type1, int type2);

std::string indent(int level);

// checks

// END OF FILE
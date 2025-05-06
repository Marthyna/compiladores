// semantic.hpp - marthyna 2025
#include <iostream>
#include <string>
#include "ast.hpp"

enum
{
    NO_SPACE_BETWEEN = 0,
    SPACE_BETWEEN = 1,
    COMMA_BETWEEN = 2
};

void astToSourceCode(AST *node, std::ostream &out, int level, int spacing = NO_SPACE_BETWEEN);
std::string indent(int level);

// checks

// END OF FILE
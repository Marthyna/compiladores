// UFRGS - Marthyna Weber - 2025-1
// semantic.cpp

#include "semantic.hpp"

bool isCompatible(int type1, int type2)
{
    bool compatible = false;
    if (type1 == SYMBOL_DATA_TYPE_REAL)
    {
        if (type2 == SYMBOL_DATA_TYPE_REAL)
            compatible = true;
    }
    else if (type1 == SYMBOL_DATA_TYPE_INT)
    {
        if (type2 == SYMBOL_DATA_TYPE_INT || type2 == SYMBOL_DATA_TYPE_CHAR)
            compatible = true;
    }
    else if (type1 == SYMBOL_DATA_TYPE_CHAR)
    {
        if (type2 == SYMBOL_DATA_TYPE_INT || type2 == SYMBOL_DATA_TYPE_CHAR)
            compatible = true;
    }
    return compatible;
}

std::string indent(int level)
{
    return std::string(level * 4, ' ');
}

void astToSourceCode(AST *node, std::ostream &out, int level, int spacing)
{
    if (!node)
        return;

    switch (node->type)
    {
    case AST_DECL:
    case AST_CMDL:
    case AST_LIST:
        for (AST *child : node->children)
        {
            if (child->symbol && child->symbol->type == SYMBOL_LIT_STRING)
            {
                if (child == node->children.front())
                    out << " ";
                out << '"';
                astToSourceCode(child, out, level);
                out << '"';
                if (child != node->children.back())
                    out << " ";
            }
            else if (spacing == SPACE_BETWEEN)
            {
                if (child == node->children.front())
                    out << " ";
                astToSourceCode(child, out, level);
                if (child != node->children.back())
                    out << " ";
            }
            else if (spacing == COMMA_BETWEEN)
            {
                astToSourceCode(child, out, level);
                if (child != node->children.back())
                    out << ", ";
            }
            else
                astToSourceCode(child, out, level);
        }
        break;

    case AST_VARDEC:
        astToSourceCode(node->children[0], out, 0);
        out << " " << node->symbol->text << " = ";
        astToSourceCode(node->children[1], out, 0);
        out << ";\n";
        break;

    case AST_TYPEINT:
        out << "int";
        break;
    case AST_TYPEBYTE:
        out << "byte";
        break;
    case AST_TYPEREAL:
        out << "real";
        break;

    case AST_LIT:
        if (node->symbol)
        {
            std::string text = node->symbol->text;
            for (char c : text)
            {
                if (c == '\n')
                    out << "\\n";
                else
                    out << c;
            }
        }
        break;

    case AST_ASG:
        out << indent(level) << node->symbol->text << " = ";
        astToSourceCode(node->children[0], out, 0);
        out << ";\n";
        break;

    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
    case AST_LT:
    case AST_GT:
    case AST_LE:
    case AST_GE:
    case AST_EQ:
    case AST_NE:
    case AST_AND:
    case AST_OR:
    {
        const char *ops[] = {"+", "-", "*", "/", "<", ">", "<=", ">=", "==", "!=", "&&", "||"};
        int index = node->type - AST_ADD;

        if (index < 0 || index >= static_cast<int>(sizeof(ops) / sizeof(ops[0])))
        {
            out << "[invalid operator]";
            break;
        }

        const char *op = ops[index];
        if (!node->children.empty() && node->children[0])
            astToSourceCode(node->children[0], out, 0);
        out << " " << op << " ";
        if (node->children.size() > 1 && node->children[1])
            astToSourceCode(node->children[1], out, 0);
        break;
    }

    case AST_NOT:
        out << "~";
        astToSourceCode(node->children[0], out, 0);
        break;

    case AST_IF:
        out << indent(level) << "if (";
        if (!node->children.empty() && node->children[0])
            astToSourceCode(node->children[0], out, 0);
        out << ") ";
        if (node->children.size() > 1 && node->children[1])
            astToSourceCode(node->children[1], out, level);
        break;

    case AST_IF_ELSE:
        out << indent(level) << "if (";
        if (!node->children.empty() && node->children[0])
            astToSourceCode(node->children[0], out, 0);
        out << ") ";
        if (node->children.size() > 1 && node->children[1])
            astToSourceCode(node->children[1], out, level);
        out << indent(level) << "else \n"
            << indent(level);
        if (node->children.size() > 2 && node->children[2])
            astToSourceCode(node->children[2], out, level);
        break;

    case AST_EMPTY:
        out << indent(level) << ";\n";
        break;

    case AST_BLOCK:
        out << "{";
        for (AST *child : node->children)
        {
            if (child && child->type != AST_EMPTY)
            {
                out << "\n";
                astToSourceCode(child, out, level + 1);
            }
        }
        out << indent(level) << "}\n";
        break;

    case AST_SYMBOL:
        if (node->symbol)
            out << node->symbol->text;
        else
            out << "[null symbol]";
        break;

    case AST_FUNDEC:
        astToSourceCode(node->children[0], out, 0);
        out << " " << node->symbol->text << " (";
        for (size_t i = 0; i < node->children[1]->children.size(); ++i)
        {
            astToSourceCode(node->children[1]->children[i], out, 0);
            if (i < node->children[1]->children.size() - 1)
                out << ", ";
        }
        out << ") ";
        astToSourceCode(node->children[2], out, level);
        break;

    case AST_VECTORDEC:
        astToSourceCode(node->children[0], out, 0);
        out << " " << node->symbol->text << "[";
        astToSourceCode(node->children[1], out, 0);
        out << "];\n";
        break;

    case AST_VECTORDEC_INIT:
        astToSourceCode(node->children[0], out, 0);
        out << " " << node->symbol->text << "[";
        astToSourceCode(node->children[1], out, 0);
        out << "] = ";
        for (size_t i = 0; i < node->children[2]->children.size(); ++i)
        {
            astToSourceCode(node->children[2]->children[i], out, 0);
            if (i < node->children[2]->children.size() - 1)
                out << ", ";
        }
        out << ";\n";
        break;

    case AST_WHILE:
        out << indent(level) << "while (";
        astToSourceCode(node->children[0], out, 0);
        out << ") do ";
        astToSourceCode(node->children[1], out, level);
        break;

    case AST_DOWHILE:
        out << indent(level) << "do ";
        astToSourceCode(node->children[0], out, level);
        out << indent(level) << "while (";
        astToSourceCode(node->children[1], out, 0);
        out << ");\n";
        break;

    case AST_RETURN:
        out << indent(level) << "return ";
        if (!node->children.empty() && node->children[0])
            astToSourceCode(node->children[0], out, 0);
        out << ";\n";
        break;

    case AST_PRINT:
        out << indent(level) << "print";
        for (size_t i = 0; i < node->children.size(); ++i)
        {
            astToSourceCode(node->children[i], out, 0, SPACE_BETWEEN);
        }
        out << ";\n";
        break;

    case AST_READ:
        out << indent(level) << "read ";
        if (!node->children.empty() && node->children[0])
            astToSourceCode(node->children[0], out, 0);
        out << ";\n";
        break;

    case AST_PARAM:
        if (node->children.size() > 0)
            astToSourceCode(node->children[0], out, 0);
        out << " " << node->symbol->text;
        break;

    case AST_FUNCALL:
        if (node->symbol)
            out << node->symbol->text << "(";
        for (size_t i = 0; i < node->children.size(); ++i)
        {
            astToSourceCode(node->children[i], out, 0, COMMA_BETWEEN);
        }
        out << ")";
        break;

    case AST_VECTORACCESS:
        out << node->symbol->text << "[";
        astToSourceCode(node->children[0], out, 0);
        out << "]";
        break;

    case AST_ASG_VECTOR:
        out << indent(level) << node->symbol->text << "[";
        astToSourceCode(node->children[0], out, 0);
        out << "] = ";
        astToSourceCode(node->children[1], out, 0);
        out << ";\n";
        break;

    default:
        out << "[unhandled node type: " << node->type << "]";
        break;
    }
}

void checkAndSetDeclarations(AST *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case AST_VECTORDEC_INIT:
        if (node->symbol)
        {
            if (node->symbol->type == SYMBOL_IDENTIFIER)
            {
                node->symbol->type = (node->type == AST_VARDEC) ? SYMBOL_VARDEC : SYMBOL_VECTORDEC;

                if (node->children[0] && node->children[1] && node->children[2])
                {
                    long unsigned int idx = stoi(node->children[1]->symbol->text.c_str());
                    if (idx == 0)
                    {
                        fprintf(stderr, "Semantic error at line %d: vector '%s' initialized with size 0.\n", node->lineNumber, node->symbol->text.c_str());
                        exit(4);
                    }
                    if (idx < node->children[2]->children.size())
                    {
                        fprintf(stderr, "Semantic error at line %d: vector '%s' initialized with too many elements.\n", node->lineNumber, node->symbol->text.c_str());
                        exit(4);
                    }

                    if (node->children[0]->type == AST_TYPEINT)
                        node->symbol->dataType = SYMBOL_DATA_TYPE_INT;
                    else if (node->children[0]->type == AST_TYPEBYTE)
                        node->symbol->dataType = SYMBOL_DATA_TYPE_CHAR;
                    else if (node->children[0]->type == AST_TYPEREAL)
                        node->symbol->dataType = SYMBOL_DATA_TYPE_REAL;
                    else
                        node->symbol->dataType = SYMBOL_DATA_TYPE_INVALID;
                }
                else
                {
                    fprintf(stderr, "Internal error: Declaration node for %s is missing type child at line %d.\n", node->symbol->text.c_str(), node->lineNumber);
                    exit(4);
                }
            }
            else
            {
                fprintf(stderr, "Semantic error at line %d: identifier '%s' redeclared.\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
        }
        else
        {
            fprintf(stderr, "Internal error: Declaration node is missing symbol at line %d.\n", node->lineNumber);
            exit(4);
        }
        break;
    case AST_VARDEC:
    case AST_VECTORDEC:
        if (node->symbol)
        {
            if (node->symbol->type == SYMBOL_IDENTIFIER)
            {
                node->symbol->type = (node->type == AST_VARDEC) ? SYMBOL_VARDEC : SYMBOL_VECTORDEC;
                if (node->children[0])
                {
                    if (node->children[0]->type == AST_TYPEINT)
                        node->symbol->dataType = SYMBOL_DATA_TYPE_INT;
                    else if (node->children[0]->type == AST_TYPEBYTE)
                        node->symbol->dataType = SYMBOL_DATA_TYPE_CHAR;
                    else if (node->children[0]->type == AST_TYPEREAL)
                        node->symbol->dataType = SYMBOL_DATA_TYPE_REAL;
                    else
                        node->symbol->dataType = SYMBOL_DATA_TYPE_INVALID;
                }
                else
                {
                    fprintf(stderr, "Internal error: Declaration node for %s is missing type child at line %d.\n", node->symbol->text.c_str(), node->lineNumber);
                    exit(4);
                }
            }
            else
            {
                fprintf(stderr, "Semantic error at line %d: identifier '%s' redeclared.\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
        }
        else
        {
            fprintf(stderr, "Internal error: Declaration node is missing symbol at line %d.\n", node->lineNumber);
            exit(4);
        }
        break;
    case AST_FUNDEC:
        if (node->symbol)
            if (node->symbol->type == SYMBOL_IDENTIFIER)
            {
                node->symbol->declarationAST = node;
                node->symbol->type = SYMBOL_FUNDEC;
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
                    fprintf(stderr, "Semantic error at line %d: function x not declared\n", node->lineNumber);
                    exit(4);
                }
            }
            else
            {
                fprintf(stderr, "Semantic error at line %d: function x redeclared\n", node->lineNumber);
                exit(4);
            }
        else
        {
            fprintf(stderr, "Semantic errorat line %d: function x not declared\n", node->lineNumber);
            exit(4);
        }
        break;
    case AST_ASG:
        if (node->symbol)
        {
            if (node->symbol->type == SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "Semantic error at line %d: variable '%s' used in assignment is not declared.\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
            else if (node->symbol->type == SYMBOL_FUNDEC)
            {
                fprintf(stderr, "Semantic error at line %d: cannot assign to function '%s'.\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
        }
        else
        {
            fprintf(stderr, "Internal error: Assignment node is missing symbol for LHS at line %d.\n", node->lineNumber);
            exit(4);
        }
        break;
    case AST_ASG_VECTOR:
        if (node->symbol)
        {
            if (node->children.size() == 1)
            {
                int idxType = getExprType(node->children[0]);
                if (idxType != SYMBOL_DATA_TYPE_INT && idxType != SYMBOL_DATA_TYPE_CHAR)
                {
                    fprintf(stderr, "Semantic error at line %d: vector index is not an integer\n", node->lineNumber);
                    exit(4);
                }
            }
            if (node->symbol->type == SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "Semantic error at line %d: variable '%s' used in assignment is not declared.\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
            else if (node->symbol->type == SYMBOL_FUNDEC)
            {
                fprintf(stderr, "Semantic error at line %d: cannot assign to function '%s'.\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
        }
        else
        {
            fprintf(stderr, "Internal error: Assignment node is missing symbol for LHS at line %d.\n", node->lineNumber);
            exit(4);
        }
        break;
    case AST_PARAM:
        if (node->symbol)
        {
            if (node->symbol->type == SYMBOL_IDENTIFIER)
            {
                node->symbol->type = SYMBOL_VARDEC;
                if (node->children[0])
                {
                    if (node->children[0]->type == AST_TYPEINT)
                        node->symbol->dataType = SYMBOL_DATA_TYPE_INT;
                    else if (node->children[0]->type == AST_TYPEBYTE)
                        node->symbol->dataType = SYMBOL_DATA_TYPE_CHAR;
                    else if (node->children[0]->type == AST_TYPEREAL)
                        node->symbol->dataType = SYMBOL_DATA_TYPE_REAL;
                    else
                        node->symbol->dataType = SYMBOL_DATA_TYPE_INVALID;
                }
                else
                {
                    fprintf(stderr, "Internal error: Parameter node for %s is missing type child at line %d.\n", node->symbol->text.c_str(), node->lineNumber);
                    exit(4);
                }
            }
            else
            {
                fprintf(stderr, "Semantic error at line %d: parameter name '%s' redeclared or invalid.\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
        }
        else
        {
            fprintf(stderr, "Internal error: Parameter node is missing symbol at line %d.\n", node->lineNumber);
            exit(4);
        }
        break;
    case AST_VECTORACCESS:
    {
        if (node->children.size() == 1)
        {
            int idxType = getExprType(node->children[0]);
            if (idxType != SYMBOL_DATA_TYPE_INT && idxType != SYMBOL_DATA_TYPE_CHAR)
            {
                fprintf(stderr, "Semantic error at line %d: vector index is not an integer\n", node->lineNumber);
                exit(4);
            }
        }
        break;
    }
    default:
        break;
    }
    for (auto child : node->children)
        checkAndSetDeclarations(child);
}

void checkUsage(AST *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case AST_ASG:
    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
    case AST_LT:
    case AST_GT:
    case AST_LE:
    case AST_GE:
    case AST_EQ:
    case AST_NE:
    case AST_AND:
    case AST_OR:
    case AST_NOT:
        if (node->symbol)
        {
            if (node->symbol->type == SYMBOL_VECTORDEC)
            {
                fprintf(stderr, "Semantic error at line %d: vector '%s' used as scalar in expression.\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
            else if (node->symbol->type == SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "Semantic error at line %d: identifier '%s' used in expression is not declared.\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
        }
        break;
    case AST_SYMBOL:
        if (node->symbol)
        {
            if (node->symbol->type == SYMBOL_FUNDEC)
            {
                fprintf(stderr, "Semantic error at line %d: function '%s' used as scalar\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
            else if (node->symbol->type == SYMBOL_VECTORDEC)
            {
                fprintf(stderr, "Semantic error at line %d: vector '%s' used as scalar\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
        }
        break;
    case AST_ASG_VECTOR:
    case AST_VECTORACCESS:
        if (node->symbol)
        {
            if (node->symbol->type == SYMBOL_VARDEC)
            {
                fprintf(stderr, "Semantic error at line %d: variable '%s' used as vector\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
            else if (node->symbol->type == SYMBOL_FUNDEC)
            {
                fprintf(stderr, "Semantic error at line %d: function '%s' used as vector\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
            else if (node->symbol->type != SYMBOL_VECTORDEC)
            {
                fprintf(stderr, "Semantic error at line %d: identifier '%s' is not a vetor\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
        }
        break;
    case AST_FUNCALL:
        if (node->symbol)
        {
            if (node->symbol->type == SYMBOL_VARDEC)
            {
                fprintf(stderr, "Semantic error at line %d: variable '%s' used as function\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
            else if (node->symbol->type == SYMBOL_VECTORDEC)
            {
                fprintf(stderr, "Semantic error at line %d: vector '%s' used as function\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
            else if (node->symbol->type != SYMBOL_FUNDEC)
            {
                fprintf(stderr, "Semantic error at line %d: identifier '%s' is not a function\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
        }
        break;

    default:
        break;
    }

    for (auto child : node->children)
    {
        checkUsage(child);
    }
}

void checkTypes(AST *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case AST_VARDEC:
        if (node->symbol && node->children.size() == 2)
        {
            int declaredType = node->symbol->dataType;
            int initializerType = getExprType(node->children[1]);

            if (initializerType == SYMBOL_DATA_TYPE_INVALID)
            {
                fprintf(stderr, "Semantic error at line %d: invalid type for initializer of variable '%s'.\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
            if (declaredType == SYMBOL_DATA_TYPE_INVALID)
            {
                fprintf(stderr, "Semantic error at line %d: variable '%s' has an invalid declared type.\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }

            if (initializerType == SYMBOL_DATA_TYPE_STRING &&
                (declaredType == SYMBOL_DATA_TYPE_INT || declaredType == SYMBOL_DATA_TYPE_CHAR || declaredType == SYMBOL_DATA_TYPE_REAL))
            {
                fprintf(stderr, "Semantic error at line %d: cannot initialize numeric variable '%s' with a string.\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
            if (declaredType == SYMBOL_DATA_TYPE_STRING)
            {
                fprintf(stderr, "Semantic error at line %d: cannot declare variable '%s' of type string (not a supported variable type).\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }

            if (!isCompatible(declaredType, initializerType))
            {
                fprintf(stderr, "Semantic error at line %d: incompatible type for initializer of variable '%s'. Declared as %s, initializer is %s.\n",
                        node->lineNumber,
                        node->symbol->text.c_str(),
                        SymbolDataTypeName[declaredType].c_str(),
                        SymbolDataTypeName[initializerType].c_str());
                exit(4);
            }
        }
        break;

    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
    {
        if (node->children.empty())
        {
            fprintf(stderr, "Semantic error at line %d: arithmetic operation with no operands\n", node->lineNumber);
            exit(4);
        }

        int left = getExprType(node->children[0]);
        int right = (node->children.size() > 1) ? getExprType(node->children[1]) : SYMBOL_DATA_TYPE_INVALID;

        // tipo inválido ou string
        if ((left == SYMBOL_DATA_TYPE_INVALID || right == SYMBOL_DATA_TYPE_INVALID) ||
            (left == SYMBOL_DATA_TYPE_STRING || right == SYMBOL_DATA_TYPE_STRING))
        {
            fprintf(stderr, "Semantic error at line %d: invalid type in arithmetic expression\n", node->lineNumber);
            exit(4);
        }

        if (!isCompatible(left, right))
        {
            fprintf(stderr, "Semantic error at line %d: arithmetic operation with incompatible types\n", node->lineNumber);
            exit(4);
        }
        break;
    }

    case AST_VECTORACCESS:
    {
        if (node->children.size() == 1)
        {
            int idxType = getExprType(node->children[0]);
            if (idxType != SYMBOL_DATA_TYPE_INT && idxType != SYMBOL_DATA_TYPE_CHAR)
            {
                fprintf(stderr, "Semantic error at line %d: vector index is not an integer\n", node->lineNumber);
                exit(4);
            }
        }
        break;
    }

    case AST_ASG:
    {
        if (node->children.size() == 1)
        {
            int left = node->symbol->dataType;
            int right = getExprType(node->children[0]);

            // tipo inválido ou string
            if ((left == SYMBOL_DATA_TYPE_INVALID || right == SYMBOL_DATA_TYPE_INVALID) ||
                (left == SYMBOL_DATA_TYPE_STRING || right == SYMBOL_DATA_TYPE_STRING))
            {
                fprintf(stderr, "Semantic error at line %d: invalid type in attribution\n", node->lineNumber);
                exit(4);
            }
            // tipos diferentes
            if (!isCompatible(left, right))
            {
                fprintf(stderr, "Semantic error at line %d: incompatible types in attribution\n", node->lineNumber);
                exit(4);
            }
        }
        break;
    }

    case AST_ASG_VECTOR:
    {
        if (node->children.size() == 2)
        {
            int idxType = getExprType(node->children[0]);
            int valueType = getExprType(node->children[1]);
            if (idxType != SYMBOL_DATA_TYPE_INT && idxType != SYMBOL_DATA_TYPE_CHAR)
            {
                fprintf(stderr, "Semantic error at line %d: vector index is not an integer\n", node->lineNumber);
                exit(4);
            }
            if (valueType == SYMBOL_DATA_TYPE_INVALID || valueType == SYMBOL_DATA_TYPE_STRING)
            {
                fprintf(stderr, "Semantic error at line %d: invalid type in vector attribution\n", node->lineNumber);
                exit(4);
            }
            if (node->symbol->dataType == SYMBOL_DATA_TYPE_INVALID)
            {
                fprintf(stderr, "Semantic error at line %d: vector '%s' has an invalid declared type.\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
            if (!isCompatible(node->symbol->dataType, valueType))
            {
                fprintf(stderr, "Semantic error at line %d: incompatible types in vector attribution\n", node->lineNumber);
                exit(4);
            }
        }
        break;
    }

    case AST_FUNDEC:
    {
        if (node->children.size() == 3)
        {
            int returnType = getExprType(node->children[0]);
            if (returnType == SYMBOL_DATA_TYPE_INVALID ||
                returnType == SYMBOL_DATA_TYPE_STRING)
            {
                fprintf(stderr, "Semantic error at line %d: invalid type in function return\n", node->lineNumber);
                exit(4);
            }
            node->symbol->dataType = returnType;
        }
        break;
    }

    case AST_RETURN:
    {
        if (node->children.size() == 1)
        {
            int returnType = getExprType(node->children[0]);
            if (returnType == SYMBOL_DATA_TYPE_INVALID ||
                returnType == SYMBOL_DATA_TYPE_STRING)
            {
                fprintf(stderr, "Semantic error at line %d: invalid type in function return\n", node->lineNumber);
                exit(4);
            }
            if (node->symbol && node->symbol->declarationAST)
            {
                int funcReturnType = node->symbol->declarationAST->symbol->dataType;
                if (funcReturnType == SYMBOL_DATA_TYPE_INVALID)
                {
                    fprintf(stderr, "Semantic error at line %d: function '%s' has an invalid declared return type.\n", node->lineNumber, node->symbol->text.c_str());
                    exit(4);
                }
                if (!isCompatible(returnType, funcReturnType))
                {
                    fprintf(stderr, "Semantic error at line %d: incompatible types in function return\n", node->lineNumber);
                    exit(4);
                }
            }
        }
        break;
    }

    case AST_IF:
    case AST_IF_ELSE:
    case AST_WHILE:
    {
        if (node->children.size() == 2)
        {
            int condType = getExprType(node->children[0]);
            if (condType != SYMBOL_DATA_TYPE_INT)
            {
                fprintf(stderr, "Semantic error at line %d: condition is not a boolean\n", node->lineNumber);
                exit(4);
            }
        }
        break;
    }

    case AST_DOWHILE:
    {
        if (node->children.size() == 2)
        {
            int condType = getExprType(node->children[1]);
            if (condType != SYMBOL_DATA_TYPE_INT)
            {
                fprintf(stderr, "Semantic error at line %d: do-while condition is not a boolean\n", node->lineNumber);
                exit(4);
            }
        }
        break;
    }

    case AST_PRINT:
    {
        for (auto child : node->children[0]->children)
        {
            int printType = child->type;
            if (printType == SYMBOL_DATA_TYPE_INVALID)
            {
                fprintf(stderr, "Semantic error at line %d: invalid type in print\n", node->lineNumber);
                exit(4);
            }
            if (auto search = typesAllowedInPrint.find(printType); search == typesAllowedInPrint.end())
            {
                fprintf(stderr, "Semantic error at line %d: type '%s' not allowed in print\n", node->lineNumber, SymbolDataTypeName[printType].c_str());
                exit(4);
            }
        }
        break;
    }

    case AST_READ:
    {
        for (auto child : node->children)
        {
            if (child->type == AST_SYMBOL)
            {
                if (child->symbol && child->symbol->type != SYMBOL_VARDEC)
                {
                    fprintf(stderr, "Semantic error at line %d: reading undeclared variable\n", node->lineNumber);
                    exit(4);
                }
            }
            else
            {
                fprintf(stderr, "Semantic error at line %d: reading invalid expression\n", node->lineNumber);
                exit(4);
            }
        }
        break;
    }

    case AST_PARAM:
    {
        if (node->children.size() == 1)
        {
            int paramType = getExprType(node->children[0]);
            if (paramType == SYMBOL_DATA_TYPE_INVALID ||
                paramType == SYMBOL_DATA_TYPE_STRING)
            {
                fprintf(stderr, "Semantic error at line %d: invalid type in function parameter\n", node->lineNumber);
                exit(4);
            }
            node->symbol->dataType = paramType;
        }
        break;
    }

    case AST_VECTORDEC:
    {
        if (node->children.size() == 2)
        {
            int sizeType = getExprType(node->children[1]);
            if (sizeType != SYMBOL_DATA_TYPE_INT)
            {
                fprintf(stderr, "Semantic error at line %d: vector size is not an integer\n", node->lineNumber);
                exit(4);
            }
        }
        break;
    }

    case AST_VECTORDEC_INIT:
    {
        if (node->children.size() == 3)
        {
            int sizeType = getExprType(node->children[1]);
            if (sizeType != SYMBOL_DATA_TYPE_INT)
            {
                fprintf(stderr, "Semantic error at line %d: vector size is not an integer\n", node->lineNumber);
                exit(4);
            }
            for (auto child : node->children[2]->children)
            {
                int valueType = getExprType(child);
                if (valueType == SYMBOL_DATA_TYPE_INVALID ||
                    valueType == SYMBOL_DATA_TYPE_STRING ||
                    !isCompatible(valueType, node->symbol->dataType))
                {
                    fprintf(stderr, "Semantic error at line %d: invalid type in vector initialization\n", node->lineNumber);
                    exit(4);
                }
            }
        }
        break;
    }

    case AST_LT:
    case AST_GT:
    case AST_LE:
    case AST_GE:
    case AST_EQ:
    case AST_NE:
    {
        if (node->children.size() == 2)
        {
            int left = getExprType(node->children[0]);
            int right = getExprType(node->children[1]);
            if ((left == SYMBOL_DATA_TYPE_INVALID || right == SYMBOL_DATA_TYPE_INVALID) ||
                (left == SYMBOL_DATA_TYPE_STRING || right == SYMBOL_DATA_TYPE_STRING))
            {
                fprintf(stderr, "Semantic error at line %d: invalid type in comparison\n", node->lineNumber);
                exit(4);
            }
            if (!isCompatible(left, right))
            {
                fprintf(stderr, "Semantic error at line %d: incompatible types in comparison\n", node->lineNumber);
                exit(4);
            }
        }
        break;
    }

    case AST_AND:
    case AST_OR:
    {
        if (node->children.size() == 2)
        {
            int left = getExprType(node->children[0]);
            int right = getExprType(node->children[1]);
            if ((left == SYMBOL_DATA_TYPE_INVALID || right == SYMBOL_DATA_TYPE_INVALID) ||
                (left == SYMBOL_DATA_TYPE_STRING || right == SYMBOL_DATA_TYPE_STRING))
            {
                fprintf(stderr, "Semantic error at line %d: invalid type in logical operation\n", node->lineNumber);
                exit(4);
            }
            if (!isCompatible(left, right))
            {
                fprintf(stderr, "Semantic error at line %d: incompatible types in logical operation\n", node->lineNumber);
                exit(4);
            }
        }
        break;
    }

    case AST_FUNCALL:
    {
        if (node->symbol)
        {
            AST *funcDecl = node->symbol->declarationAST;
            if (!funcDecl || funcDecl->type != AST_FUNDEC)
            {
                fprintf(stderr, "Semantic error at line %d: function '%s' not declared\n", node->lineNumber, node->symbol->text.c_str());
                exit(4);
            }
            AST *paramList = funcDecl->children[1];
            AST *argList = node->children[0];

            size_t paramCount = paramList->children.size();
            size_t argCount = argList->children.size();

            if (paramCount != argCount)
            {
                fprintf(stderr, "Semantic error at line %d: function '%s' called with wrong arguments (expected %zu, received %zu)\n",
                        node->lineNumber, node->symbol->text.c_str(), paramCount, argCount);
                exit(4);
            }

            for (size_t i = 0; i < paramCount; ++i)
            {
                int paramType = getExprType(paramList->children[i]);
                int argType = getExprType(argList->children[i]);
                if (!isCompatible(paramType, argType))
                {
                    fprintf(stderr, "Semantic error at line %d: argument type %zu incompatible when calling function '%s'\n",
                            node->lineNumber, i + 1, node->symbol->text.c_str());
                    exit(4);
                }
            }
        }
        break;
    }

    default:
        break;
    }
    for (auto child : node->children)
        checkTypes(child);
}

int getExprType(AST *node)
{
    if (!node)
        return SYMBOL_DATA_TYPE_INVALID;

    switch (node->type)
    {
    case AST_SYMBOL:
    case AST_VECTORACCESS:
        if (node->symbol)
            return node->symbol->dataType;
        else
            return SYMBOL_DATA_TYPE_INVALID;
    case AST_LIT:
        if (node->symbol)
        {
            switch (node->symbol->type)
            {
            case SYMBOL_LIT_INT:
                return SYMBOL_DATA_TYPE_INT;
            case SYMBOL_LIT_CHAR:
                return SYMBOL_DATA_TYPE_CHAR;
            case SYMBOL_LIT_REAL:
                return SYMBOL_DATA_TYPE_REAL;
            case SYMBOL_LIT_STRING:
                return SYMBOL_DATA_TYPE_STRING;
            default:
                return SYMBOL_DATA_TYPE_INVALID;
            }
        }
        return SYMBOL_DATA_TYPE_INVALID;
    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
    {
        int left = getExprType(node->children[0]);
        int right = getExprType(node->children[1]);
        if (left == SYMBOL_DATA_TYPE_REAL && right == SYMBOL_DATA_TYPE_REAL)
            return SYMBOL_DATA_TYPE_REAL;
        if ((left == SYMBOL_DATA_TYPE_INT && right == SYMBOL_DATA_TYPE_INT) ||
            (left == SYMBOL_DATA_TYPE_CHAR && right == SYMBOL_DATA_TYPE_CHAR) ||
            (left == SYMBOL_DATA_TYPE_INT && right == SYMBOL_DATA_TYPE_CHAR) ||
            (left == SYMBOL_DATA_TYPE_CHAR && right == SYMBOL_DATA_TYPE_INT))
            return SYMBOL_DATA_TYPE_INT;
        return SYMBOL_DATA_TYPE_INVALID;
    }
    case AST_TYPEBYTE:
        return SYMBOL_DATA_TYPE_CHAR;
    case AST_TYPEINT:
        return SYMBOL_DATA_TYPE_INT;
    case AST_TYPEREAL:
        return SYMBOL_DATA_TYPE_REAL;

    case AST_LT:
    case AST_GT:
    case AST_LE:
    case AST_GE:
    case AST_EQ:
    case AST_NE:
    {
        int left = getExprType(node->children[0]);
        int right = getExprType(node->children[1]);
        if ((left == SYMBOL_DATA_TYPE_INT && right == SYMBOL_DATA_TYPE_INT) ||
            (left == SYMBOL_DATA_TYPE_CHAR && right == SYMBOL_DATA_TYPE_INT) ||
            (left == SYMBOL_DATA_TYPE_INT && right == SYMBOL_DATA_TYPE_CHAR) ||
            (left == SYMBOL_DATA_TYPE_CHAR && right == SYMBOL_DATA_TYPE_CHAR))
            return SYMBOL_DATA_TYPE_INT;
        if (left == SYMBOL_DATA_TYPE_REAL && right == SYMBOL_DATA_TYPE_REAL)
            return SYMBOL_DATA_TYPE_REAL;
        return SYMBOL_DATA_TYPE_INVALID;
    }
    case AST_AND:
    case AST_OR:
    {
        int left = getExprType(node->children[0]);
        int right = getExprType(node->children[1]);
        if ((left == SYMBOL_DATA_TYPE_INT && right == SYMBOL_DATA_TYPE_INT) ||
            (left == SYMBOL_DATA_TYPE_CHAR && right == SYMBOL_DATA_TYPE_INT) ||
            (left == SYMBOL_DATA_TYPE_INT && right == SYMBOL_DATA_TYPE_CHAR) ||
            (left == SYMBOL_DATA_TYPE_CHAR && right == SYMBOL_DATA_TYPE_CHAR))
            return SYMBOL_DATA_TYPE_INT;
        return SYMBOL_DATA_TYPE_INVALID;
    }
    case AST_NOT:
    {
        int left = getExprType(node->children[0]);
        if (left == SYMBOL_DATA_TYPE_INT || left == SYMBOL_DATA_TYPE_CHAR)
            return SYMBOL_DATA_TYPE_INT;
        return SYMBOL_DATA_TYPE_INVALID;
    }
    case AST_FUNCALL:
    case AST_PARAM:
    {
        if (node->symbol)
            return node->symbol->dataType;
        return SYMBOL_DATA_TYPE_INVALID;
    }

    default:
        return SYMBOL_DATA_TYPE_INVALID;
    }
}

// END OF FILE
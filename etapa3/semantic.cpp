// semantic.cpp - marthyna 2025

#include "semantic.hpp"

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

// checks

// END OF FILE
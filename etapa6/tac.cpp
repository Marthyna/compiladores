// UFRGS - Marthyna Weber - 2025-1
//
// tac.cpp - three address code
// Código parcialmente desenvolvido em sala de aula pelo professor Marcelo Johann.

#include "tac.hpp"
#include <stdio.h>
#include <ostream>
#include <set>

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
    "TAC_IFZ",
    "TAC_LABEL",
    "TAC_BEGINFUN",
    "TAC_ENDFUN",
    "TAC_JUMP"};

TAC *tacCreate(int type, SYMBOL *res, SYMBOL *op1, SYMBOL *op2)
{
    TAC *newtac = (TAC *)calloc(1, sizeof(TAC));
    newtac->type = type;
    newtac->res = res;
    newtac->op1 = op1;
    newtac->op2 = op2;
    newtac->prev = 0;
    return newtac;
}

std::string reverseStr(const std::string &s)
{
    std::string r;
    for (int i = s.size() - 1; i >= 0; --i)
        r += s[i];
    return r;
};

void tacPrintSingle(TAC *tac)
{
    if ((!tac) || (tac->type == TAC_SYMBOL))
        return;

    fprintf(stderr, "TAC(");
    switch (tac->type)
    {
    case TAC_ADD:
        fprintf(stderr, "ADD");
        break;
    case TAC_SUB:
        fprintf(stderr, "SUB");
        break;
    case TAC_MUL:
        fprintf(stderr, "MUL");
        break;
    case TAC_DIV:
        fprintf(stderr, "DIV");
        break;
    case TAC_IFZ:
        fprintf(stderr, "IFZ");
        break;
    case TAC_LABEL:
        fprintf(stderr, "LABEL");
        break;
    case TAC_VARDEC:
        fprintf(stderr, "VARDEC");
        break;
    case TAC_BEGINFUN:
        fprintf(stderr, "BEGINFUN");
        break;
    case TAC_ENDFUN:
        fprintf(stderr, "ENDFUN");
        break;
    case TAC_FUNDEC:
        fprintf(stderr, "FUNDEC");
        break;
    case TAC_VECTORDEC:
        fprintf(stderr, "VECTORDEC");
        break;
    case TAC_VECTORDEC_INIT:
        fprintf(stderr, "VECTORDEC_INIT");
        break;
    case TAC_VECTORACCESS:
        fprintf(stderr, "VECTORACCESS");
        break;
    case TAC_LIT:
        fprintf(stderr, "LIT");
        break;
    case TAC_LE:
        fprintf(stderr, "LE");
        break;
    case TAC_GE:
        fprintf(stderr, "GE");
        break;
    case TAC_LT:
        fprintf(stderr, "LT");
        break;
    case TAC_GT:
        fprintf(stderr, "GT");
        break;
    case TAC_EQ:
        fprintf(stderr, "EQ");
        break;
    case TAC_NE:
        fprintf(stderr, "NE");
        break;
    case TAC_AND:
        fprintf(stderr, "AND");
        break;
    case TAC_OR:
        fprintf(stderr, "OR");
        break;
    case TAC_NOT:
        fprintf(stderr, "NOT");
        break;
    case TAC_ASG:
        fprintf(stderr, "ASG");
        break;
    case TAC_READ:
        fprintf(stderr, "READ");
        break;
    case TAC_PRINT:
        fprintf(stderr, "PRINT");
        break;
    case TAC_RETURN:
        fprintf(stderr, "RETURN");
        break;
    case TAC_FUNCALL:
        fprintf(stderr, "FUNCALL");
        break;
    case TAC_ASG_VECTOR:
        fprintf(stderr, "ASG_VECTOR");
        break;
    case TAC_JUMP:
        fprintf(stderr, "JUMP");
        break;
    case TAC_IF:
        fprintf(stderr, "IF");
        break;
    case TAC_IF_ELSE:
        fprintf(stderr, "IF_ELSE");
        break;
    case TAC_WHILE:
        fprintf(stderr, "WHILE");
        break;
    case TAC_DOWHILE:
        fprintf(stderr, "DOWHILE");
        break;
    case TAC_PARAM:
        fprintf(stderr, "PARAM");
        break;
    case TAC_VECTORINITVAL:
        fprintf(stderr, "VECTORINITVAL");
        break;
    case TAC_PARAM_DECL:
        fprintf(stderr, "PARAM_DECL");
        break;

    default:
        fprintf(stderr, "UNKNOWN");
        break;
    }

    if (tac->res)
    {
        bool isNumber = true;
        for (char ch : tac->res->text)
            if (ch < '0' || ch > '9')
                isNumber = false;
        if (isNumber)
            fprintf(stderr, ", %s", reverseStr(tac->res->text).c_str());
        else
            fprintf(stderr, ", %s", tac->res->text.c_str());
    }
    if (tac->op1)
    {
        bool isNumber = true;
        for (char ch : tac->op1->text)
            if (ch < '0' || ch > '9')
                isNumber = false;
        if (isNumber)
            fprintf(stderr, ", %s", reverseStr(tac->op1->text).c_str());
        else
            fprintf(stderr, ", %s", tac->op1->text.c_str());
    }
    if (tac->op2)
    {
        bool isNumber = true;
        for (char ch : tac->op2->text)
            if (ch < '0' || ch > '9')
                isNumber = false;
        if (isNumber)
            fprintf(stderr, ", %s", reverseStr(tac->op2->text).c_str());
        else
            fprintf(stderr, ", %s", tac->op2->text.c_str());
    }
    fprintf(stderr, ")\n");
}

void tacPrintBackwards(TAC *tac)
{
    if (!tac)
        return;

    tacPrintBackwards(tac->prev);
    tacPrintSingle(tac);
}

TAC *tacJoin(TAC *l1, TAC *l2)
{
    if (!l1)
        return l2;
    if (!l2)
        return l1;

    TAC *t;
    for (t = l2; t->prev; t = t->prev)
        ;
    t->prev = l1;
    return l2;
}

TAC *binTacCreate(int type, const std::vector<TAC *> children)
{
    SYMBOL *op1 = (children.size() > 0 && children[0]) ? children[0]->res : nullptr;
    SYMBOL *op2 = (children.size() > 1 && children[1]) ? children[1]->res : nullptr;

    TAC *tac = tacCreate(type, makeTemp(), op1, op2);

    return tacJoin(
        tacJoin(
            children.size() > 0 ? children[0] : nullptr,
            children.size() > 1 ? children[1] : nullptr),
        tac);
}

TAC *ifTacCreate(const std::vector<TAC *> children)
{
    SYMBOL *ifLabel = makeLabel();
    TAC *ifTac = tacJoin(
        children.size() > 0 ? children[0] : nullptr,
        tacCreate(
            TAC_IFZ,
            ifLabel,
            children.size() > 0 ? (children[0] ? children[0]->res : 0) : nullptr,
            0));
    TAC *ifLabelTac = tacCreate(TAC_LABEL, ifLabel, 0, 0);

    if (children.size() > 2 ? children[2] : nullptr)
    {
        SYMBOL *elseLabel = makeLabel();
        TAC *elseLabelTac = tacCreate(TAC_LABEL, elseLabel, 0, 0);
        TAC *elseJumpTac = tacCreate(TAC_JUMP, elseLabel, 0, 0);
        TAC *ifElseTac = tacJoin(
            tacJoin(
                tacJoin(
                    tacJoin(
                        tacJoin(
                            ifTac,
                            children.size() > 1 ? children[1] : nullptr),
                        elseJumpTac),
                    ifLabelTac),
                children.size() > 2 ? children[2] : nullptr),
            elseLabelTac);
        return ifElseTac;
    }
    else
    {
        return tacJoin(tacJoin(ifTac, children.size() > 1 ? children[1] : nullptr), ifLabelTac);
    }
}

TAC *functionTacCreate(TAC *symbol, TAC *params, TAC *code)
{
    return tacJoin(
        tacJoin(
            tacJoin(
                tacCreate(
                    TAC_BEGINFUN,
                    symbol->res,
                    0,
                    0),
                params),
            code),
        tacCreate(
            TAC_ENDFUN,
            symbol->res,
            0,
            0));
}

TAC *generateCode(AST *node)
{
    if (!node)
        return nullptr;

    std::vector<TAC *> children;

    for (auto child : node->children)
        children.push_back(generateCode(child));

    switch (node->type)
    {
    case AST_TYPEINT:
    case AST_TYPEBYTE:
    case AST_TYPEREAL:
        return 0;

    case AST_SYMBOL:
    case AST_LIT:
        return tacCreate(TAC_SYMBOL, node->symbol, 0, 0);

    case AST_ADD:
        return binTacCreate(TAC_ADD, children);

    case AST_SUB:
        return binTacCreate(AST_SUB, children);

    case AST_MUL:
        return binTacCreate(TAC_MUL, children);

    case AST_DIV:
        return binTacCreate(TAC_DIV, children);

    case AST_LT:
        return binTacCreate(TAC_LT, children);

    case AST_GT:
        return binTacCreate(TAC_GT, children);

    case AST_LE:
        return binTacCreate(TAC_LE, children);

    case AST_GE:
        return binTacCreate(TAC_GE, children);

    case AST_EQ:
        return binTacCreate(TAC_EQ, children);

    case AST_NE:
        return binTacCreate(TAC_NE, children);

    case AST_AND:
        return binTacCreate(TAC_AND, children);

    case AST_OR:
        return binTacCreate(TAC_OR, children);

    case AST_NOT:
        return binTacCreate(TAC_NOT, children);

    case AST_ASG:
        return tacJoin(
            children.size() > 0 ? children[0] : nullptr,
            tacCreate(
                TAC_ASG,
                node->symbol,
                children.size() > 0 ? (children[0] ? children[0]->res : 0) : nullptr,
                0));

    case AST_VECTORDEC_INIT:
    {
        TAC *initVals = nullptr;
        if (children.size() > 2 && node->children[2])
        {
            AST *listNode = node->children[2];
            int idx = 0;
            for (auto elem : listNode->children)
            {
                TAC *valTac = generateCode(elem);
                if (valTac && valTac->res)
                    initVals = tacJoin(initVals, tacCreate(TAC_VECTORINITVAL, node->symbol, makeIntSymbol(idx), valTac->res));
                idx++;
            }
        }
        return tacJoin(
            children.size() > 0 ? children[0] : nullptr,
            tacJoin(
                children.size() > 1 ? children[1] : nullptr,
                tacJoin(
                    initVals,
                    tacCreate(
                        TAC_VECTORDEC_INIT,
                        node->symbol,
                        children.size() > 1 ? (children[1] ? children[1]->res : 0) : nullptr,
                        0))));
    }

    case AST_VECTORACCESS:
        return tacJoin(
            children.size() > 0 ? children[0] : nullptr,
            tacCreate(
                TAC_VECTORACCESS,
                makeTemp(),
                node->symbol,
                children.size() > 0 ? (children[0] ? children[0]->res : 0) : nullptr));

    case AST_VECTORDEC:
        return tacJoin(
            tacJoin(
                children.size() > 0 ? children[0] : nullptr,
                tacCreate(
                    TAC_VECTORDEC,
                    node->symbol,
                    children.size() > 1 && children[1] ? children[1]->res : nullptr,
                    0)),
            children.size() > 2 ? children[2] : nullptr);

    case AST_IF:
    case AST_IF_ELSE:
        return ifTacCreate(children);

    case AST_WHILE:
        return whileTacCreate(children);

    case AST_DOWHILE:
        return doWhileTacCreate(children);

    case AST_READ:
        return tacCreate(
            TAC_READ,
            (children.size() > 0 && children[0]) ? children[0]->res : nullptr,
            0, 0);

    case AST_PRINT:
    {
        TAC *result = nullptr;
        if (!children.empty() && children[0])
        {
            AST *listNode = node->children[0];
            for (auto elem : listNode->children)
            {
                TAC *elemTac = generateCode(elem);
                result = tacJoin(result, elemTac);
                if (elemTac && elemTac->res)
                    result = tacJoin(result, tacCreate(TAC_PRINT, elemTac->res, 0, 0));
            }
        }
        return result;
    }

    case AST_RETURN:
        return tacJoin(
            children.size() > 0 ? children[0] : nullptr,
            tacCreate(
                TAC_RETURN,
                children.size() > 0 ? (children[0] ? children[0]->res : 0) : nullptr,
                0,
                0));

    case AST_FUNDEC:
        return functionTacCreate(
            tacCreate(
                TAC_SYMBOL,
                node->symbol,
                0,
                0),
            children.size() > 1 ? children[1] : nullptr,
            children.size() > 2 ? children[2] : nullptr);

    case AST_FUNCALL:
    {
        TAC *params = nullptr;
        if (!node->children.empty() && node->children[0])
        {
            AST *argsList = node->children[0];
            for (auto arg : argsList->children)
            {
                TAC *argTac = generateCode(arg);
                params = tacJoin(params, argTac);
                if (argTac && argTac->res)
                    params = tacJoin(params, tacCreate(TAC_PARAM, argTac->res, 0, 0));
            }
        }
        SYMBOL *temp = makeTemp();
        TAC *call = tacCreate(TAC_FUNCALL, temp, node->symbol, 0);
        return tacJoin(params, call);
    }

    case AST_VARDEC:
        return tacJoin(
            tacJoin(
                children.size() > 0 ? children[0] : nullptr,
                tacCreate(
                    TAC_VARDEC,
                    node->symbol,
                    children.size() > 1 && children[1] ? children[1]->res : nullptr,
                    0)),
            children.size() > 2 ? children[2] : nullptr);

    case AST_PARAM:
        return tacJoin(tacCreate(TAC_PARAM_DECL, node->symbol, 0, 0), children.size() > 1 ? children[1] : nullptr);

    case AST_ASG_VECTOR:
        return tacJoin(
            tacJoin(
                children.size() > 0 ? children[0] : nullptr,
                children.size() > 1 ? children[1] : nullptr),
            tacCreate(
                TAC_ASG_VECTOR,
                node->symbol,
                children.size() > 0 && children[0] ? children[0]->res : nullptr,
                children.size() > 1 && children[1] ? children[1]->res : nullptr));

    default:
    {
        TAC *result = nullptr;
        for (auto c : children)
            result = tacJoin(result, c);
        return result;
    }
    }
}

TAC *doWhileTacCreate(const std::vector<TAC *> children)
{
    SYMBOL *labelStart = makeLabel();
    SYMBOL *labelEnd = makeLabel();
    TAC *labelStartTac = tacCreate(TAC_LABEL, labelStart, 0, 0);

    TAC *bodyTacs = children.size() > 0 ? children[0] : nullptr;
    TAC *condTacs = children.size() > 1 ? children[1] : nullptr;
    SYMBOL *condResult = (children.size() > 1 && children[1]) ? children[1]->res : nullptr;

    TAC *ifz = tacCreate(TAC_IFZ, labelEnd, condResult, 0);
    TAC *jump = tacCreate(TAC_JUMP, labelStart, 0, 0);
    TAC *labelEndTac = tacCreate(TAC_LABEL, labelEnd, 0, 0);

    return tacJoin(
        labelStartTac,
        tacJoin(
            bodyTacs,
            tacJoin(
                condTacs,
                tacJoin(
                    ifz,
                    tacJoin(jump, labelEndTac)))));
}

TAC *whileTacCreate(const std::vector<TAC *> children)
{
    SYMBOL *labelStart = makeLabel();
    SYMBOL *labelEnd = makeLabel();
    TAC *labelStartTac = tacCreate(TAC_LABEL, labelStart, 0, 0);

    TAC *condTacs = children.size() > 0 ? children[0] : nullptr;
    SYMBOL *condResult = (children.size() > 0 && children[0]) ? children[0]->res : nullptr;

    TAC *ifz = tacCreate(TAC_IFZ, labelEnd, condResult, 0);
    TAC *jump = tacCreate(TAC_JUMP, labelStart, 0, 0);
    TAC *labelEndTac = tacCreate(TAC_LABEL, labelEnd, 0, 0);

    return tacJoin(
        labelStartTac,
        tacJoin(
            condTacs,
            tacJoin(
                ifz,
                tacJoin(
                    children.size() > 1 ? children[1] : nullptr,
                    tacJoin(jump, labelEndTac)))));
}

void generatePrintTACs(TAC *node, TAC *&result)
{
    if (!node)
        return;
    if (node->type == AST_LIST)
    {
        generatePrintTACs(node->prev, result);
        generatePrintTACs(node, result);
    }
    else if (node->res)
    {
        result = tacJoin(result, tacCreate(TAC_PRINT, node->res, 0, 0));
    }
}
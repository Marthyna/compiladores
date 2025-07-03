// UFRGS - Marthyna Weber - 2025-1
//
// asm.cpp - assembly code generation

#include "asm.hpp"
#include <stdio.h>
#include <ostream>
#include <set>

std::map<std::string, std::string> stringLabels;
static int paramIndex = 0;
static const char *paramRegs[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
static std::vector<std::string> currentFunctionParams;

bool isCurrentFunctionParam(const std::string &name)
{
    for (size_t i = 0; i < currentFunctionParams.size(); ++i)
    {
        if (currentFunctionParams[i] == name)
            return true;
    }
    return false;
}

int getCurrentFunctionParamIndex(const std::string &name)
{
    for (size_t i = 0; i < currentFunctionParams.size(); ++i)
    {
        if (currentFunctionParams[i] == name)
            return static_cast<int>(i);
    }
    return -1;
}

std::tuple<std::string, std::string, std::string> getOpsPIE(TAC *tac)
{
    std::string op1 = getAsmOperand(tac->op1);
    std::string op2 = getAsmOperand(tac->op2);
    std::string res = getAsmOperand(tac->res);
    return std::make_tuple(op1, op2, res);
}

void generateLogicOpAsm(TAC *tac, std::ostream &out, const char *operation)
{
    if (tac->res && tac->op1 && tac->op2)
    {
        auto ops = getOpsPIE(tac);
        out << "    movq " << std::get<0>(ops) << ", %rax\n";
        out << "    " << operation << std::get<1>(ops) << ", %rax\n";
        out << "    movq %rax, " << std::get<2>(ops) << "\n";
    }
}

void generateCompOpAsm(TAC *tac, std::ostream &out, const char *operation)
{
    if (tac->res && tac->op1 && tac->op2)
    {
        auto ops = getOpsPIE(tac);
        out << "    movq " << std::get<0>(ops) << ", %rax\n";
        out << "    cmpq " << std::get<1>(ops) << ", %rax\n";
        out << "    " << operation << " %al\n";
        out << "    movzbq %al, %rax\n";
        out << "    movq %rax, " << std::get<2>(ops) << "\n";
    }
}

void generateArithOpAsm(TAC *tac, std::ostream &out, const std::string &operation)
{
    if (tac->res && tac->op1 && tac->op2)
    {
        auto ops = getOpsPIE(tac);
        out << "    movq " << std::get<0>(ops) << ", %rax\n";

        if (operation == std::string("idivq "))
        {
            out << "    cqto\n";
            out << "    idivq " << std::get<1>(ops) << "\n";
        }
        else
            out << "    " << operation << std::get<1>(ops) << ", %rax\n";
        out << "    movq %rax, " << std::get<2>(ops) << "\n";
    }
}

void generateAsm(TAC *first, std::ostream &out)
{
    out << ".section .bss\n";
    out << "_print_buf: .space 32\n";
    out << ".section .data\n";

    stringLabels.clear();
    int strCount = 0;
    std::vector<TAC *> tacList;
    TAC *tac = first;
    while (tac)
    {
        tacList.push_back(tac);
        tac = tac->prev;
    }
    for (auto it = tacList.rbegin(); it != tacList.rend(); ++it)
    {
        TAC *tac = *it;
        if (tac->type == TAC_VARDEC && tac->op1 && tac->op1->type == SYMBOL_LIT_STRING)
        {
            if (stringLabels.find(tac->op1->text) == stringLabels.end())
            {
                stringLabels[tac->op1->text] = "_str" + std::to_string(strCount++);
            }
        }
        if (tac->res && tac->res->type == SYMBOL_LIT_STRING)
        {
            if (stringLabels.find(tac->res->text) == stringLabels.end())
            {
                stringLabels[tac->res->text] = "_str" + std::to_string(strCount++);
            }
        }
    }
    for (const auto &pair : stringLabels)
    {
        out << pair.second << ":\n    .asciz \"" << escapeString(pair.first) << "\"\n";
    }

    std::map<std::string, bool> declaredSymbols;
    for (auto it = tacList.rbegin(); it != tacList.rend(); ++it)
    {
        TAC *tac = *it;
        if (tac->type == TAC_VARDEC && tac->res && declaredSymbols.find(tac->res->text) == declaredSymbols.end())
        {
            out << ".globl _" << tac->res->text << "\n";
            out << "_" << tac->res->text << ":\n";
            if (tac->op1)
            {
                if (tac->op1->type == SYMBOL_LIT_STRING)
                {
                    out << "    .quad " << stringLabels[tac->op1->text] << "\n";
                }
                else if (tac->op1->type == SYMBOL_LIT_INT)
                {
                    int value = std::stoi(tac->op1->text);
                    out << "    .quad " << value << "\n";
                }
                else
                {
                    out << "    .quad " << tac->op1->text << "\n";
                }
            }
            else
            {
                out << "    .quad 0\n";
            }
            declaredSymbols[tac->res->text] = true;
        }
    }

    std::set<std::string> usedSymbols;
    std::map<std::string, int> vectorSizes;
    std::set<std::string> codeLabels;
    for (auto it = tacList.rbegin(); it != tacList.rend(); ++it)
    {
        TAC *tac = *it;
        if ((tac->type == TAC_LABEL || tac->type == TAC_BEGINFUN || tac->type == TAC_ENDFUN) && tac->res)
        {
            codeLabels.insert(tac->res->text);
        }
        if (tac->type == TAC_FUNDEC && tac->res)
        {
            codeLabels.insert(tac->res->text);
        }
        if ((tac->type == TAC_VECTORDEC || tac->type == TAC_VECTORDEC_INIT) && tac->res && tac->op1)
        {
            int size = 1;
            try
            {
                std::string szStr = reverseStr(tac->op1->text);
                size = std::stoi(szStr);
            }
            catch (...)
            {
            }
            vectorSizes[tac->res->text] = size;
        }
    }
    for (auto it = tacList.rbegin(); it != tacList.rend(); ++it)
    {
        TAC *tac = *it;
        auto addSym = [&](SYMBOL *sym)
        {
            if (!sym)
                return;
            if (codeLabels.find(sym->text) != codeLabels.end())
                return;
            if (sym->type == SYMBOL_LIT_INT || sym->type == SYMBOL_LIT_CHAR || sym->type == SYMBOL_LIT_STRING)
                return;
            usedSymbols.insert(sym->text);
        };
        addSym(tac->res);
        addSym(tac->op1);
        addSym(tac->op2);
    }
    for (const auto &sym : usedSymbols)
    {
        if (declaredSymbols.find(sym) != declaredSymbols.end())
            continue;
        if (sym.empty() || !(isalpha(sym[0]) || sym[0] == '_'))
            continue;
        bool valido = true;
        for (char c : sym)
        {
            if (!(isalnum(c) || c == '_'))
            {
                valido = false;
                break;
            }
        }
        if (!valido)
            continue;
        if (codeLabels.find(sym) != codeLabels.end())
            continue;
        if (vectorSizes.find(sym) != vectorSizes.end())
        {
            out << ".globl _" << sym << "\n";
            int sz = vectorSizes[sym];
            out << "_" << sym << ": .zero " << (sz * 8) << "\n";
        }
        else
        {
            out << "_" << sym << ": .quad 0\n";
        }
        declaredSymbols[sym] = true;
    }

    out << "_print_temp: .quad 0\n";

    out << "\n.section .text\n";
    out << ".globl main\n";
    out << "main:\n";

    for (auto it = tacList.rbegin(); it != tacList.rend(); ++it)
    {
        TAC *tac = *it;

        if (tac->type == TAC_BEGINFUN)
        {
            currentFunctionParams.clear();
            auto jt = it;
            ++jt;
            while (jt != tacList.rend() && (*jt)->type == TAC_PARAM_DECL)
            {
                currentFunctionParams.push_back((*jt)->res->text);
                ++jt;
            }
        }
        generateAsmInstruction(*it, out);
    }

    generateItoa64Asm(out);
}

void generateItoa64Asm(std::ostream &out)
{
    out << "itoa64:\n";
    out << "    lea _print_buf+31(%rip), %rdi\n"; // Point %rdi to the end of the buffer
    out << "    movb $0, (%rdi)\n";               // Null-terminate the string
    out << "    cmpq $0, %rax\n";                 // Check if the number is zero
    out << "    jne itoa64_loop\n";               // If not zero, jump to the loop
    out << "    movb $48, -1(%rdi)\n";            // If zero, write '0' to the buffer
    out << "    lea -1(%rdi), %rdi\n";            // Adjust %rdi to point to the start of the buffer
    out << "    jmp itoa64_done\n";               // Jump to the end

    out << "itoa64_loop:\n";
    out << "    movq $10, %rbx\n";   // Set divisor to 10
    out << "    xorq %rdx, %rdx\n";  // Clear %rdx for division
    out << "    idivq %rbx\n";       // Divide %rcx by 10, quotient in %rax, remainder in %rdx
    out << "    addb $48, %dl\n";    // Convert remainder to ASCII character
    out << "    decq %rdi\n";        // Move %rdi back to the next position in the buffer
    out << "    movb %dl, (%rdi)\n"; // Store the ASCII character in the buffer
    out << "    testq %rax, %rax\n"; // Check if the quotient is zero
    out << "    jnz itoa64_loop\n";  // If not zero, continue the loop

    out << "itoa64_done:\n";
    out << "    lea _print_buf+32(%rip), %rcx\n"; // Point %rcx to the end of the buffer
    out << "    subq %rdi, %rcx\n";               // Calculate the length of the string
    out << "    movq %rcx, %rax\n";               // Move the length into %rax
    out << "    ret\n";

    out << "atoi64:\n";
    out << "    leaq _print_buf(%rip), %rsi\n";
    out << "    xorq %rax, %rax\n";
    out << "    xorq %rbx, %rbx\n";
    out << "atoi64_loop:\n";
    out << "    movzbq (%rsi), %rcx\n";
    out << "    cmpb $0, %cl\n";
    out << "    je atoi64_done\n";
    out << "    cmpb $10, %cl # quebra de linha\n";
    out << "    je atoi64_done\n";
    out << "    subb $'0', %cl\n";
    out << "    imulq $10, %rax\n";
    out << "    addq %rcx, %rax\n";
    out << "    incq %rsi\n";
    out << "    jmp atoi64_loop\n";
    out << "atoi64_done:\n";
    out << "    ret\n";
}

void generateAsmInstruction(TAC *tac, std::ostream &out)
{
    if (!tac)
        return;

    switch (tac->type)
    {
    case TAC_ADD:
    {
        generateArithOpAsm(tac, out, "addq ");
        break;
    }

    case TAC_SUB:
    {
        generateArithOpAsm(tac, out, "subq ");
        break;
    }

    case TAC_MUL:
    {
        generateArithOpAsm(tac, out, "imulq ");
        break;
    }

    case TAC_DIV:
    {
        generateArithOpAsm(tac, out, "idivq ");
        break;
    }

    case TAC_ASG:
        if (tac->res && tac->op1)
        {
            auto ops = getOpsPIE(tac);
            out << "    movq " << std::get<0>(ops) << ", %rax\n";
            out << "    movq %rax, " << std::get<2>(ops) << "\n";
        }
        break;

    case TAC_LT:
        generateCompOpAsm(tac, out, "setl");
        break;

    case TAC_GT:
        generateCompOpAsm(tac, out, "setg");
        break;

    case TAC_LE:
        generateCompOpAsm(tac, out, "setle");
        break;

    case TAC_GE:
        generateCompOpAsm(tac, out, "setge");
        break;

    case TAC_EQ:
        generateCompOpAsm(tac, out, "sete");
        break;

    case TAC_NE:
        generateCompOpAsm(tac, out, "setne");
        break;

    case TAC_AND:
        generateLogicOpAsm(tac, out, "andq ");
        break;

    case TAC_OR:
        generateLogicOpAsm(tac, out, "orq ");
        break;

    case TAC_NOT:
        if (tac->res && tac->op1)
        {
            auto ops = getOpsPIE(tac);
            out << "    movq " << std::get<0>(ops) << ", %rax\n";
            out << "    testq %rax, %rax\n";
            out << "    setz %al\n";
            out << "    movzbq %al, %rax\n";
            out << "    movq %rax, " << std::get<2>(ops) << "\n";
        }
        break;

    case TAC_LABEL:
        if (tac->res)
        {
            out << "_" << tac->res->text << ":\n";
        }
        break;

    case TAC_JUMP:
        if (tac->res)
        {
            out << "    jmp _" << tac->res->text << "\n";
        }
        break;

    case TAC_IFZ:
        if (tac->op1 && tac->res)
        {
            out << "    movq " << getAsmOperand(tac->op1) << ", %rax\n";
            out << "    testq %rax, %rax\n";
            out << "    jz _" << tac->res->text << "\n";
        }
        break;

    case TAC_BEGINFUN:
        paramIndex = 0;
        if (tac->res)
        {
            out << "_" << tac->res->text << ":\n";
            out << "    pushq %rbp\n";
            out << "    movq %rsp, %rbp\n";
            for (size_t i = 0; i < currentFunctionParams.size() && i < 6; ++i)
            {
                out << "    movq " << paramRegs[i] << ", _" << currentFunctionParams[i] << "(%rip)\n";
            }
        }
        break;

    case TAC_ENDFUN:
        out << "    movq %rbp, %rsp\n";
        out << "    popq %rbp\n";
        out << "    ret\n";
        break;

    case TAC_PRINT:
        if (tac->res)
        {
            std::string operand = getAsmOperand(tac->res);
            if (tac->res->type == SYMBOL_LIT_STRING)
            {
                out << "    movq $1, %rax\n";
                out << "    movq $1, %rdi\n";
                out << "    leaq " << operand << "(%rip), %rsi\n";
                out << "    movq $" << (tac->res->text.length()) << ", %rdx\n";
                out << "    syscall\n";
            }
            else
            {
                out << "    movq " << operand << ", %rax\n"; // carrega o valor a ser impresso
                out << "    andq $-16, %rsp\n";              // alinhamento necessário para syscall
                out << "    call itoa64\n";                  // converte o número para string
                out << "    movq %rax, %rdx\n";              // rdx = tamanho da string
                out << "    movq %rdi, %rsi\n";              // rsi = ponteiro para início da string
                out << "    movq $1, %rax\n";                // chamada de sistema para escrever
                out << "    movq $1, %rdi\n";                // escreve na saída padrão
                out << "    syscall\n";                      // executa a chamada de sistema
            }
        }
        break;

    case TAC_READ:
        if (tac->res)
        {
            out << "    movq $0, %rax      # sys_read\n";
            out << "    movq $0, %rdi      # stdin\n";
            out << "    leaq _print_buf(%rip), %rsi\n";
            out << "    movq $32, %rdx     # tamanho\n";
            out << "    syscall\n";
            out << "    call atoi64\n";
            out << "    movq %rax, " << getAsmOperand(tac->res) << "\n";
        }
        break;

    case TAC_RETURN:
        if (tac->op1)
        {
            out << "    movq " << getAsmOperand(tac->op1) << ", %rax\n";
        }
        out << "    movq %rbp, %rsp\n";
        out << "    popq %rbp\n";
        out << "    ret\n";
        break;

    case TAC_PARAM:
        if (tac->res)
        {
            if (paramIndex < 6)
            {
                out << "    movq " << getAsmOperand(tac->res) << ", " << paramRegs[paramIndex++] << "\n";
            }
            else
            {
                out << "    pushq " << getAsmOperand(tac->res) << "\n";
            }
        }
        break;

    case TAC_FUNCALL:
        if (tac->op1)
        {
            out << "    call _" << tac->op1->text << "\n";
            if (tac->res)
            {
                out << "    movq %rax, " << getAsmOperand(tac->res) << "\n";
            }
        }
        break;

    case TAC_VECTORACCESS:
        if (tac->res && tac->op1 && tac->op2)
        {
            out << "    movq " << getAsmOperand(tac->op2) << ", %rax\n";
            out << "    imulq $8, %rax\n";
            out << "    leaq _" << tac->op1->text << "(%rip), %rbx\n";
            out << "    addq %rbx, %rax\n";
            out << "    movq (%rax), %rbx\n";
            out << "    movq %rbx, " << getAsmOperand(tac->res) << "\n";
        }
        break;

    case TAC_ASG_VECTOR:
        if (tac->res && tac->op1 && tac->op2)
        {
            out << "    movq $" << tac->op1->text << ", %rax\n";
            out << "    imulq $8, %rax\n";
            out << "    leaq _" << tac->res->text << "(%rip), %rbx\n";
            out << "    addq %rbx, %rax\n";
            out << "    movq " << getAsmOperand(tac->op2) << ", %rbx\n";
            out << "    movq %rbx, (%rax)\n";
        }
        break;

    default:
        break;
    }
}

std::string getAsmOperand(SYMBOL *symbol)
{
    if (!symbol)
        return "$0";

    if (symbol->type == SYMBOL_LIT_INT)
    {
        return "$" + reverseStr(symbol->text);
    }

    if (symbol->type == SYMBOL_LIT_CHAR)
    {
        return "$" + symbol->text;
    }

    if (symbol->type == SYMBOL_LIT_STRING)
    {
        auto it = stringLabels.find(symbol->text);
        if (it != stringLabels.end())
            return it->second;
        else
            return "$0";
    }

    return "_" + symbol->text + "(%rip)";
}

std::string escapeString(const std::string &s)
{
    std::string r;
    for (char c : s)
    {
        if (c == '\\' || c == '"')
            r += '\\';
        if (c == '\n')
            r += "\\n";
        else if (c == '\t')
            r += "\\t";
        else if (c == '\r')
            r += "\\r";
        else if (c == '\0')
            r += "\\0";
        else
            r += c;
    }
    return r;
}
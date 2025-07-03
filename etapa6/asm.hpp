// UFRGS - Marthyna Weber - 2025-1
//
// asm.hpp - assembly code generation

#ifndef ASM_HPP
#define ASM_HPP
#include <vector>
#include "symbols.hpp"
#include "tac.hpp"

using namespace std;

// ASSEMBLY GENERATION
void generateAsm(TAC *first, std::ostream &out);
void generateItoa64Asm(std::ostream &out);
void generateAsmInstruction(TAC *tac, std::ostream &out);
void generateLogicOpAsm(TAC *tac, std::ostream &out, const char *op);
void generateCompOpAsm(TAC *tac, std::ostream &out, const char *op);
void generateArithOpAsm(TAC *tac, std::ostream &out, const std::string &op);
std::string getAsmOperand(SYMBOL *symbol);
std::string escapeString(const std::string &s);
bool isCurrentFunctionParam(const std::string &name);
int getCurrentFunctionParamIndex(const std::string &name);

#endif // TAC_HPP

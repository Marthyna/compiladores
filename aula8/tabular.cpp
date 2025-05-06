//
//  Tabular Descendent Syntatic Analyzer
//  Marcelo Johann - inf.ufrgs.br - 2023
//  compile with g++ -std=c++11

#include <stdio.h>
#include <iostream>
#include <vector>
#include <deque>

using namespace std;

// Here we must define terminal and non terminal codes and names to print
// Terminals first, and FIRST_NONTERMINAL indicates the first non terminal.

enum Symbol
{
    EPSILON,
    TER_ID,
    TER_PEV,
    TER_IGUAL,
    TER_MAIS,
    TER_MUL,
    TER_ABRE,
    TER_FECHA,
    TER_NUM,
    TER_BEG,
    TER_END,
    TER_FIM,
    NON_F,
    NON_S,
    NON_R,
    NON_T,
    NON_E,
    NON_CMD,
    NON_RESTO,
    NON_LCMD,
    NON_BL,
    MAX_TOKEN
};

const char *SymbolName[] = {"epsilon", "id", ";", "=",
                            "+", "*", "(", ")",
                            "num", "begin", "end", "$",
                            "F", "S", "R", "T", "E",
                            "CMD", "RESTO", "LCMD", "BLOCK", "unknown"};

#define FIRST_NONTERMINAL NON_F
#define NUMBER_OF_TERMINALS FIRST_NONTERMINAL
#define NUMBER_OF_NONS MAX_TOKEN - FIRST_NONTERMINAL
#define TERMINAL(a) (a < FIRST_NONTERMINAL ? true : false)
#define INDEX(s) (s < FIRST_NONTERMINAL ? s : s - FIRST_NONTERMINAL)

// Here we list each production of the GLC, with its index

typedef pair<Symbol, vector<Symbol>> Production;
Production Grammar[] = {
    {EPSILON, {}},                          // 0 is empty
    {NON_BL, {TER_BEG, NON_LCMD, TER_END}}, // 1
    {NON_LCMD, {NON_CMD, NON_RESTO}},       // 2
    {NON_RESTO, {TER_PEV, NON_LCMD}},       // 3
    {NON_RESTO, {EPSILON}},                 // 4
    {NON_CMD, {TER_ID, TER_IGUAL, NON_E}},  // 5
    {NON_CMD, {EPSILON}},                   // 6
    {NON_E, {NON_T, NON_R}},                // 7
    {NON_T, {NON_F, NON_S}},                // 8
    {NON_R, {TER_MAIS, NON_T, NON_R}},      // 9
    {NON_R, {EPSILON}},                     // 10
    {NON_S, {TER_MUL, NON_F, NON_S}},       // 11
    {NON_S, {EPSILON}},                     // 12
    {NON_F, {TER_ID}},                      // 13
    {NON_F, {TER_ABRE, NON_E, TER_FECHA}},  // 14
    {NON_F, {TER_NUM}},                     // 15
};

// This is the LL(1) Parser Table, first column unused
// Each entry contains 0 or the index of a production

int Table[NUMBER_OF_NONS][NUMBER_OF_TERMINALS] = {
    // i  ;  =  +  *  (  )  n  b  e  $
    0, 13, 0, 0, 0, 0, 14, 0, 15, 0, 0, 0,   // F
    0, 0, 12, 0, 12, 11, 0, 12, 0, 0, 12, 0, // S
    0, 0, 10, 0, 9, 0, 0, 10, 0, 0, 10, 0,   // R
    0, 8, 0, 0, 0, 0, 8, 0, 8, 0, 0, 0,      // T
    0, 7, 0, 0, 0, 0, 7, 0, 7, 0, 0, 0,      // E
    0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 6, 0,      // CMD
    0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 4, 0,      // RESTO
    0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0,      // LCMD
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,      // BL
};

// This is the stack in which the parser operates

deque<Symbol> Stack;

// This is our input, already tokenized!

int Entrada[] = {TER_BEG, TER_ID, TER_IGUAL, TER_NUM, TER_PEV,
                 TER_ID, TER_IGUAL, TER_ID, TER_END, TER_FIM};
int nextToken()
{
    static int atual = 0;
    int inputSize = end(Entrada) - begin(Entrada);
    if (atual >= inputSize)
        return 0;
    printf("Lendo token da entrada: %s \n", SymbolName[Entrada[atual]]);
    return Entrada[atual++];
}

// Monitoring and error functions

void printProduction(int productionNumber)
{
    Production p = Grammar[productionNumber];
    printf("Rule: %s->", SymbolName[p.first]);
    for (auto s : p.second)
        printf("%s ", SymbolName[s]);
    printf("\n");
}

void printStack()
{
    printf("STACK: ");
    for (auto x : Stack)
        printf("%s ", SymbolName[x]);
    printf("\n");
}

void error(const char *msg, int top, int input)
{
    fprintf(stderr, msg, SymbolName[top], SymbolName[input]);
    exit(0);
}

// MAIN: need to say more?

int main(int argc, char **argv)
{
    Stack.push_back(NON_BL);
    int input = nextToken(); // read at start up and after match
    do
    {
        getchar();
        printStack();
        int top = Stack.back();
        if (TERMINAL(top))
        {
            // Terminal on top: input must be what we expected
            if (top == input)
            {
                Stack.pop_back();
                printf("Match: desempilha e avanÃ§a\n");
                input = nextToken();
            }
            else
                error("Error: %s expected but %s was found!\n", top, input);
        }
        else
        {
            // There is a non terminal at the top: check Table
            if (Table[INDEX(top)][input] != 0)
            {
                Stack.pop_back();
                int productionNumber = Table[INDEX(top)][input];
                Production p = Grammar[productionNumber];
                int len = p.second.size();
                for (int i = len - 1; i >= 0; --i)
                    if (p.second[i] != EPSILON)
                        Stack.push_back(p.second[i]);
                printProduction(productionNumber);
            }
            else
                error("Error: No rule to match %s with non terminal %s!\n", input, top);
        }
    } while (!Stack.empty());
    fprintf(stderr, "Success! FIM\n");
}

// END OF FILE
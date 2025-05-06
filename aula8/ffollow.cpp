//  Compute First and Follow sets of a GLC
//  Marcelo Johann - inf.ufrgs.br - 2023
//  compile with g++ -std=c++11

#include <stdio.h>
#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;

// Here we must define terminal and non terminal codes and names to print;
// Terminals first, then non terminals - delimited by EPSILON and MAX_TOKEN;
// You should be able to initialize Symbol, SymbolNames and Productions below
// with #include's between '{' and '}' to process any GLC

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
    DOLLAR,
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

const char *SymbolName[] = {
    "epsilon", "id", ";", "=",
    "+", "*", "(", ")",
    "num", "begin", "end", "$",
    "F", "S", "R", "T", "E",
    "CMD", "RESTO", "LCMD", "BLOCK", "unknown"};

// Here we list each production of the GLC, with its index
// The start symbol will be LHS from the production # 1

typedef pair<Symbol, vector<Symbol>> Production;

Production Grammar[] = {
    {EPSILON, {}},                          // 0 is empty for table building
    {NON_BL, {TER_BEG, NON_LCMD, TER_END}}, // 1 - Start symbol is NON_BL
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

// determine which is the first NT to separate terminals from NTs

int FirstNonTerminal = 0;
inline bool isTerminal(Symbol s) { return (s < FirstNonTerminal) ? true : false; }
void initTerminal(void)
{
    FirstNonTerminal = MAX_TOKEN;
    for (auto p : Grammar)
        if (p.first < FirstNonTerminal && p.first != EPSILON)
            FirstNonTerminal = p.first;
    // we could check to see if all symbols above have productions
    printf("First NT is %d, %s\n", FirstNonTerminal, SymbolName[FirstNonTerminal]);
}

// print functions to check and debug

void printProduction(int productionNumber)
{
    Production p = Grammar[productionNumber];
    printf("Production: %s->", SymbolName[p.first]);
    for (auto s : p.second)
        printf("%s ", SymbolName[s]);
    printf("\n");
}

void printGrammar(void)
{
    int size = end(Grammar) - begin(Grammar);
    for (int i = 0; i < size; ++i)
        printProduction(i);
}

// output structures

map<Symbol, set<Symbol>> First;
map<Symbol, set<Symbol>> Follow;

// Includes a terminal symbol or First of an NT on a First set

bool includeFirst(Symbol N, Symbol t)
{
    bool changed = false;
    if (isTerminal(t))
    {
        if (First[N].find(t) == First[N].end())
        {
            First[N].insert(t);
            changed = true;
        }
    }
    else
    {
        for (auto s : First[t])
            if (First[N].find(s) == First[N].end())
            {
                First[N].insert(s);
                changed = true;
            }
    }
    return changed;
}

// Compute First sets for NTs in the grammar

void computeFirst(void)
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (auto p : Grammar)
            if (p.first != EPSILON)
            {
                if (p.second.size() == 0) // Epsilon production
                {
                    if (includeFirst(p.first, EPSILON))
                        changed = true;
                }
                else
                    for (auto s : p.second)
                    {
                        if (includeFirst(p.first, s))
                            changed = true;
                        if (isTerminal(s))
                            break;
                        else if (First[s].find(EPSILON) == First[s].end())
                            break;
                    }
            }
    }
}

// Called whenever we need First from a sequence of symbols

set<Symbol> getFirst(vector<Symbol> sequence)
{
    set<Symbol> result;

    if (sequence.size() == 0)
        return {EPSILON};

    int i = 0;
    for (i = 0; i < sequence.size(); ++i)
    {
        if (isTerminal(sequence[i]))
        {
            result.insert(sequence[i]);
            break;
        }
        else
        {
            for (auto k : First[sequence[i]])
                if (k != EPSILON)
                    result.insert(k);
            if (First[sequence[i]].find(EPSILON) == First[sequence[i]].end())
                break;
        }
    }
    if (i == sequence.size()) // if loop was not broken, all nonterminals with epsilon
        result.insert(EPSILON);
    return result;
}

// Functions to compute Follow's

bool includeFollows(Symbol N, set<Symbol> from)
{
    bool changed = false;
    for (auto f : from)
        if (Follow[N].find(f) == Follow[N].end())
        {
            Follow[N].insert(f);
            changed = true;
        }
    return changed;
}

void computeFollow(void)
{
    bool changed = true;
    Follow[Grammar[1].first].insert(DOLLAR); // Start symbol is followed by $
    while (changed)
    {
        changed = false;
        for (auto p : Grammar)
            for (int i = 0; i < p.second.size(); ++i)
                if (isTerminal(p.second[i]) == false)
                {
                    vector<Symbol> buffer = {}; // brand new every time
                    // copy the sequence to the right from this position
                    for (int j = i + 1; j < p.second.size(); ++j)
                        buffer.push_back(p.second[j]);
                    set<Symbol> newset = getFirst(buffer);
                    for (auto t : newset)
                        if (t != EPSILON)
                            if (Follow[p.second[i]].find(t) == Follow[p.second[i]].end())
                            {
                                Follow[p.second[i]].insert(t);
                                changed = true;
                            }
                    if (newset.find(EPSILON) != newset.end() || (i == (p.second.size() - 1)))
                    {
                        // INCLUDE FOLLOW of the LEFT HAND SIDE
                        if (includeFollows(p.second[i], Follow[p.first]))
                            changed = true;
                    }
                }
    }
}

// function to print results

void printMapSet(map<Symbol, set<Symbol>> F, const char *setName)
{
    for (auto entry : F)
    {
        printf("%s(%s)={ ", setName, SymbolName[entry.first]);
        for (auto t : entry.second)
            printf("%s ", SymbolName[t]);
        printf("}\n");
    }
}

// MAIN: need to say more?

int main(int argc, char **argv)
{
    initTerminal();
    computeFirst();
    computeFollow();
    printMapSet(First, "First");
    printMapSet(Follow, "Follow");
}

// END OF FILE
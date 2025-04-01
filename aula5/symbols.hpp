// UFRGS - Marthyna Weber - 2025-1
//
// symbols.hpp - tabela de símbolos
// Código parcialmente desenvolvido em sala de aula pelo professor.

#include <string>

using namespace std;

struct SYMBOL
{
    int type;
    string text;

public:
    SYMBOL(int type, string text) : type(type), text(text) {};
};

SYMBOL *symbolInsert(int type, char *text);
void symbolPrintTable();

int getLineNumber(void);
int isRunning(void);

// END OF FILE
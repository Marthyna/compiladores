// TODO: Adicionar headers e testes

#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "symbols.hpp"

extern FILE *yyin;
extern int running;
extern int lineNumber;
int yylex();
void symbolPrintTable();

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin)
    {
        fprintf(stderr, "Erro ao abrir arquivo\n");
        return 1;
    }

    while (running)
    {
        int tok = yylex();
        if (!running)
            break;

        printf("TOKEN: %d\n", tok);
    }

    printf("\n--- Tabela de Símbolos ---\n");
    symbolPrintTable();
    printf("Linhas: %d\n", lineNumber);
}
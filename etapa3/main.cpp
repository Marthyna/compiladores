// UFRGS - Marthyna Weber - 2025-1
//
// main.cpp - programa principal
// Código parcialmente desenvolvido em sala de aula pelo professor Marcelo Johann.

#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;
extern int running;
extern int lineNumber;
int yylex();
int yyparse();
void symbolPrintTable();
void initMe();
void processTokens(FILE *file);
int getLineNumber();

int main(int argc, char **argv)
{
    initMe();
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

    yyparse();

    // printf("\n--- Tabela de Símbolos ---\n");
    // symbolPrintTable();
    // printf("Linhas: %d\n", getLineNumber());
    exit(0);
}
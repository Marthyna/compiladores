// UFRGS - Marthyna Weber - 2025-1
//
// main.cpp - programa principal
// Código parcialmente desenvolvido em sala de aula pelo professor Marcelo Johann.

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

extern FILE *yyin;
extern int running;
extern int lineNumber;
int yylex();
int yyparse();
void symbolPrintTable();
void initMe();
void processTokens(FILE *file);
int getLineNumber();
extern std::ostream *out;

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
        fprintf(stderr, "Erro ao abrir arquivo de entrada\n");
        return 1;
    }

    std::ofstream output(argv[2]);
    if (!output.is_open())
    {
        fprintf(stderr, "Erro ao abrir arquivo de saída\n");
        return 1;
    }
    out = &output;

    yyparse();

    // printf("\n--- Tabela de Símbolos ---\n");
    // symbolPrintTable();
    // printf("Linhas: %d\n", getLineNumber());
    exit(0);
}
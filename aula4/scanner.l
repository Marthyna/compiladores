%{
// UFRGS - Marthyna Weber - 2025-1
//
// scanner.l - analisador léxico
// Código parcialmente desenvolvido em sala de aula pelo professor.

// TODO: adicionar função para inverter numeros

#include "tokens.h"
#include "symbols.hpp"

#define MAX_STRING_SIZE 1024

int lineNumber = 1;
int running = 1;
int strIndex = 0;
char strBuffer[MAX_STRING_SIZE];

%}

%option nounput
%option noinput

%x COMMENT_BLOCK COMMENT_LINE STRING_LITERAL CHAR_LITERAL

%%

"byte"                  { return KW_BYTE; } 
"int"                   { return KW_INT; }
"real"                  { return KW_REAL; }
"if"                    { return KW_IF; }
"else"                  { return KW_ELSE; }
"do"                    { return KW_DO; }
"while"                 { return KW_WHILE; }
"read"                  { return KW_READ; }
"print"                 { return KW_PRINT; }
"return"                { return KW_RETURN; }

"<="                    { return OPERATOR_LE; }
">="                    { return OPERATOR_GE; }
"=="                    { return OPERATOR_EQ; }
"!="                    { return OPERATOR_DIF; }

[0-9]+                  { symbolInsert(LIT_INT, yytext); }
[0-9]+"/"[0-9]+         { symbolInsert(LIT_REAL, yytext); }

\"                      { 
                            strIndex = 0;
                            strBuffer[0] = '\0';
                            BEGIN(STRING_LITERAL);
                        }

'([^'\\]|\\[nt"\\'])'   { symbolInsert(LIT_CHAR, yytext); }
'[^']+'                 { 
                            running = 0; 
                            fprintf(stderr, "Erro léxico: Mais de um caractere entre aspas simples. Linha %d\n", lineNumber); 
                            return TOKEN_ERROR; 
                        }

[A-Za-z][A-Za-z0-9_]*   { symbolInsert(TK_IDENTIFIER, yytext); }

[ \t\r]                 { }
\n                      { ++lineNumber; }

"//"                    { BEGIN(COMMENT_LINE); }
"/--"                   { BEGIN(COMMENT_BLOCK); }

[-,;:()[\]{}=+*/%<>&|~] { return yytext[0]; }
.                       { 
                            running = 0;
                            fprintf(stderr, "Erro léxico: Caractere inválido. Linha %d\n", lineNumber); 
                            return TOKEN_ERROR; 
                        }

<COMMENT_LINE>\n        { 
                            ++lineNumber; 
                            BEGIN(INITIAL); 
                        }
<COMMENT_LINE>.         { }

<COMMENT_BLOCK>"--/"    { BEGIN(INITIAL); }
<COMMENT_BLOCK>\n       { ++lineNumber; }
<COMMENT_BLOCK>.        { }

<STRING_LITERAL>\\["nt\\] {
                            if (strIndex < MAX_STRING_SIZE - 2) {
                                if (yytext[1] == 'n') {
                                    strBuffer[strIndex++] = '\n';
                                } else if (yytext[1] == 't') {
                                    strBuffer[strIndex++] = '\t';
                                } else if (yytext[1] == '"') {
                                    strBuffer[strIndex++] = '"';
                                } else if (yytext[1] == '\\') {
                                    strBuffer[strIndex++] = '\\';
                                }
                            }
 }
<STRING_LITERAL>\n      { 
                            fprintf(stderr, "Erro léxico: String não pode conter quebra de linha. Linha %d\n", lineNumber); 
                            running = 0; 
                            return TOKEN_ERROR; 
                        }
<STRING_LITERAL>\"      {
                            strBuffer[strIndex] = '\0';  
                            symbolInsert(LIT_STRING, strBuffer); 
                            BEGIN(INITIAL); 
                        }
<STRING_LITERAL>\"\"+   { 
                            fprintf(stderr, "Erro léxico: Aspas duplas não escapadas. Linha %d\n", lineNumber);
                            running = 0; 
                            return TOKEN_ERROR; }
<STRING_LITERAL>.       { 
                            if (strIndex < MAX_STRING_SIZE - 1) {
                                strBuffer[strIndex++] = yytext[0];
                            }
                        }

%%

int yywrap()
{
    running = 0;
    return 1;
}
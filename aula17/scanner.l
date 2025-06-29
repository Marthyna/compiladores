%{
// UFRGS - Marthyna Weber - 2025-1
//
// scanner.l - analisador léxico
// Código parcialmente desenvolvido em sala de aula pelo professor Marcelo Johann.

#include "ast.hpp"
#include "symbols.hpp"
#include "parser.tab.hpp"

#define MAX_STRING_SIZE 1024

int strIndex = 0;
char strBuffer[MAX_STRING_SIZE];
extern int running;
extern int lineNumber;

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

[0-9]+                  { 
                            yylval.symbol = symbolInsert(SYMBOL_LIT_INT, yytext); 
                            return LIT_INT;
                        }
[0-9]+"/"[0-9]+         { 
                            yylval.symbol = symbolInsert(SYMBOL_LIT_REAL, yytext); 
                            return LIT_REAL;
                        }

\"                      { 
                            strIndex = 0;
                            strBuffer[0] = '\0';
                            BEGIN(STRING_LITERAL);
                        }

'([^'\\]|\\[nt"\\'])'   { 
                            yylval.symbol = symbolInsert(SYMBOL_LIT_CHAR, yytext); 
                            return LIT_CHAR;
                        }
'[^']+'                 { 
                            running = 0; 
                            fprintf(stderr, "Erro léxico: Mais de um caractere entre aspas simples. Linha %d\n", lineNumber); 
                            return YYUNDEF; 
                        }

[A-Za-z][A-Za-z0-9_]*   { 
                            yylval.symbol = symbolInsert(SYMBOL_IDENTIFIER, yytext); 
                            return TK_IDENTIFIER;
                        }

[ \t\r]                 { }
\n                      { ++lineNumber; }

"//"                    { BEGIN(COMMENT_LINE); }
"/--"                   { BEGIN(COMMENT_BLOCK); }

[-,;:()[\]{}=+*/%<>&|~] { return yytext[0]; }
.                       { 
                            running = 0;
                            fprintf(stderr, "Erro léxico: Caractere inválido. Linha %d\n", lineNumber); 
                            return YYUNDEF; 
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
                            return YYUNDEF; 
                        }
<STRING_LITERAL>\"      {
                            strBuffer[strIndex] = '\0';  
                            yylval.symbol = symbolInsert(SYMBOL_LIT_STRING, strBuffer);
                            BEGIN(INITIAL); 
                            return LIT_STRING; 
                        }
<STRING_LITERAL>\"\"+   { 
                            fprintf(stderr, "Erro léxico: Aspas duplas não escapadas. Linha %d\n", lineNumber);
                            running = 0; 
                            return YYUNDEF; }
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
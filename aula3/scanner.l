%{

int lineNumber = 1;
int running = 1;

%}

%x COMMENT

%%

"int"               { return 256; }
"if"                { return 257; }
"for"               { return 258; }
0+                  { return 259; }
[A-Za-z]+           { return 260; }
[ \t]
==.*
"<<"                { BEGIN(COMMENT); }
[-.,:;+*/=(){}[\]]  { return yytext[0]; }
\n                  { ++lineNumber; }
.                   { return 999; }


<COMMENT>\n                  { ++lineNumber; }
<COMMENT>">>"                { BEGIN(INITIAL); }
<COMMENT>.

%%

#include "main.c"

int yywrap()
{
    running = 0;
    return 1;
}
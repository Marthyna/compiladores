%{
// defs opcionais
int lineNumber = 1;

%}

%%
// regras

"int"       { printf("KW int\n"); }
"0"*        { printf("Numeros 0\n"); }
[A-Za-z]+   { printf("Identificador\n"); }
[ \t]
\n          { ++lineNumber; }

%%

// codigo c

int main()
{
    yylex(); 
    printf("Linhas: %d\n", lineNumber);
}

int yywrap()
{
    return 1;
}
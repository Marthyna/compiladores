int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }
    yyin = fopen(argv[1], "r");
    if (yyin == 0)
    {
        fprintf(stderr, "Erro ao abrir arquivo\n");
        return 1;
    }

    while (running) // usar no trabalho
    {
        int tok = yylex();
        if (!running)
            break; // senão tenta capturar um token a mais
        switch (tok)
        {
        case 256:
        {
            fprintf(stderr, "KEYWORD_INT ");
            break;
        }
        case 257:
        {
            fprintf(stderr, "KEYWORD_IF ");
            break;
        }
        case 258:
        {
            fprintf(stderr, "KEYWORD_FOR ");
            break;
        }
        case 259:
        {
            fprintf(stderr, "LIT_0ARRAY ");
            break;
        }
        case 260:
        {
            fprintf(stderr, "NOME ");
            break;
        }
        default:
        {
            fprintf(stderr, "OPERATOR %c ", tok);
            break;
        }
        }
    }

    printf("Linhas: %d\n", lineNumber);
}
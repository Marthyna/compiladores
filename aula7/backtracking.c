#include "backtracking.h"

char *entrada = "[a]$";
int atual = 0;
char token = 0;

void le_tok(void)
{
    token = entrada[atual++];
}

void retrocede(void)
{
    atual -= 2;
    token = entrada[atual];
}

int initial_symbol()
{
    if (token == 'a')
    {
        le_tok();
        return 1;
    }
    else
    {
        if (token == '[')
        {
            le_tok();
            if (l_symbol())
                if (token == ']')
                {
                    le_tok();
                    return 1;
                }
                else
                    return 0;
            else
                return 0;
        }
        else
            return 0;
    }
}

int l_symbol()
{
    if (initial_symbol())
        if (token == ';')
        {
            le_tok();
            if (l_symbol())
                return 1;
            else
                return 0;
        }
        else
        {
            retrocede();
            if (initial_symbol())
                return 1;
            else
                return 0;
        }
    else
        return 0;
}

int main(void)
{
    le_tok();
    if (initial_symbol())
        if (token == '$')
            printf("Fim do programa\n");
        else
            printf("Erro de sintaxe\n");
    else
        printf("Erro de sintaxe\n");
    return 0;
}
char *entrada = "[a]$";
int atual = 0;

struct prodution_t
{
    char *nome;
    char *producao;
};

struct prodution_t producoes[] = {
    {"S", "A"},
    {"A", "a"},
    {"A", "[B]"},
    {"B", "A;B"},
    {"B", "A"}};

char terminais[] = {'a', '[', ']', ';', '$'};

int is_terminal(char c)
{
    for (int i = 0; i < sizeof(terminais) / sizeof(terminais[0]); i++)
    {
        if (c == terminais[i])
            return 1;
    }
    return 0;
}

void first(char *entrada)
{
    char *first_set[256];
    for (int i = 0; i < 256; i++)
    {
        first_set[i] = "NULL";
    }

    int changed = 1;
    while (changed)
    {
        for (int i = 0; i < sizeof(producoes) / sizeof(producoes[0]); i++)
        {
            if (entrada[0] == producoes[i].nome[0])
            {
                if (is_terminal(producoes[i].producao[0]))
                {
                }
            }
        }
    }
}

int main()
{
    first(entrada);
    return 0;
}
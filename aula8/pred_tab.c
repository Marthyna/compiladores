#define N 5
#define T 4

char *terminais[T] = {
    "id",
    "v",
    "&",
    "!"};

char *nao_terminais[N] = {
    "E",
    "E'",
    "T",
    "T'",
    "F"};

struct mapeamento_t
{
    char *nao_terminal;
    char *terminal;
    char *producao[100];
};

struct mapeamento_t M[N][T + 1] = {
    {"E", "id", {"T", "E'"}},
    {"E", "!", {"T", "E'"}},
    {"E'", "v", {"v", "T", "E'"}},
    {"E'", "$", {"null"}},
    {"T", "id", {"F", "T'"}},
    {"T", "!", {"F", "T'"}},
    {"T'", "v", {"null"}},
    {"T'", "&", {"&", "F", "T'"}},
    {"T'", "$", {"null"}},
    {"F", "id", {"id"}},
    {"F", "!", {"!", "F"}}};

char *entrada = "idvid&id$";
int cabecote = 0;
char *pilha[100] = {"E", "$"};
int topo = 0;

// prototipos
int pred_tab(char *sentenca, struct mapeamento_t M[N][T + 1]);
int is_terminal(char c);

int is_terminal(char c)
{
    for (int i = 0; i < sizeof(terminais) / sizeof(terminais[0]); i++)
    {
        if (c == terminais[i])
            return 1;
    }
    return 0;
}

int busca_tabela(char *nao_terminal, char *terminal)
{
    for (int i = 0; i < N; i++)
    {
        if (strcmp(M[i][0].nao_terminal, nao_terminal) == 0)
        {
            for (int j = 0; j < T; j++)
            {
                if (strcmp(M[i][j].terminal, terminal) == 0)
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

int pred_tab(char *sentenca, struct mapeamento_t M[N][T + 1])
{
    char *simbolo = pilha[topo];
    char *terminal = sentenca[cabecote];

    while ((strcmp(simbolo, "$") != 0))
    {
        if (is_terminal(simbolo))
            if (simbolo == terminal)
            {
                topo++;
                cabecote++;
            }
            else
                return -1;
        else
        {
            int mapeamento = busca_tabela(simbolo, terminal);
            if (M[mapeamento]->producao[2][0] != "null")
            {
                        }
            else
                return -1;
        }
    }
}

void main(void)
{
    printf("Sou um analisador preditivo...\n");
    pred_tab(entrada, M);
}
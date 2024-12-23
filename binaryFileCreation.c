// fora de funções

#include <stdio.h>
#include <stdlib.h>

#define PONTUACAO_MARCELO 1000
#define PONTUACAO_GABRIEL 900
#define PONTUACAO_FRANCISCO 800
#define PONTUACAO_ISMAEL 500
#define PONTUACAO_JOANA 300

#define NUMJOG 5
typedef struct // structs need to be outside functions
{
    char nomeJog[20]; // string nome jogador
    int pontosJog; // pontuação jogador
} POSICOES;

// end of declarations

int main() // add retornos e parâmetros
{
    POSICOES leaderboard[5] = {"MARCELO", PONTUACAO_MARCELO, "GABRIEL", PONTUACAO_GABRIEL, "FRANCISCO", PONTUACAO_FRANCISCO, "ISMAEL", PONTUACAO_ISMAEL, "JOANA", PONTUACAO_JOANA};

    FILE *fp;

    // abertura arquivo
    fp = fopen("top_scores.bin", "wb") // write

    if(fp != NULL) // se der certo
    {
        if(fwrite(leaderboard, sizeof(POSICOES), NUMJOG, fp) != NUMJOG)
            printf("ERRO NA ESCRITA!");
        else
            printf("ARQUIVO ESCRITO COM SUCESSO!\n");
    }
    fclose(fp);

    return 0;
}

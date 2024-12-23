void binaryFileWrite(int posicao, char nomeJog[], int pontuacao)
{
    POSICOES novoJog = {nomeJog, pontuacao};
    
    leaderboard[posicao] = novoJog;

    printf("INFORMACAO TROCADA NO ARRAY!");
}
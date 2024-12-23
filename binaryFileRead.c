void binaryFileRead()
{
    FILE *fp;

    fp = fopen(nome_arq, "rb");

    if(fp != NULL)
    {
        // fread(top5, sizeof(JOGADOR), MAXSCORES, fp);
        if(fread(leaderboard, sizeof(POSICOES), NUMJOG, fp) != NUMJOG)
            printf("ERRO NA LEITURA!");
        else
            printf("ARQUIVO LIDO COM SUCESSO!");
    }
    fclose(fp);
}
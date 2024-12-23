void binaryFileRead()
{
    FILE *fp;

    fp = fopen("top_scores.bin", "rb");

    if(fp != NULL)
    {
        if(fread(leaderboard, sizeof(POSICOES), NUMJOG, fp) != NUMJOG)
            printf("ERRO NA LEITURA!");
        else
            printf("ARQUIVO LIDO COM SUCESSO!");
    }
    fclose(fp);
}

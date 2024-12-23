int binaryFileSave(POSICOES leaderboard[NUMJOG])
{
    FILE *fp;

    fp = fopen("top_score.bin", "wb");

    if(fp != NULL)
    {
        if(fwrite(leaderboard, sizeof(POSICOES), NUMJOG, fp) != NUMJOG)
            printf("ERRO NA ESCRITA!");
        else
            printf("ARQUIVO ESCRITO COM SUCESSO!\n");
    }
    fclose(fp);
}
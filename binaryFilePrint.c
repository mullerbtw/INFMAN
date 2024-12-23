void binaryFilePrint(POSICOES leaderboard[])
{
    for(int i = 0; i < MAXSCORES; i++)
        {
            printf("%s - ", leaderboard[i].nomeJog);
            printf("%d\n", leaderboad[i].pontosJog);
        }
}
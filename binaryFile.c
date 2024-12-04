// fora de funções

#DEFINE PONTUACAO_MARCELO 1000
#DEFINE PONTUACAO_GABRIEL 900
#DEFINE PONTUACAO_FRANCISCO 800
#DEFINE PONTUACAO_ISMAEL 500
#DEFINE PONTUACAO_JOANA 300

struct posicoes // structs need to be outside functions
{
   char nomeJog[]; // string nome jogador
   int pontosJog; // pontuação jogador
}; 

// end of declarations

int arquivoBinario() // add retornos e parâmetros
{
   struct posicoes leaderboard[5];
  
   leaderboard[0] = {"marcelo", PONTUACAO_MARCELO};
   leaderboard[1] = {"gabriel", PONTUACAO_GABRIEL};
   leaderboard[2] = {"francisco", PONTUACAO_FRANCISCO};
   leaderboard[3] = {"ismael", PONTUACAO_ISMAEL};
   leaderboard[4] = {"joana", PONTUACAO_JOANA};
  
   FILE *fp;
  
   // abertura arquivo
   fp = fopen("top_scores.bin", "rb+") // read && write
  
   if(fp != NULL) // leitura e escrita
   
   {
      for(int i = 0; i < JOG; i++)
      
      {
         fwrite(leaderboard[i].nomeJog);
         fwrite(leaderboard[i].pontosJog);
      }
      
      for(int j = 0; j < JOG; j++)
      
      {
         if(pontuacaoAtual >= leaderboard[j].pontosJog)
         {
            leaderboard[j].pontosJog = pontuacaoAtual; // !declared
            leaderboard[j].nomeJog = jogAtual; // !declared
         }
      }
   
   fclose(fp); // fechar arquivo
  }
  
  else
  {
     printf("erro de abertura!"); // checagem abertura
  }
  
return 0;
}
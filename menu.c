#include "raylib.h"

#define PLAY 1
#define LEADERBOARD 2
#define EXIT 3


int main() // não recebe nenhum valor
{
   const int screenWidth = 1200;
   const int screenHeight = 600;

   InitWindow(screenWidth, screenHeight, "INF MAN");

   // const char titleText[8] = {"INF MAN"}; 
   // const char playText[5] = {"play"};
   // const char leaderboardText[12] = {"leaderboard"};
   // const char exitText[5] = {"exit"};

   SetTargetFPS(60);


   while(!WindowShouldClose())
   {
      BeginDrawing();

      ClearBackground(SKYBLUE);

      DrawText("INF MAN", screenWidth / 2.0f - MeasureText("INF MAN", 80) / 2.0f, 80, 80, RAYWHITE);
      DrawText("play", screenWidth / 2.0f - MeasureText("play", 40) / 2.0f, 250, 40, RAYWHITE);
      DrawText("leaderboard", screenWidth / 2.0f - MeasureText("leaderboard", 40) / 2.0f , 350, 40, RAYWHITE);
      DrawText("exit", screenWidth / 2.0f - MeasureText("exit", 40) / 2.0f, 450, 40, RAYWHITE);

      Rectangle playRec = (Rectangle)
      {
         screenWidth / 2.0f - MeasureText("play", 40) / 2.0f,
         250,
         MeasureText("play", 40),
         40
      };

      Rectangle leaderboardRec = (Rectangle)
      {
         screenWidth / 2.0f - MeasureText("leaderboard", 40) / 2.0f,
         350,
         MeasureText("leaderboard", 40),
         40
      };

      Rectangle exitRec = (Rectangle)
      {
         screenWidth / 2.0f - MeasureText("exit", 40) / 2.0f,
         450,
         MeasureText("exit", 40),
         40
      };

      if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      {
         if(CheckCollisionPointRec(GetMousePosition(), playRec))
         // if button pressed on 'play' area
         {
            return PLAY;
         }

         if(CheckCollisionPointRec(GetMousePosition(), leaderboardRec))
         // if button pressed on 'leaderboard' area
         {
            return LEADERBOARD;
            // only load .bin file if leaderboard chosen
         }

         if(CheckCollisionPointRec(GetMousePosition(), exitRec))
         // if button pressed on 'exit' area
         {
            return EXIT;
         }
       
      }
      
      EndDrawing();
      

   }
   
   CloseWindow(); // necessary (?)
   return 0;
}   

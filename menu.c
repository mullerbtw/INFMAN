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
   
   Texture2D title = LoadTexture("INFMAN.png");
   
   SetTargetFPS(60);


   while(!WindowShouldClose())
   {
      BeginDrawing();

      ClearBackground(DARKBLUE);

      DrawTexture(title, screenWidth / 2 - 426 / 2, 100, RAYWHITE);
      DrawText("PLAY", screenWidth / 2.0f - MeasureText("PLAY", 30) / 2.0f, 280, 30, RAYWHITE);
      DrawText("LEADERBOARD", screenWidth / 2.0f - MeasureText("LEADERBOARD", 30) / 2.0f , 360, 30, RAYWHITE);
      DrawText("EXIT", screenWidth / 2.0f - MeasureText("EXIT", 30) / 2.0f, 440, 30, RAYWHITE);

      Rectangle playRec = (Rectangle)
      {
         screenWidth / 2.0f - MeasureText("PLAY", 30) / 2.0f,
         280,
         MeasureText("PLAY", 30),
         30
      };

      Rectangle leaderboardRec = (Rectangle)
      {
         screenWidth / 2.0f - MeasureText("LEADERBOARD", 30) / 2.0f,
         360,
         MeasureText("LEADERBOARD", 30),
         30
      };

      Rectangle exitRec = (Rectangle)
      {
         screenWidth / 2.0f - MeasureText("EXIT", 30) / 2.0f,
         440,
         MeasureText("EXIT", 30),
         30
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
   
   UnloadTexture(title);
   
   CloseWindow(); // necessary (?)
   
   return 0;
}   

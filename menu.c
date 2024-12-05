#include "raylib.h"

#define PLAY 1
#define LEADERBOARD 2
#define EXIT 3


int main() // não recebe nenhum valor
{
   const int screenWidth = 1200;
   const int screenHeight = 600;

   InitWindow(screenWidth, screenHeight, "INF MAN");

   const char titleText[8] = {"INF MAN"}; 
   const char playText[5] = {"play"};
   const char leaderboardText[12] = {"leaderboard"};
   const char exitText[5] = {"exit"};

   SetTargetFPS(60);


   while(!WindowShouldClose())
   {
      BeginDrawing();

      ClearBackground(SKYBLUE);

      DrawText(titleText,(screenWidth - MeasureText()) / 2, 80, 70, 3, RAYWHITE);
      DrawText(playText, (screenWidth - MeasureText()) / 2, 200, 30, 2, RAYWHITE);
      DrawText(leaderboardText, (screenWidth - MeasureText()) / 2, 300, 30, 2, RAYWHITE);
      DrawText(exitText, (screenWidth - MeasureText()) / 2, 400, 30, 2, RAYWHITE);

      Rectangle playRec = (Rectangle)
      {
         (screenWidth - MeasureText(playText, 30)) / 2,
         200,
         MeasureText(playText, 30),
         30
      };

      Rectangle leaderboardRec = (Rectangle)
      {
         (screenWidth - MeasureText(leaderboardText, 30)) / 2,
         300,
         MeasureTextEx(start, leaderboardText, 30, 2),
         30
      };

      Rectangle exitRec = (Rectangle)
      {
         (screenWidth - MeasureText(exitText, 30)) / 2,
         400,
         MeasureTextEx(start, exitText, 30, 2),
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
       

      EndDrawing();

   }


   CloseWindow(); // necessary (?)
   return 0;
}   

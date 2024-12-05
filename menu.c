#include "raylib.h"
#include <stdio.h>

#define PLAY 1
#define LEADERBOARD 2
#define EXIT 3


int menu() // não recebe nenhum valor
{
   const int screenWidth = 1200;
   const int screenHeight = 600;

   Font honk = LoadFont("honk.tff");
   Font start = LoadFont("start.tff");

   FileExists("honk.tff"); // check honk existence
   FileExists("start.tff"); // check pressStart existence

   InitWindow(screenWidth, screenHeight, "INF MAN");

   // coordinate x first
   Vector2 titlePosition = (Vector2) {550, 80};
   Vector2 playPosition = (Vector2) {550, 200};
   Vector2 leaderboardPosition = (Vector2) {550, 300};
   Vector2 exitPosition = (Vector2) {550, 400};

   const char titleText[8] = {"INF MAN"}; 
   const char playText[5] = {"play"};
   const char leaderboardText[12] = {"leaderboard"};
   const char exitText[5] = {"exit"};

   SetTargetFPS(60);


   while(!WindowShouldClose())
   {
      BeginDrawing();

      ClearBackground(YELLOW);

      DrawTextEx(honk, titleText, titlePosition, 70, 3, RAYWHITE);
      DrawTextEx(start, playText, playPosition, 30, 2, RAYWHITE);
      DrawTextEx(start, leaderboardText, leaderboardPosition, 30, 2, RAYWHITE);
      DrawTextEx(start, exitText, exitPosition, 30, 2, RAYWHITE);

      /* Rectangle playRec = (Rectangle)
      {
         playPosition.x;
         playPosition.y;
         MeasureTextEx(start, playText, 30, 2);
         30;
      };

      Rectangle leaderboardRec = (Rectangle)
      {
         leaderboardPosition.x;
         leaderboardPosition.y;
         MeasureTextEx(start, leaderboardText, 30, 2);
         30;
      };

      Rectangle exitRec = (Rectangle)
      {
         exitPosition.x;
         exitPosition.y;
         MeasureTextEx(start, exitText, 30, 2);
         30;
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
         } */
       

      EndDrawing();

   }


   CloseWindow(); // necessary (?)
   return 0;
}   

#include "raylib.h"

#define PLAY 1
#define LEADERBOARD 2
#define EXIT 3
   
   
int menu(void) // não recebe nenhum valor
{
   const int screenWidth = 1200;
   const int screenHeight = 600;
   
   Font honk = LoadFont("honk.tff");
   Font start = LoadFont("start.tff");
   
   FileExists("honk.tff"); // check honk existence
   FileExists("start.tff"); // check pressStart existence
   
   InitWindow(screenWidth, screenHeight, "INF MAN");
   
   // coordinate x first
   Vector2 titlePosition = {550, 80};
   Vector2 playPosition = {550, 200};
   Vector2 leaderboardPosition = {550, 300};
   Vector2 exitPosition = {550, 400};
   
   const char titleText[9] = "INF MAN"; 
   const char playText[6] = "play";
   const char leaderboardText[12] = "leaderboard";
   const char exitText[5] = "exit";
   
   SetTargetFPS(60);

   
   while(!WindowShouldClose)
   {
      BeginDrawing();
      
      ClearBackground(YELLOW);
      
      DrawTextEx(honk, titleText, titlePosition, 70, 3, RAYWHITE);
      DrawTextEx(start, playText, playPosition, 30, 2, RAYWHITE);
      DrawTextEx(start, leaderboardText, leaderboardPosition, 30, 2, RAYWHITE);
      DrawTextEx(start, exitText, exitPosition, 30, 2, RAYWHITE);
      
      Rectangle playRec
      {
         playPosition.x;
         playPosition.y;
         MeasureTextEx(start, playText, 30, 2);
         30;
      };
      
      Rectangle leaderboardRec
      {
         leaderboardPosition.x;
         leaderboardPosition.y;
         MeasureTextEx(start, leaderboardText, 30, 2);
         30;
      };
      
      Rectangle exitRec
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
         }
      }
      
      EndDrawing();
      
   }
   
   CloseWindow(); // necessary (?)
   return 0;
}

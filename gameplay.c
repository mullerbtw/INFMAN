#include <stdbool.h>
#include "raylib.h"
#include "raymath.h"

int gameplay()
{
	const int screenWidth = 1200;
    	const int screenHeight = 600;
    
	InitWindow(screenWidth, screenHeight, "GAMEPLAY");
	Texture2D megaman = LoadTexture("charMoving.png");
    
    	unsigned numTiles = 3;
    
    	// int totalTileWidth = 74;
    
    	float megamanWidth = (float)megaman.width / numTiles;
    
	Rectangle frameRec = {0.0f, 0.0f, megamanWidth, (float)megaman.height};
    
	Vector2 megamanPos = {(screenWidth / 2.0f) - ((megaman.width / (float)numTiles) / 2), screenHeight / 2.0f};
    
	float megamanSpeed = 150;
    
    	SetTargetFPS(60);

	while(!WindowShouldClose())
	{
        Vector2 movement = {0, 0}; 
		
        BeginDrawing();

		ClearBackground(WHITE);

		DrawTextureRec(megaman, frameRec, megamanPos, WHITE); // background color is different from texture's white
        
        if(IsKeyDown(KEY_W))
        {
            movement.y += -1;
        }
        
        if(IsKeyDown(KEY_A))
        {
            movement.x += -1;
            // frameRec.x -= (float)megamanWidth;
            WaitTime(0.08);
        }
        
        if(IsKeyDown(KEY_S))
        {
            movement.y += 1;
        }
        
        if(IsKeyDown(KEY_D))
        {
            movement.x += 1;
            frameRec.x += megamanWidth;
            WaitTime(0.08);
        }
        
        Vector2 movementScaling = Vector2Scale(movement, GetFrameTime() * megamanSpeed); // vetor para incrementar posição
        
        megamanPos = Vector2Add(megamanPos, movementScaling); // adds standard vector to the moving one
        
		EndDrawing();
	}

	CloseWindow();

	return 0;
}

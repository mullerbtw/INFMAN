#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>

int gameplay()
{
	const int screenWidth = 1200;
    const int screenHeight = 600;
    
	InitWindow(screenWidth, screenHeight, "GAMEPLAY");
	Texture2D megaman = LoadTexture("basicCharMovement.jpg");

	// int frameWidth = megaman.width / 4;
    
	Rectangle frameRec = {0.0f, 0.0f, (float)megaman.width / 4, (float)megaman.height};

	Vector2 megamanPos = {(screenWidth / 2.0f) - ((255 / 4.0f) / 2), screenHeight / 2.0f};
	
    // Vector2 megamanPos = {screenWidth / 2, screenHeight / 2};

    SetTargetFPS(60);

	while(!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(WHITE);

		DrawTextureRec(megaman, frameRec, megamanPos, WHITE); // background color is different from texture's white

		// DrawTexture(megaman, screenWidth / 2 - megaman.width / 2, ScreenHeight / 2 - megaman.height / 2 - 40, RAYWHITE);

		EndDrawing();
	}

	CloseWindow();

	return 0;
    
}
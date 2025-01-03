#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MATRIXLINES 15
#define MATRIXCOLUMNS 200

typedef struct
{
    Texture2D texture;
    Vector2 movement;
    unsigned numTiles;
    float width;
    int speed;
    int gravity;
    Vector2 position;
    Rectangle frameRec;
} MEGAMAN;

typedef struct
{
    Texture2D texture;
} ENEMY;

typedef struct
{
    Texture2D texture;
} BACKGROUND;

typedef struct
{
    Texture2D texture;
} BOX;

typedef struct
{
    Texture2D texture;
} SPIKE;

int main()
{
    char matrix[MATRIXLINES][MATRIXCOLUMNS];
    FILE *fp;
    if((fp = fopen("matrix.txt", "r")) != NULL)
    {
        for (int l = 0; l < MATRIXLINES; l++)
        {
            for (int c = 0; c < MATRIXCOLUMNS; c++)
            {
                fscanf(fp, "%c", &matrix[l][c]);
                if (matrix[l][c] == '\n')
                    matrix[l][c] = ' ';
            }
        }
        fclose(fp);
    }
    
    const int screenWidth = 1200;
    const int screenHeight = 600;
    const int gameWidth = 6400;
    int screenFloor = (screenHeight / 2);
	int xStartingPosition = 30;
    bool isMegamanJumping = false;
    bool megamanHitSomething = false;
    unsigned frameDelay = 5;
	unsigned frameDelayCounter = 0;
    
    InitWindow(screenWidth, screenHeight, "GAMEPLAY");
    
    MEGAMAN megaman;
    megaman.texture = (Texture2D) LoadTexture("megamanWalking.png");
    megaman.movement = (Vector2) {0, 0};
    megaman.numTiles = (int) 3;
    megaman.width = (float) ((float) megaman.texture.width / (float) megaman.numTiles);
    megaman.speed = 5;
    megaman.gravity = 1;
    megaman.position = (Vector2) {xStartingPosition,  screenFloor - megaman.texture.height}; // or fill with zeros in position
    megaman.frameRec = (Rectangle) {0.0f, 0.0f, megaman.width, (float) megaman.texture.height};
    ENEMY bomb;
    bomb.texture = LoadTexture("bomb.png");
    BACKGROUND background;
    background.texture = LoadTexture("background.png");
    BOX floor;
    floor.texture = LoadTexture("box.png");
    SPIKE spike;
    spike.texture = LoadTexture("spike.png");
    Camera2D camera;
    camera.offset = (Vector2) {0, 0};
    camera.target = (Vector2) {megaman.position.x, 0};
    camera.rotation = 0;
    camera.zoom = 1.0;
    
    SetTargetFPS(60);
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
		ClearBackground(WHITE);
        BeginMode2D(camera);
        
        for (int i = 0; i <= 9 ; i++)
        {
            DrawTexture(background.texture, i * background.texture.width, 0.0f, WHITE);
        }
        
        for (int l = 0; l < MATRIXLINES; l++)
        {
            for (int c = 0; c < MATRIXCOLUMNS; c++)
            {
                if (matrix[l][c] == 'P')
                {                
                    // megaman.position = (Vector2)
                }               
                if (matrix[l][c] == 'B')
                {
                    DrawTexture(floor.texture, (c * floor.texture.width), (l * floor.texture.height), WHITE);
                }
                if (matrix[l][c] == 'S')
                {                
                    DrawTexture(spike.texture, (c * spike.texture.width), (l * spike.texture.height), WHITE);
                }   
                if (matrix[l][c] == 'E')
                {                
                    DrawTexture(bomb.texture, (c * bomb.texture.width), (l * bomb.texture.height), WHITE);
                }
            }
        }
        
        if ((megaman.position.x + megaman.texture.width / 3) == screenFloor) // we need to update the screen floor in order to be the tile under character
            
        if (IsKeyPressed(KEY_W) && (!isMegamanJumping))
        {   
            megaman.movement.y = -4 * megaman.speed + megaman.gravity;
            DrawTextureRec(megaman.texture, megaman.frameRec, megaman.position, WHITE);
        }
        
        else if (IsKeyDown(KEY_A) && ((megaman.position.y - megaman.texture.height) >= screenFloor))
        {
            isMegamanJumping = false;
            DrawTextureRec(megaman.texture, megaman.frameRec, megaman.position, WHITE);
            megaman.movement.x = -megaman.speed;
            if (megaman.frameRec.width > 0)
                megaman.frameRec.width = -megaman.frameRec.width;
            frameDelayCounter++;
            if (frameDelayCounter >= frameDelay)
            {
                megaman.frameRec.x += megaman.width;
                frameDelayCounter = 0;
            }
        }
        
        else if (IsKeyDown(KEY_D) && ((megaman.position.y - megaman.texture.height) >= screenFloor))
        {
            isMegamanJumping = false;
            DrawTextureRec(megaman.texture, megaman.frameRec, megaman.position, WHITE);
            megaman.movement.x = megaman.speed;
            if (megaman.frameRec.width < 0)
                megaman.frameRec.width = -megaman.frameRec.width;
            frameDelayCounter++;
            if (frameDelayCounter >= frameDelay)
            {
                megaman.frameRec.x += megaman.width;
                frameDelayCounter = 0;
            }
        }
            
        else if ((megaman.position.y - megaman.texture.height) >= screenFloor)
        {
            isMegamanJumping = false;
            DrawTextureRec(megaman.texture, megaman.frameRec, megaman.position, WHITE);
            megaman.movement.x = 0;
            if (megaman.frameRec.width > 0)
                megaman.frameRec.width = +megaman.frameRec.width;
        }
        
        else
        {
            isMegamanJumping = true;
            DrawTextureRec(megaman.texture, megaman.frameRec, megaman.position, WHITE);

        }
        
        megaman.position = Vector2Add(megaman.position, megaman.movement);
        megaman.movement.y += megaman.gravity;

        // floor correction and movement stop
        if ((megaman.position.y - megaman.texture.height) >= screenFloor) // if megaman is on the floor or under
        {
            megaman.movement.y = 0;
            megaman.position.y = screenFloor + megaman.texture.height; // corrects position to floor
        }
        
        if (megaman.position.x < xStartingPosition - (megaman.texture.width / 6))
        {
            megaman.position.x = xStartingPosition - (megaman.texture.width / 6);
        }
        
        if (megaman.position.x > gameWidth - (megaman.texture.width / 3))
        {
            megaman.position.x = gameWidth - (megaman.texture.width / 3);
        }
        
        if (megaman.position.x <= screenWidth / 2)
        {
            camera.target.x = 0;
            camera.offset.x = 0;
        }

        if (megaman.position.x >= screenWidth / 2)
        {
            camera.target.x = megaman.position.x;
            camera.offset.x = screenWidth / 2 - ((float) megaman.texture.width / 6);
        }
        
        if (megaman.position.x >= (gameWidth - (screenWidth / 2) - (megaman.texture.width / 6)))
        {
            camera.target.x = gameWidth - (screenWidth / 2) - (megaman.texture.width / 6);
        }
        
        EndDrawing();
    }
    
    CloseWindow();

	return 0;
}

#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct
{
    Rectangle frameRec;
    Vector2 position;
    Vector2 movement;
    Texture2D texture;
    unsigned numTiles;
    float width;
    int speed;
    int gravity;
} MEGAMAN;

typedef struct
{
    Rectangle frameRec;
    Vector2 position;
    Texture2D texture;
} ENEMY;

typedef struct
{
    Vector2 position;
    Texture2D texture;
} BOX;

typedef struct
{
    Rectangle frameRec;
    Vector2 position;
    Texture2D texture;
} BACKGROUND;

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 600;
    const int gameWidth = 5432;
    const int screenFloor = (2 * screenHeight) / 3; // makes floor 2/3 of the screen height
	const int xStartingPosition = 30;
    
    bool isMegamanJumping = false;
    
    InitWindow(screenWidth, screenHeight, "GAMEPLAY"); // inicializa janela
    
    unsigned frameDelay = 5;
	unsigned frameDelayCounter = 0;
    
    MEGAMAN megaman;
    megaman.texture = (Texture2D) LoadTexture("megamanWalking.png");
    megaman.movement = (Vector2) {0, 0};
    megaman.numTiles = (int) 3;
    megaman.width = (float) ((float) megaman.texture.width / (float) megaman.numTiles);
    megaman.speed = 5;
    megaman.gravity = 1;
    megaman.position = (Vector2) {xStartingPosition,  screenFloor - megaman.texture.height};
    megaman.frameRec = (Rectangle) {0.0f, 0.0f, megaman.width, (float) megaman.texture.height};
    
    ENEMY bomb;
    bomb.texture = (Texture2D) LoadTexture("bomb.png");
    bomb.position = (Vector2) {2 * screenWidth / 3.0f, screenFloor - bomb.texture.height};
    bomb.frameRec = (Rectangle) {0.0f, 0.0f, (float) bomb.texture.width, (float) bomb.texture.height};
    
    BOX floor;
    floor.texture = LoadTexture("box.png");
    floor.position = (Vector2) {0.0f, screenFloor};
    
    BACKGROUND background;
    background.texture = LoadTexture("background.png");
    
    Camera2D camera;
    camera.offset = (Vector2) {0, 0};
    camera.target = (Vector2) {megaman.position.x, 0};
    camera.rotation = 0;
    camera.zoom = 1.0;
    
    SetTargetFPS(60); // setando fps da janela do jogo
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
		ClearBackground(WHITE);
        
        BeginMode2D(camera);
        
        for (int i = 0; i < 8; i++)
        {
            DrawTexture(background.texture, i * background.texture.width, 0.0f, WHITE);
        }
        
        if (IsKeyPressed(KEY_W) && (!isMegamanJumping)) // se W estiver apertado, pula
        {   
            megaman.movement.y = -4 * megaman.speed + megaman.gravity; // adds jumps with gravity
            DrawTextureRec(megaman.texture, megaman.frameRec, megaman.position, WHITE); // draws texture
        }
        
        else if (IsKeyDown(KEY_A) && ((megaman.position.y - megaman.texture.height) >= screenFloor)) // se A estiver apertado e estiver no chão, vai pra trás
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
            
        else if (IsKeyDown(KEY_D) && ((megaman.position.y - megaman.texture.height) >= screenFloor)) // se D estiver apertado e estiver no chão, vai pra frente
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
            DrawTextureRec(megaman.texture, megaman.frameRec, megaman.position, WHITE); // if megaman is in air
        }
        
        // esses steps são necessários pra fazer ele andar
        megaman.position = Vector2Add(megaman.position, megaman.movement); // adds standard vector to the moving one
        megaman.movement.y += megaman.gravity; // adds gravity to the movement vector

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
        
        DrawTextureRec(bomb.texture, bomb.frameRec, bomb.position, WHITE); // possibly has to go up
        
        if (megaman.position.x - bomb.position.x < 100)
        {
            // inimigo a direita do megaman
            if (bomb.position.x > megaman.position.x)
            {
                bomb.position.x = bomb.position.x - 1;
            }

            // inimigo a esquerda do megaman
            if (bomb.position.x < megaman.position.x)
            {
                bomb.position.x = bomb.position.x + 1;
            }

            // above or below
            // inimigo a direita do megaman
            if (bomb.position.y > megaman.position.y)
            {
                bomb.position.y = bomb.position.y - 1;
            }

            // inimigo a esquerda do megaman
            if (bomb.position.y < megaman.position.y)
            {
                bomb.position.y = bomb.position.y + 1;
            }

            // COLISAO COM INIMIGO
            if (fabs(bomb.position.x - megaman.position.x) < 10.0f)
            {
                if (fabs(bomb.position.y - megaman.position.y) < 10.0f)
                {
                    DrawText("COLLISION DETECTED", screenWidth / 2 - (MeasureText("COLLISION DETECTED", 20) / 2), screenHeight / 2, 20, RED);
                    return 0;
                }
            }
        }
        
        if (megaman.position.x <= screenWidth / 2)
        {
            camera.target.x = 0;
            camera.offset.x = 0;
        }
        // else
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

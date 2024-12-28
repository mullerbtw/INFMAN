#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "raymath.h"

int main()
{
    const int screenWidth = 1200; // inicializa largura da janela
    const int screenHeight = 600; // iniciliza altura da janela

    InitWindow(screenWidth, screenHeight, "GAMEPLAY"); // inicializa janela

    // secondary tile initialization
    Texture2D background = LoadTexture("background.png");
    Texture2D bombLeft = LoadTexture("bombLeft.png");
    Texture2D bombRight = LoadTexture("bombRight.png");
    Texture2D box = LoadTexture("box.png");

    // megaman tile initialization
    Texture2D megamanClimbing = LoadTexture("megamanClimbing.png");
    Texture2D megamanDying = LoadTexture("megamanDying.png");
    Texture2D megamanHurting = LoadTexture("megamanHurting.png");
    Texture2D megamanJumping = LoadTexture("megamanJumping.png");
    Texture2D megamanShootingMoving = LoadTexture("megamanShootingMoving.png");
    Texture2D megamanShootingStationary = LoadTexture("megamanShootingStationary.png");
    Texture2D megamanStill = LoadTexture("megamanStill.png");
    Texture2D megamanTeleporting = LoadTexture("megamanTeleporting.png");
    Texture2D megamanWalking = LoadTexture("megamanWalking.png"); // carrega textura de movimento do megaman

    int keyDown = 0;

    // variáveis pra armazenar quantas figuras tem em cada tile
    unsigned climbingNumTiles = 3;
    unsigned dyingNumTiles = 3;
    unsigned hurtingNumTiles = 1;
    unsigned jumpingNumTiles = 1;
    unsigned shootingMovingNumTiles = 3;
    unsigned shootingStationaryNumTiles = 1;
    unsigned stillNumTiles = 2;
    unsigned teleportingNumTiles = 2; // but vertically
    unsigned walkingNumTiles = 3;

    // variables related to state of the character
    bool megamanMovingRight = false;
    bool megamanMovingLeft = false;

    // variáveis pra armazenar largura de cara tiles do desenhos do megaman
    float climbingWidth = (float)megamanClimbing.width / climbingNumTiles;
    float dyingWidth = (float)megamanDying.width / dyingNumTiles;
    float hurtingWidth = (float)megamanHurting.width / hurtingNumTiles;
    float jumpingWidth = (float)megamanJumping.width / jumpingNumTiles;
    float shootingMovingWidth = (float)megamanShootingMoving.width / shootingMovingNumTiles;
    float shootingStationaryWidth = (float)megamanShootingStationary.width / shootingStationaryNumTiles;
    float stillWidth = (float)megamanStill.width / stillNumTiles;
    float teleportingWidth = (float)megamanTeleporting.width / teleportingNumTiles;
    float walkingWidth = (float)megamanWalking.width / walkingNumTiles;

    float megamanSpeed = 100; // variável que armazena velocidade (em pixels/second)

    // creating megaman texture rectangles
    Rectangle climbingFrameRec = {0.0f, 0.0f, climbingWidth, (float)megamanClimbing.height};
    Rectangle dyingFrameRec = {0.0f, 0.0f, dyingWidth, (float)megamanDying.height}; // adjust for vertically
    Rectangle hurtingFrameRec = {0.0f, 0.0f, hurtingWidth, (float)megamanHurting.height};
    Rectangle jumpingFrameRec = {0.0f, 0.0f, jumpingWidth, (float)megamanJumping.height};
    Rectangle shootingMovingFrameRec = {0.0f, 0.0f, shootingMovingWidth, (float)megamanShootingMoving.height};
    Rectangle shootingStationaryFrameRec = {0.0f, 0.0f, shootingStationaryWidth, (float)megamanShootingStationary.height};
    Rectangle stillFrameRec = {0.0f, 0.0f, stillWidth, (float)megamanStill.height};
    Rectangle teleportingFrameRec = {0.0f, 0.0f, teleportingWidth, (float)megamanTeleporting.height};
    Rectangle walkingFrameRec = {0.0f, 0.0f, walkingWidth, (float)megamanWalking.height};

    // creating secondary texture rectangles
    Rectangle backgroundFrameRec = {0.0f, 0.0f, (float)background.width, (float)background.height};
    Rectangle bombLeftFrameRec = {0.0f, 0.0f, (float)bombLeft.width, (float)bombLeft.height};
    Rectangle bombRightFrameRec = {0.0f, 0.0f, (float)bombRight.width, (float)bombRight.height};
    Rectangle boxrameRec = {0.0f, 0.0f, (float)box.width, (float)box.height};

    Vector2 megamanPos = {(screenWidth / 2.0f) - ((walkingWidth) / 2), screenHeight / 2.0f}; // vetor para posição do megaman

    double frameDelay = 0.1;

    SetTargetFPS(60); // setando fps da janela do jogo

    while(!WindowShouldClose())
    {
        Vector2 movement = {0, 0};  // vetor de movimento (necessário pra fazer ele andar)

        BeginDrawing();

        ClearBackground(WHITE);

        // DrawTextureRec(megamanStill, stillFrameRec, megamanPos, WHITE); // desenha megamanWalking
        
        if(IsKeyDown(KEY_A) && IsKeyDown(KEY_W))
        {
            DrawTextureRec(megamanJumping, jumpingFrameRec, megamanPos, WHITE);
            movement.y += -1;
            movement.x += -1;
        }
        
        else if(IsKeyDown(KEY_D) && IsKeyDown(KEY_W))
        {
            DrawTextureRec(megamanJumping, jumpingFrameRec, megamanPos, WHITE);
            movement.y += -1;
            movement.x += 1;
        }
        
        else if(IsKeyDown(KEY_A) && IsKeyDown(KEY_S))
        {
            DrawTextureRec(megamanJumping, jumpingFrameRec, megamanPos, WHITE);
            movement.y += 1;
            movement.x += -1;
        }
        
        else if(IsKeyDown(KEY_D) && IsKeyDown(KEY_S))
        {
            DrawTextureRec(megamanJumping, jumpingFrameRec, megamanPos, WHITE);
            movement.y += 1;
            movement.x += 1;
        }
        
        else if(IsKeyDown(KEY_A) && IsKeyDown(KEY_C))
        {
            DrawTextureRec(megamanShootingMoving, shootingMovingFrameRec, megamanPos, WHITE);
            if(shootingMovingFrameRec.width > 0)
                shootingMovingFrameRec.width = -shootingMovingFrameRec.width;
            movement.x += -1;
            shootingMovingFrameRec.x += shootingMovingWidth;
            WaitTime(frameDelay);
        }
        
        else if(IsKeyDown(KEY_D) && IsKeyDown(KEY_C))
        {
            DrawTextureRec(megamanShootingMoving, shootingMovingFrameRec, megamanPos, WHITE);
            if(shootingMovingFrameRec.width < 0)
                shootingMovingFrameRec.width = -shootingMovingFrameRec.width;
            movement.x += 1;
            shootingMovingFrameRec.x += shootingMovingWidth;
            WaitTime(frameDelay);
        }
        
        else if(IsKeyDown(KEY_W)) // se W estiver apertado, vai pra cima
        {
            DrawTextureRec(megamanJumping, jumpingFrameRec, megamanPos, WHITE);
            movement.y += -1;
        }
        
        else if(IsKeyDown(KEY_A)) // se A estiver apertado, vai pra trás
        {
            DrawTextureRec(megamanWalking, walkingFrameRec, megamanPos, WHITE);
            if(walkingFrameRec.width > 0)
                walkingFrameRec.width = -walkingFrameRec.width;
            movement.x += -1;
            walkingFrameRec.x += walkingWidth;
            WaitTime(frameDelay);
        }
        
        else if(IsKeyDown(KEY_S)) // se S estiver apertado, vai pra baixo
        {
            DrawTextureRec(megamanJumping, jumpingFrameRec, megamanPos, WHITE);
            movement.y += 1;
        }
        
        else if(IsKeyDown(KEY_D)) // se D estiver apertado, vai pra frente
        {
            DrawTextureRec(megamanWalking, walkingFrameRec, megamanPos, WHITE);
            if(walkingFrameRec.width < 0)
                walkingFrameRec.width = -walkingFrameRec.width;
            movement.x += 1;
            walkingFrameRec.x += walkingWidth;
            WaitTime(frameDelay);
        }
        
        else if(IsKeyDown(KEY_C))
        {
            DrawTextureRec(megamanShootingStationary, shootingStationaryFrameRec, megamanPos, WHITE);
        }
        
        else
        {
            DrawTextureRec(megamanStill, stillFrameRec, megamanPos, WHITE);
            stillFrameRec.x += stillWidth;
            WaitTime(frameDelay);
        }

        // esses steps são necessários pra fazer ele andar
        Vector2 movementScaling = Vector2Scale(movement, GetFrameTime() * megamanSpeed); // vetor para incrementar posição
        megamanPos = Vector2Add(megamanPos, movementScaling); // adds standard vector to the moving one

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

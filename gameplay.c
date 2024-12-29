#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//estrutura inimigo
typedef struct{
    Vector2 position; // posição atual do inimigo
    // float speed; // velocidade do inimigo
    Texture2D texture; // textura do inimigo
} ENEMY;

int main()
{
    //instancia inimigo
    
    const int screenWidth = 1200; // inicializa largura da janela
    const int screenHeight = 600; // inicializa altura da janela

    const int megamanSpeed = 5; // variável que armazena velocidade (em pixels/second)
    const int megamanGravity = 1; // variável que armazena aceleração

    const int screenFloor = (2 * screenHeight) / 3; // makes floor 2/3 of the screen height

    InitWindow(screenWidth, screenHeight, "GAMEPLAY"); // inicializa janela
    InitAudioDevice(); //precisa pra tocar audio
    
    // secondary tile initialization
    Texture2D background = LoadTexture("background.png");
    Texture2D bombLeft = LoadTexture("bombLeft.png");
    Texture2D bombRight = LoadTexture("bombRight.png");
    Texture2D box = LoadTexture("box.png");
    
    //sons
    Sound dor = LoadSound("manDeathScream.ogg");
    SetSoundVolume(dor, 10.0f);
    

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
	
	// for movement
	unsigned frameDelay = 5;
	unsigned frameDelayCounter = 0;
	unsigned frameIndex = 0;
	
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
    Rectangle boxFrameRec = {0.0f, 0.0f, (float)box.width, (float)box.height};

    Vector2 megamanPos = {(screenWidth / 2.0f) - ((walkingWidth) / 2), screenHeight / 2.0f}; // vetor para posição do megaman
    Vector2 megamanMovement = {0, 0};  // vetor de movimento (necessário pra fazer ele andar)
    
    // enemy struct declaration
    ENEMY enemy;
        
    enemy.position = (Vector2){100.0f, 300.0f}; // posição inicial do inimigo
    //enemy.speed = 2.0f; // velocidade do inimigo
    enemy.texture = bombLeft;   
   
    bool soundPlayed = false;

    SetTargetFPS(60); // setando fps da janela do jogo

    while(!WindowShouldClose())
    {
        
        // se megaman estiver perto do inimigo, ele começa a perseguir o megaman
        
        if (megamanPos.x - enemy.position.x < 100)
        {
            enemy.texture = bombLeft; 
            
            //inimigo a direita do megaman
            if (enemy.position.x > megamanPos.x )
            {
                enemy.position.x = enemy.position.x - 1;
            }
            
            // inimigo a esquerda do megaman
            if (enemy.position.x < megamanPos.x)
            {
                enemy.texture = bombRight;
                enemy.position.x = enemy.position.x + 1;
            }
            
            //acima ou abaixo
        
            //inimigo a direita do megaman
            if (enemy.position.y > megamanPos.y)
            {
                enemy.position.y = enemy.position.y - 1;    
            }
            
            //inimigo a esquerda do megaman
            if (enemy.position.y < megamanPos.y )
            {
                enemy.position.y = enemy.position.y + 1;    
            }
       
            if (fabs(enemy.position.x - megamanPos.x) < 10.0f)
            {
                PlaySound(dor);  // toca o som
            }
        }
        
        BeginDrawing();
        
        ClearBackground(WHITE);
        
        DrawTexture(enemy.texture, enemy.position.x, enemy.position.y, WHITE); // needs to be DrawTextureRec to facilitate collision recognition

        // if(IsKeyDown(KEY_A) && IsKeyDown(KEY_W))
        // {
        //     DrawTextureRec(megamanJumping, jumpingFrameRec, megamanPos, WHITE);
        //     megamanMovement.y = -megamanSpeed;
        //     megamanMovement.x = -megamanSpeed;
        // }
        
        // else if(IsKeyDown(KEY_D) && IsKeyDown(KEY_W))
        // {
        //     DrawTextureRec(megamanJumping, jumpingFrameRec, megamanPos, WHITE);
        //     megamanMovement.y = -megamanSpeed;
        //     megamanMovement.x = megamanSpeed;
        // }
        
        // else if(IsKeyDown(KEY_A) && IsKeyDown(KEY_S))
        // {
        //     DrawTextureRec(megamanJumping, jumpingFrameRec, megamanPos, WHITE);
        //     megamanMovement.y = megamanSpeed;
        //     megamanMovement.x += -megamanSpeed;
        // }
        
        // else if(IsKeyDown(KEY_D) && IsKeyDown(KEY_S))
        // {
        //     DrawTextureRec(megamanJumping, jumpingFrameRec, megamanPos, WHITE);
        //     megamanMovement.y = megamanSpeed;
        //     megamanMovement.x = megamanSpeed;
        // }
        
        if(IsKeyDown(KEY_A) && IsKeyDown(KEY_C)) // moves to right with shooting position
        {
            DrawTextureRec(megamanShootingMoving, shootingMovingFrameRec, megamanPos, WHITE);
            if(shootingMovingFrameRec.width > 0)
                shootingMovingFrameRec.width = -shootingMovingFrameRec.width;
            megamanMovement.x = -megamanSpeed;
            frameDelayCounter++;
            if(frameDelayCounter >= frameDelay)
            {
            	shootingMovingFrameRec.x += shootingMovingWidth;
            	frameDelayCounter = 0;
            }
        }
        
        else if(IsKeyDown(KEY_D) && IsKeyDown(KEY_C)) // moves right with shooting position
        {
            DrawTextureRec(megamanShootingMoving, shootingMovingFrameRec, megamanPos, WHITE);
            if(shootingMovingFrameRec.width < 0)
                shootingMovingFrameRec.width = -shootingMovingFrameRec.width;
            megamanMovement.x = megamanSpeed;
            frameDelayCounter++;
            if(frameDelayCounter >= frameDelay)
            {
            	shootingMovingFrameRec.x += shootingMovingWidth;
            	frameDelayCounter = 0;
            }
        }
        
        else if(IsKeyPressed(KEY_W)) // se W estiver apertado, pula
        {
            megamanMovement.y = - 4 * megamanSpeed + megamanGravity;
            DrawTextureRec(megamanJumping, jumpingFrameRec, megamanPos, WHITE);
        }

        else if(IsKeyDown(KEY_A) && ((megamanPos.y - megamanWalking.height) >= screenFloor)) // se A estiver apertado e estiver no chão, vai pra trás
        {
            DrawTextureRec(megamanWalking, walkingFrameRec, megamanPos, WHITE);
            if(walkingFrameRec.width > 0)
                walkingFrameRec.width = -walkingFrameRec.width;
            megamanMovement.x = - megamanSpeed;
            frameDelayCounter++;
            if(frameDelayCounter >= frameDelay)
            {
            	walkingFrameRec.x += walkingWidth;
            	frameDelayCounter = 0;
            }
        }

        // else if(IsKeyDown(KEY_S)) // se S estiver apertado, vai pra baixo
        // {
        //     DrawTextureRec(megamanJumping, jumpingFrameRec, megamanPos, WHITE);
        //     megamanMovement.y = megamanSpeed;
        // }

        else if(IsKeyDown(KEY_D) && ((megamanPos.y - megamanWalking.height) >= screenFloor)) // se D estiver apertado e estiver no chão, vai pra frente
        {
            DrawTextureRec(megamanWalking, walkingFrameRec, megamanPos, WHITE);
            if(walkingFrameRec.width < 0)
                walkingFrameRec.width = -walkingFrameRec.width;
            megamanMovement.x = megamanSpeed;
            frameDelayCounter++;
            if(frameDelayCounter >= frameDelay)
            {
            	walkingFrameRec.x += walkingWidth;
            	frameDelayCounter = 0;
            }
        }

        else if(IsKeyDown(KEY_C) || (IsKeyDown(KEY_C) && ((megamanPos.y - megamanWalking.height) >= screenFloor)))
        {
            DrawTextureRec(megamanShootingStationary, shootingStationaryFrameRec, megamanPos, WHITE);
            // if(shootingMovingFrameRec.width > 0)
            // shootingMovingFrameRec.width = -shootingMovingFrameRec.width;
        }
        
        else if((megamanPos.y - megamanWalking.height) >= screenFloor)
        {
            DrawTextureRec(megamanStill, stillFrameRec, megamanPos, WHITE);
            megamanMovement.x = 0;
            frameDelayCounter++;
            if(frameDelayCounter >= frameDelay)
            {
            	stillFrameRec.x += stillWidth;
            	frameDelayCounter = 0;
            }
        }

        // esses steps são necessários pra fazer ele andar
        megamanPos = Vector2Add(megamanPos, megamanMovement); // adds standard vector to the moving one
        megamanMovement.y += megamanGravity; // adds gravity to the movement vector
        
        if((megamanPos.y - megamanWalking.height) >= screenFloor)
        {
            megamanMovement.y = 0;
            // megamanPos.y = screenFloor;
        }
        else if(!IsKeyDown(KEY_C))
        {
            DrawTextureRec(megamanJumping, jumpingFrameRec, megamanPos, WHITE);
        }
        
        EndDrawing();
    }
    
    UnloadSound(dor);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

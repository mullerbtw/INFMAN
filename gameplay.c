#include <stdbool.h>
#include "raylib.h"
#include "raymath.h"

int main()
{
	const int screenWidth = 1200; // inicializa largura da janela
    	const int screenHeight = 600; // iniciliza altura da janela
    
	InitWindow(screenWidth, screenHeight, "GAMEPLAY"); // iniciliza janela
	Texture2D megaman = LoadTexture("charMoving.png"); // carrega textura de movimento do megaman
    
    	unsigned numTiles = 3; // variável para armazenar número de figuras do megaman na charMoving.png
    
    	// bool inverted = false;
    
    	float oneWidth = (float)megaman.width / numTiles; // variável para armazenar largura de um dos três desenhos
        
	Rectangle frameRec = {0.0f, 0.0f, oneWidth, (float)megaman.height}; // cria retângulo onde fica megaman
	Vector2 megamanPos = {(screenWidth / 2.0f) - ((oneWidth) / 2), screenHeight / 2.0f}; // vetor para posição do megaman
    
	float megamanSpeed = 100; // variável que armazena velocidade (em pixels/second)
    
    	SetTargetFPS(60); // setando fps da janela do jogo

	while(!WindowShouldClose())
	{
        	Vector2 movement = {0, 0};  // vetor de movimento (necessário pra fazer ele andar)
		
        	BeginDrawing();

		ClearBackground(WHITE);

		DrawTextureRec(megaman, frameRec, megamanPos, WHITE); // desenha textura do megaman
        
        	if(IsKeyDown(KEY_W)) // se W estiver apertado, vai pra cima
        	{
            		movement.y += -1;
        	}
        
        	if(IsKeyDown(KEY_A)) // se A estiver apertado, vai pra trás
        	{   
            		// inverted = true;
            		// if(inverted == true)
            		megaman.width = -megaman.width;
            		movement.x += -1;
            		frameRec.x += oneWidth;
            		WaitTime(0.1);
        	}
        
        	if(IsKeyDown(KEY_S)) // se S estiver apertado, vai pra baixo
        	{
            		movement.y += 1;
        	}
        
        	if(IsKeyDown(KEY_D)) // se D estiver apertado, vai pra frente
        	{
            		// inverted = false;
            		// if(inverted == false)
            		megaman.width = megaman.width;
            		movement.x += 1;
            		frameRec.x += oneWidth;
            		WaitTime(0.1);
        	}
        
        	// esses steps são necessários pra fazer ele andar
        	Vector2 movementScaling = Vector2Scale(movement, GetFrameTime() * megamanSpeed); // vetor para incrementar posição
        	megamanPos = Vector2Add(megamanPos, movementScaling); // adds standard vector to the moving one
        
		EndDrawing();
	}

	CloseWindow();

	return 0;
}

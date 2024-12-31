#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//estrutura inimigo
typedef struct
{
	Vector2 position;  // posição atual do inimigo
	Texture2D texture; // textura do inimigo
} ENEMY;

typedef struct
{
	Vector2 position;
	Texture2D texture;
} TIRO;

typedef struct
{
	Vector2 position;
	Texture2D texture;
} COLISAO;

#define MAP_WIDTH 39
#define MAP_HEIGHT 19
#define TILE_SIZE 16

int TileMap[MAP_HEIGHT][MAP_WIDTH] =
{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void drawTileMap(int map[MAP_HEIGHT][MAP_WIDTH], Texture2D boxTexture, Texture2D spikeTexture)
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (map[y][x] == 1)
			{
				// se o tile for 1, ele considera como sólido. se for 0, não faz nada
				// função para desenhar os tiles. recebe: posição x e posição y
				DrawTexture(boxTexture, x * TILE_SIZE, y * TILE_SIZE, WHITE);
			}
			else if (map[y][x] == 2)
			{
				// se o tile for 2, ele considera como spike
				DrawTexture(spikeTexture, x * TILE_SIZE, y * TILE_SIZE, WHITE);
			}
		}
	}
}

void drawTiro(TIRO tiro, int correcaoX)
{
	DrawTexture(tiro.texture, tiro.position.x - correcaoX, tiro.position.y, WHITE);
}

void atualizaTiro(TIRO *tiro, Vector2 megamanPos)
{
	tiro->position.x = megamanPos.x;
	tiro->position.y = megamanPos.y;
}

void atualizaColisao(COLISAO *colisao, Vector2 megamanPos)
{
	colisao->position.x = megamanPos.x;
	colisao->position.y = megamanPos.y;
}

int main()
{
	bool isDeath = false;

	int correcaoX = 10;

	const int screenWidth = 1200 / 2; // inicializa largura da janela
	const int screenHeight = 600 / 2; // inicializa altura da janela

	const int megamanSpeed = 5;	  // variável que armazena velocidade (em pixels/second)
	const int megamanGravity = 1; // variável que armazena aceleração

	const int screenFloor = (2 * screenHeight) / 3; // makes floor 2/3 of the screen height

	InitWindow(screenWidth, screenHeight, "GAMEPLAY"); // inicializa janela
	InitAudioDevice();								   // precisa pra tocar audio

	// secondary tile initialization
	Texture2D backgroundTexture = LoadTexture("background.png");
	Texture2D bombLeftTexture = LoadTexture("bombLeft.png");
	Texture2D bombRightTexture = LoadTexture("bombRight.png");
	Texture2D boxTexture = LoadTexture("box.png");
	Texture2D spikeTexture = LoadTexture("spike.png");
	Texture2D tiroTexture = LoadTexture("thinLaser.png"); // carrega textura de movimento do megaman

	// sons
	Sound dorSound = LoadSound("ai.mp3");
	Sound tiroSound = LoadSound("tiro.mp3");
	Music musicaSound = LoadMusicStream("musica.mp3");

	// megaman tile initialization
	Texture2D megamanClimbingTexture = LoadTexture("megamanClimbing.png");
	Texture2D megamanDyingTexture = LoadTexture("megamanDying.png");
	Texture2D megamanHurtingTexture = LoadTexture("megamanHurting.png");
	Texture2D megamanJumpingTexture = LoadTexture("megamanJumping.png");
	Texture2D megamanShootingMovingTexture = LoadTexture("megamanShootingMoving.png");
	Texture2D megamanShootingStationaryTexture = LoadTexture("megamanShootingStationary.png");
	Texture2D megamanStillTexture = LoadTexture("megamanStill.png");
	Texture2D megamanTeleportingTexture = LoadTexture("megamanTeleporting.png");
	Texture2D megamanWalkingTexture = LoadTexture("megamanWalking.png");
	Texture2D colisaoTexture = LoadTexture("colisao.png");

	// for movement
	unsigned frameDelay = 5;
	unsigned frameDelayCounter = 0;

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

	// variáveis pra armazenar largura de cara tiles do desenhos do megaman
	float climbingWidth = (float)megamanClimbingTexture.width / climbingNumTiles;
	float dyingWidth = (float)megamanDyingTexture.width / dyingNumTiles;
	float hurtingWidth = (float)megamanHurtingTexture.width / hurtingNumTiles;
	float jumpingWidth = (float)megamanJumpingTexture.width / jumpingNumTiles;
	float shootingMovingWidth = (float)megamanShootingMovingTexture.width / shootingMovingNumTiles;
	float shootingStationaryWidth = (float)megamanShootingStationaryTexture.width / shootingStationaryNumTiles;
	float stillWidth = (float)megamanStillTexture.width / stillNumTiles;
	float teleportingWidth = (float)megamanTeleportingTexture.width / teleportingNumTiles;
	float walkingWidth = (float)megamanWalkingTexture.width / walkingNumTiles;

	// creating megaman texture rectangles
	Rectangle climbingFrameRec = {0.0f, 0.0f, climbingWidth, (float)megamanClimbingTexture.height};
	Rectangle dyingFrameRec = {0.0f, 0.0f, dyingWidth, (float)megamanDyingTexture.height}; // adjust for vertically
	Rectangle hurtingFrameRec = {0.0f, 0.0f, hurtingWidth, (float)megamanHurtingTexture.height};
	Rectangle jumpingFrameRec = {0.0f, 0.0f, jumpingWidth, (float)megamanJumpingTexture.height};
	Rectangle shootingMovingFrameRec = {0.0f, 0.0f, shootingMovingWidth, (float)megamanShootingMovingTexture.height};
	Rectangle shootingStationaryFrameRec = {0.0f, 0.0f, shootingStationaryWidth, (float)megamanShootingStationaryTexture.height};
	Rectangle stillFrameRec = {0.0f, 0.0f, stillWidth, (float)megamanStillTexture.height};
	Rectangle teleportingFrameRec = {0.0f, 0.0f, teleportingWidth, (float)megamanTeleportingTexture.height};
	Rectangle walkingFrameRec = {0.0f, 0.0f, walkingWidth, (float)megamanWalkingTexture.height};

	// creating secondary texture rectangles
	Rectangle backgroundFrameRec = {0.0f, 0.0f, (float)backgroundTexture.width, (float)backgroundTexture.height};
	Rectangle bombLeftFrameRec = {0.0f, 0.0f, (float)bombLeftTexture.width, (float)bombLeftTexture.height};
	Rectangle bombRightFrameRec = {0.0f, 0.0f, (float)bombRightTexture.width, (float)bombRightTexture.height};
	Rectangle boxFrameRec = {0.0f, 0.0f, (float)boxTexture.width, (float)boxTexture.height};
	Rectangle colisaoFrameRec = {0.0f, 0.0f, (float)colisaoTexture.width, (float)colisaoTexture.height};

	Vector2 megamanPos = {(screenWidth / 2.0f) - ((walkingWidth) / 2), screenHeight / 2.0f}; // vetor para posição do megaman
	Vector2 megamanMovement = {0, 0};														 // vetor de movimento (necessário pra fazer ele andar)

	TIRO tiro;

	tiro.position = (Vector2){megamanPos.x, megamanPos.y};
	tiro.texture = tiroTexture;

	ENEMY enemy;

	enemy.position = (Vector2){100.0f, 300.0f}; // posição inicial do inimigo
	//enemy.speed = 2.0f; // velocidade do inimigo
	enemy.texture = bombLeftTexture;

	COLISAO colisaoMan;

	colisaoMan.position = (Vector2){megamanPos.x, megamanPos.y};

	colisaoMan.texture = colisaoTexture;

	bool soundPlayed = false;
	bool playmusic = true;

	SetTargetFPS(60); // setando fps da janela do jogo

	PlayMusicStream(musicaSound);

	while (!WindowShouldClose())
	{
		UpdateMusicStream(musicaSound);

		if (!isDeath)
		{
			drawTileMap(TileMap, boxTexture, spikeTexture);

			DrawTextureRec(colisaoMan.texture, colisaoFrameRec, colisaoMan.position, WHITE);

			atualizaColisao(&colisaoMan, megamanPos);

			// se megaman estiver perto do inimigo, ele começa a perseguir o megaman

			if (megamanPos.x - enemy.position.x < 100)
			{
				enemy.texture = bombLeftTexture;

				// inimigo a direita do megaman
				if (enemy.position.x > megamanPos.x)
				{
					enemy.position.x = enemy.position.x - 1;
				}

				// inimigo a esquerda do megaman
				if (enemy.position.x < megamanPos.x)
				{
					enemy.texture = bombRightTexture;
					enemy.position.x = enemy.position.x + 1;
				}

				// acima ou abaixo

				//inimigo a direita do megaman
				if (enemy.position.y > megamanPos.y)
				{
					enemy.position.y = enemy.position.y - 1;
				}

				// inimigo a esquerda do megaman
				if (enemy.position.y < megamanPos.y)
				{
					enemy.position.y = enemy.position.y + 1;
				}

				// COLISAO COM INIMIGO
				if (fabs(enemy.position.x - megamanPos.x) < 10.0f)
				{
					if (fabs(enemy.position.y - megamanPos.y) < 10.0f)
						PlaySound(dorSound);

					// StopMusicStream(musicaSound);

					playmusic = false;
				}
			}

			BeginDrawing();

			ClearBackground(WHITE);

			DrawTexture(enemy.texture, enemy.position.x, enemy.position.y, WHITE); // needs to be DrawTextureRec to facilitate collision recognition

			if (IsKeyPressed(KEY_C))
			{
				// printf("tiro");
				PlaySound(tiroSound);
			}

			if (IsKeyDown(KEY_A) && IsKeyDown(KEY_C)) // moves to right with shooting position
			{
				// precisa pra corrigir a posicao do tiro
				correcaoX = 1200 - 50;
				// precisa pra atualizar a posição de aparecer do tiro
				atualizaTiro(&tiro, megamanPos);
				drawTiro(tiro, correcaoX);

				DrawTextureRec(megamanShootingMovingTexture, shootingMovingFrameRec, megamanPos, WHITE);
				if (shootingMovingFrameRec.width > 0)
					shootingMovingFrameRec.width = -shootingMovingFrameRec.width;
				megamanMovement.x = -megamanSpeed;
				frameDelayCounter++;
				if (frameDelayCounter >= frameDelay)
				{
					shootingMovingFrameRec.x += shootingMovingWidth;
					frameDelayCounter = 0;
				}
			}

			else if (IsKeyDown(KEY_D) && IsKeyDown(KEY_C)) // moves right with shooting position
			{
				correcaoX = -30;
				atualizaTiro(&tiro, megamanPos);
				drawTiro(tiro, correcaoX);

				DrawTextureRec(megamanShootingMovingTexture, shootingMovingFrameRec, megamanPos, WHITE);
				if (shootingMovingFrameRec.width < 0)
					shootingMovingFrameRec.width = -shootingMovingFrameRec.width;
				megamanMovement.x = megamanSpeed;
				frameDelayCounter++;
				if (frameDelayCounter >= frameDelay)
				{
					shootingMovingFrameRec.x += shootingMovingWidth;
					frameDelayCounter = 0;
				}
			}

			else if (IsKeyPressed(KEY_W)) // se W estiver apertado, pula
			{
				megamanMovement.y = -4 * megamanSpeed + megamanGravity;
				DrawTextureRec(megamanJumpingTexture, jumpingFrameRec, megamanPos, WHITE);
			}

			else if (IsKeyDown(KEY_A) && ((megamanPos.y - megamanWalkingTexture.height) >= screenFloor)) // se A estiver apertado e estiver no chão, vai pra trás
			{
				DrawTextureRec(megamanWalkingTexture, walkingFrameRec, megamanPos, WHITE);
				if (walkingFrameRec.width > 0)
					walkingFrameRec.width = -walkingFrameRec.width;
				megamanMovement.x = -megamanSpeed;
				frameDelayCounter++;
				if (frameDelayCounter >= frameDelay)
				{
					walkingFrameRec.x += walkingWidth;
					frameDelayCounter = 0;
				}
			}

			// else if(IsKeyDown(KEY_S)) // se S estiver apertado, vai pra baixo
			// {
			//     DrawTextureRec(megamanJumpingTexture, jumpingFrameRec, megamanPos, WHITE);
			//     megamanMovement.y = megamanSpeed;
			// }

			else if (IsKeyDown(KEY_D) && ((megamanPos.y - megamanWalkingTexture.height) >= screenFloor)) // se D estiver apertado e estiver no chão, vai pra frente
			{
				DrawTextureRec(megamanWalkingTexture, walkingFrameRec, megamanPos, WHITE);
				if (walkingFrameRec.width < 0)
					walkingFrameRec.width = -walkingFrameRec.width;
				megamanMovement.x = megamanSpeed;
				frameDelayCounter++;
				if (frameDelayCounter >= frameDelay)
				{
					walkingFrameRec.x += walkingWidth;
					frameDelayCounter = 0;
				}
			}

			else if (IsKeyDown(KEY_C) || (IsKeyDown(KEY_C) && ((megamanPos.y - megamanWalkingTexture.height) >= screenFloor)))
			{
				// precisa pra corrigir a posicao do tiro
				correcaoX = -30;
				// precisa pra atualizar a posição de aparecer do tiro
				atualizaTiro(&tiro, megamanPos);
				drawTiro(tiro, correcaoX);

				DrawTextureRec(megamanShootingStationaryTexture, shootingStationaryFrameRec, megamanPos, WHITE);
				// if(shootingMovingFrameRec.width > 0)
				// shootingMovingFrameRec.width = -shootingMovingFrameRec.width;
			}

			else if ((megamanPos.y - megamanWalkingTexture.height) >= screenFloor)
			{
				DrawTextureRec(megamanStillTexture, stillFrameRec, megamanPos, WHITE);
				megamanMovement.x = 0;
				frameDelayCounter++;
				if (frameDelayCounter >= frameDelay)
				{
					stillFrameRec.x += stillWidth;
					frameDelayCounter = 0;
				}
			}

			// esses steps são necessários pra fazer ele andar
			megamanPos = Vector2Add(megamanPos, megamanMovement); // adds standard vector to the moving one
			megamanMovement.y += megamanGravity;				  // adds gravity to the movement vector

			if ((megamanPos.y - megamanWalkingTexture.height) >= screenFloor)
			{
				megamanMovement.y = 0;
				// megamanPos.y = screenFloor;
			}
			else if (!IsKeyDown(KEY_C))
			{
				DrawTextureRec(megamanJumpingTexture, jumpingFrameRec, megamanPos, WHITE);
			}

			EndDrawing();
		}
		else if (isDeath == true)
		{
		}
	}

	UnloadMusicStream(musicaSound);
	UnloadSound(dorSound);
	CloseAudioDevice();
	CloseWindow();

	return 0;
}

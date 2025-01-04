#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MATRIXLINES 15
#define MATRIXCOLUMNS 200

const int screenWidth = 1200;
const int screenHeight = 600;
const int gameWidth = 6400;

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
	Vector2 position;
	Rectangle frameRec;
} ENEMY;
typedef struct
{
	Texture2D texture;
	Vector2 position;
	Rectangle frameRec;
} BOX;
typedef struct
{
	Texture2D texture;
} BACKGROUND;
typedef struct
{
	Texture2D texture;
} SPIKE;
typedef struct
{
	Texture2D texture;
	Vector2 position;
} TIRO;
void DrawTiro(TIRO tiro, int correcaoX)
{
	DrawTexture(tiro.texture, tiro.position.x - correcaoX, tiro.position.y, WHITE);
}
void AtualizaTiro(TIRO *tiro, Vector2 megamanPos)
{
	tiro->position.x = megamanPos.x;
	tiro->position.y = megamanPos.y;
}
void readMatrix(char matrix[][MATRIXCOLUMNS])
{
    FILE *fp;
	if ((fp = fopen("matrix.txt", "r")) != NULL)
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
}
void initMegamanPosition(char matrix[][MATRIXCOLUMNS], MEGAMAN *megaman)
{
    for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'P')
			{
				megaman->position = (Vector2){(c * megaman->texture.width), (l * megaman->texture.height)};
			}
		}
	}
}
void initParedePosition(char matrix[][MATRIXCOLUMNS], BOX *parede)
{
    for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'P')
			{
				parede->frameRec = (Rectangle){c * parede->texture.width, l * parede->texture.height, parede->texture.width, parede->texture.height};
			}
		}
	}
}
void initFloorPosition(char matrix[][MATRIXCOLUMNS], BOX *floor)
{
    for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'P')
			{
				floor->frameRec = (Rectangle){c * floor->texture.width, l * floor->texture.height, floor->texture.width, floor->texture.height};
			}
		}
	}
}
void initCameraSettings(Camera2D *camera, MEGAMAN *megaman)
{
    camera->offset = (Vector2){0, 0};
	camera->target = (Vector2){megaman->position.x, 0};
	camera->rotation = 0;
	camera->zoom = 1.0;
}
void drawBackground(BACKGROUND *background)
{
    for (int i = 0; i <= 9; i++)
    {
        DrawTexture(background->texture, i * background->texture.width, 0.0f, WHITE);
    }
}
int bombMovement(ENEMY *bomb, MEGAMAN *megaman)
{
    if (megaman->position.x - bomb->position.x < 100)
		{
			// inimigo a direita do megaman
			if (bomb->position.x > megaman->position.x)
			{
				bomb->texture = (Texture2D)LoadTexture("bomb.png");
				bomb->position.x = bomb->position.x - 1;
			}
			// inimigo a esquerda do megaman
			if (bomb->position.x < megaman->position.x)
			{
				bomb->texture = (Texture2D)LoadTexture("bombInvert.png");
				bomb->position.x = bomb->position.x + 1;
			}
			// above or below
			if (bomb->position.y > megaman->position.y)
			{
				bomb->position.y = bomb->position.y - 1;
			}
			if (bomb->position.y < megaman->position.y)
			{
				bomb->position.y = bomb->position.y + 1;
			}
			// colisão com inimigo
			if (fabs(bomb->position.x - megaman->position.x) < 10.0f)
			{
				if (fabs(bomb->position.y - megaman->position.y) < 10.0f)
				{
					DrawText("COLLISION DETECTED", (screenWidth / 2), (screenHeight / 2), 20, RED);
					return 0;
				}
			}
		}
}
void cameraUpdate(Camera2D *camera, MEGAMAN *megaman)
{
    if (megaman->position.x <= screenWidth / 2)
		{
			camera->target.x = 0;
			camera->offset.x = 0;
		}
		if (megaman->position.x >= screenWidth / 2)
		{
			camera->target.x = megaman->position.x;
			camera->offset.x = screenWidth / 2 - ((float)megaman->texture.width / 6);
		}
		if (megaman->position.x >= (gameWidth - (screenWidth / 2) - (megaman->texture.width / 6)))
		{
			camera->target.x = gameWidth - (screenWidth / 2) - (megaman->texture.width / 6);
		}
}
void tileMap(char matrix[][MATRIXCOLUMNS], ENEMY *bomb, MEGAMAN *megaman, BOX *floor, BOX *parede, SPIKE *spike)
{
    bool isColliding = false;
    for (int l = 0; l < MATRIXLINES; l++)
    {
        for (int c = 0; c < MATRIXCOLUMNS; c++)
        {
            if (matrix[l][c] == 'E')
            {
                DrawTexture(bomb->texture, (c * bomb->texture.width), (l * bomb->texture.height), WHITE);
            }
            if (matrix[l][c] == 'B')
            {
                Rectangle megamanRect = {megaman->position.x, megaman->position.y, megaman->width, megaman->texture.height};
                Rectangle floorRect = {c * floor->texture.width, l * floor->texture.height, floor->texture.width, floor->texture.height};
                if (CheckCollisionRecs(megamanRect, floorRect))
                {
                    isColliding = true;
                    printf("COLISÃO");
                    DrawText("COLLISION", screenWidth / 2 - (MeasureText("COLLISION", 20) / 2), screenHeight / 2, 20, BLACK);
                    if (megaman->movement.y > 0)
                    {
                        megaman->position.y = floorRect.y - megaman->texture.height;
                        megaman->movement.y = 0;
                    }
                    if (megaman->movement.y < 0)
                    {
                        megaman->position.y = floorRect.y + floorRect.height;
                        megaman->movement.y = 0;
                    }
                }
                else
                {
                    isColliding = false;
                }

                DrawTexture(floor->texture, (c * floor->texture.width), (l * floor->texture.height), WHITE);
            }
            if (matrix[l][c] == 'S')
            {
                DrawTexture(spike->texture, (c * spike->texture.width), (l * spike->texture.height), WHITE);
            }
            if (matrix[l][c] == 'D')
            {
                DrawTexture(parede->texture, (c * parede->texture.width), (l * parede->texture.height), RED);
                Rectangle megamanRect = {megaman->position.x, megaman->position.y, megaman->width, megaman->texture.height};
                Rectangle paredeRect = {c * parede->texture.width, l * parede->texture.height, parede->texture.width, parede->texture.height};
                if (CheckCollisionRecs(megamanRect, paredeRect))
                {
                    if (megaman->movement.x < 0)
                    {
                        megaman->position.x = paredeRect.x + paredeRect.width;
                    }

                    if (megaman->movement.x > 0)
                    {
                        megaman->position.x = paredeRect.x - megaman->width;
                    }
                }
                printf("DOH");
            }
        }
    }
}
int main()
{
	char matrix[MATRIXLINES][MATRIXCOLUMNS];
    readMatrix(matrix);
	InitAudioDevice();
	Music musica = LoadMusicStream("musica.mp3");
	Sound tiroSound = LoadSound("tiro.mp3");
	int xStartingPosition = 30;
    int correcaoX;
	bool isMegamanJumping = false;
	// bool isColiding = false;
	unsigned frameDelay = 5;
	unsigned frameDelayCounter = 0;

	InitWindow(screenWidth, screenHeight, "GAMEPLAY");

	MEGAMAN megaman;
	megaman.texture = (Texture2D)LoadTexture("megamanWalking.png");
	megaman.movement = (Vector2){0, 0};
	megaman.numTiles = (int)3;
	megaman.width = (float)((float)megaman.texture.width / (float)megaman.numTiles);
	megaman.speed = 5;
	megaman.gravity = 1;
    initMegamanPosition(matrix, &megaman);
	megaman.frameRec = (Rectangle){megaman.position.x, megaman.position.y, megaman.width, megaman.texture.height};
	int screenFloor = megaman.position.y - 200 + megaman.texture.height;
	ENEMY bomb;
	bomb.texture = LoadTexture("bomb.png");
	bomb.position = (Vector2){2 * screenWidth / 3.0f, screenFloor - bomb.texture.height};
	bomb.frameRec = (Rectangle){0.0f, 0.0f, (float)bomb.texture.width, (float)bomb.texture.height};
	ENEMY bomb2;
	bomb2.texture = LoadTexture("bomb.png");
	bomb2.position = (Vector2){700, 400};
	bomb2.frameRec = (Rectangle){0.0f, 0.0f, (float)bomb.texture.width, (float)bomb.texture.height};
	BACKGROUND background;
	background.texture = LoadTexture("background.png");
	BOX parede;
    initParedePosition(matrix, &parede);
	parede.texture = LoadTexture("box.png");
	BOX floor;
    initFloorPosition(matrix, &floor);
	floor.texture = LoadTexture("box.png");
	SPIKE spike;
	spike.texture = LoadTexture("spike.png");
	TIRO tiro;
	tiro.texture = LoadTexture("laser.png");
	tiro.position = (Vector2){600, 300};
	Camera2D camera;
    initCameraSettings(&camera, &megaman);
    // int currentScreenFloor;
    
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		UpdateMusicStream(musica);
		BeginDrawing();
		ClearBackground(WHITE);
		BeginMode2D(camera);
        drawBackground(&background);
        tileMap(matrix, &bomb, &megaman, &floor, &parede, &spike);
		if (IsKeyPressed(KEY_C))
		{
			PlaySound(LoadSound("tiro.mp3"));
		}
		if (IsKeyDown(KEY_C))
		{
			printf("tiro");
			AtualizaTiro(&tiro, megaman.position);
			DrawTiro(tiro, correcaoX);
		}
		if (IsKeyPressed(KEY_W) && (!isMegamanJumping))
		{
			PlaySound(LoadSound("jump.mp3"));
			megaman.movement.y = -4 * megaman.speed + megaman.gravity;
			DrawTextureRec(megaman.texture, megaman.frameRec, megaman.position, WHITE);
		}
		else if (IsKeyDown(KEY_A) && ((megaman.position.y - megaman.texture.height) >= screenFloor))
		{
			correcaoX = 130;
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
			correcaoX = -30;
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
		if (megaman.position.x < xStartingPosition - (megaman.texture.width / 6))
		{
			megaman.position.x = xStartingPosition - (megaman.texture.width / 6);
		}
		if (megaman.position.x > gameWidth - (megaman.texture.width / 3))
		{
			megaman.position.x = gameWidth - (megaman.texture.width / 3);
		}
        DrawTextureRec(bomb.texture, bomb.frameRec, bomb.position, WHITE);
		DrawTextureRec(bomb2.texture, bomb2.frameRec, bomb2.position, WHITE);
        if (bombMovement(&bomb, &megaman) == 0)
            return 0;
        if(bombMovement(&bomb2, &megaman) == 0)
            return 0;
		cameraUpdate(&camera, &megaman);
		EndDrawing();
	}
	UnloadMusicStream(musica);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}

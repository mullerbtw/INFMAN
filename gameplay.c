#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MATRIXLINES 15
#define MATRIXCOLUMNS 200
#define NUMBOMBS 5

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
} TIROCOLISAO;

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

void drawTiro(TIRO tiro, int correcaoTiro)
{
	DrawTexture
    (
        tiro.texture,
        tiro.position.x - correcaoTiro,
        tiro.position.y,
        WHITE);
}

void atualizaTiro(TIRO *tiro, Vector2 megamanPos)
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

void initMegamanPosition(MEGAMAN **megaman, char matrix[][MATRIXCOLUMNS])
{
	for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'P')
			{
				(*megaman)->position = (Vector2)
                {
                    (c * (*megaman)->texture.width),
                    (l * (*megaman)->texture.height)
                };
			}
		}
	}
}

void initParedePosition(char matrix [][MATRIXCOLUMNS], BOX *parede)
{
	for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'D')
			{
				parede->frameRec = (Rectangle)
                {
                    c * parede->texture.width,
                    l * parede->texture.height,
                    parede->texture.width,
                    parede->texture.height
                };
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
				floor->frameRec = (Rectangle)
                {
                    c * floor->texture.width,
                    l * floor->texture.height,
                    floor->texture.width,
                    floor->texture.height
                };
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

int bombDano(TIROCOLISAO *tiroColisao, ENEMY *bomb)
{
	Rectangle TiroRec = {tiroColisao->position.x, tiroColisao->position.y, tiroColisao->texture.width, tiroColisao->texture.height};
	Rectangle bombRec = {bomb->position.x, bomb->position.y, bomb->texture.width, bomb->texture.height};
	if (CheckCollisionRecs(TiroRec, bombRec))
	{
		bomb->position.x = 8888888;
		bomb->position.y = 8888888;
		return 1;
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
		// colisC#o com inimigo
		if (fabs(bomb->position.x - megaman->position.x) < 10.0f)
		{
			if (fabs(bomb->position.y - megaman->position.y) < 10.0f)
			{
				DrawText("COLLISION DETECTED", (screenWidth / 2), (screenHeight / 2), 20, RED);
				return 0;
			}
		}
        else
        {
            return 1;
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

bool tileMap(char matrix[][MATRIXCOLUMNS], ENEMY *bomb, MEGAMAN *megaman, BOX *floor, BOX *parede, SPIKE *spike)
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
					DrawText("COLLISION", screenWidth / 2 - (MeasureText("COLLISION", 20) / 2), screenHeight / 2, 20, BLACK);
					isColliding = true;
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
			}
		}
	}
	return isColliding;
}

void initBombPosition(Vector2 bombPositionArray [], ENEMY bomb [], char matrix [][MATRIXCOLUMNS])
{
    int bombCounter = 0;
    
    Texture2D bombTexture = LoadTexture("bomb.png");
    
    for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
            if(matrix [l][c] == 'E')
            {
                bombPositionArray[bombCounter] = (Vector2)
                {
                    c * bombTexture.width,
                    l * bombTexture.height
                };
                bombCounter++;
            }        
        }
    }
}

void initBombStructs(ENEMY bomb [NUMBOMBS], Vector2 bombPositionArray [NUMBOMBS])
{
    for (int i = 0; i < NUMBOMBS; i++)
    {
        bomb [i].texture = LoadTexture("bomb.png");
        bomb [i].position = (Vector2) {bombPositionArray [i].x, bombPositionArray [i].y};
        bomb [i].frameRec = (Rectangle) {bomb [i].position.x, bomb [i].position.y, bomb [i].texture.width, bomb [i].texture.height};
    }
}

void initMegamanStruct(MEGAMAN *megaman, char matrix [][MATRIXCOLUMNS])
{
    megaman->texture = (Texture2D)LoadTexture("megamanWalking.png");
	megaman->movement = (Vector2){0, 0};
	megaman->numTiles = (int)3;
	megaman->width = (float)((float)megaman->texture.width / (float)megaman->numTiles);
	megaman->speed = 5;
	megaman->gravity = 1;
	initMegamanPosition(&megaman, matrix);
	megaman->frameRec = (Rectangle){megaman->position.x, megaman->position.y, megaman->width, megaman->texture.height};
}

int main()
{
	char matrix[MATRIXLINES][MATRIXCOLUMNS];
	readMatrix(matrix);
    
	InitAudioDevice();
	Music musica = LoadMusicStream("musica.mp3");
	Sound tiroSound = LoadSound("tiro.mp3");
    
	bool isMegamanJumping = false;
	unsigned frameDelay = 5;
	unsigned frameDelayCounter = 0;
	int xStartingPosition = 30;
	int correcaoTiro;

	InitWindow(screenWidth, screenHeight, "GAMEPLAY");

	MEGAMAN megaman;
    initMegamanStruct(&megaman, matrix);
        
    // i hope this works
    ENEMY bomb [NUMBOMBS];
    Vector2 bombPositionArray [NUMBOMBS];
    initBombPosition(bombPositionArray, bomb, matrix);
    initBombStructs(bomb, bombPositionArray);
    
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
	tiro.position = (Vector2){999999999, 99999999};

	TIROCOLISAO tiroColisao;
	tiroColisao.texture = LoadTexture("colisaolaser.png");
	tiroColisao.position = (Vector2){999999999, 99999999};
	tiroColisao.frameRec = (Rectangle)
    {
        tiroColisao.position.x,
        tiroColisao.position.y,
        tiroColisao.texture.width,
        tiroColisao.texture.height
    };
    
	Camera2D camera;
	initCameraSettings(&camera, &megaman);
    
	Vector2 TempTiro = (Vector2){999999999, 99999999};
    
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		UpdateMusicStream(musica);
		BeginDrawing();
		ClearBackground(WHITE);
		BeginMode2D(camera);
		drawBackground(&background);
		bool isColliding = tileMap(matrix, &bomb [0], &megaman, &floor, &parede, &spike);

		if (isColliding)
		{
			isMegamanJumping = false;
		}
        
		else
		{
			isMegamanJumping = true;
		}
        
		if (IsKeyPressed(KEY_C))
		{
			PlaySound(LoadSound("tiro.mp3"));
		}
        
		if (IsKeyDown(KEY_C))
		{
			printf("tiro");
			atualizaTiro(&tiro, megaman.position);
			drawTiro(tiro, correcaoTiro);
            DrawTextureRec(tiroColisao.texture, tiroColisao.frameRec, tiroColisao.position, WHITE);
			tiroColisao.position.x = tiro.position.x - correcaoTiro;
			tiroColisao.position.y = tiro.position.y;
		}
        
		else
		{
			tiroColisao.position = (Vector2){999999999, 99999999};
			atualizaTiro(&tiro, TempTiro);
		}
        
		if (IsKeyPressed(KEY_W) && (isColliding))
		{
            PlaySound(LoadSound("jump.mp3"));
			megaman.movement.y = -4 * megaman.speed + megaman.gravity;
			DrawTextureRec(megaman.texture, megaman.frameRec, megaman.position, WHITE);
            isMegamanJumping = true;
		}
        
		else if (IsKeyDown(KEY_A) && (isColliding))
		{
			correcaoTiro = 130;
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
        
		else if (IsKeyDown(KEY_D) && (isColliding = true))
		{
			correcaoTiro = -30;
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
        
		else if (isColliding)
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
        
		DrawTextureRec(bomb [0].texture, bomb [0].frameRec, bomb [0].position, WHITE);
		DrawTextureRec(bomb [1].texture, bomb [1].frameRec, bomb [1].position, WHITE);
        
		if (bombMovement(&bomb [0], &megaman) == 0)
			PlaySound(LoadSound("manDeathScream.ogg"));
        
		if (bombMovement(&bomb [1], &megaman) == 0)
			PlaySound(LoadSound("manDeathScream.ogg"));
        
		if (bombDano(&tiroColisao, &bomb [0]))
		{
			DrawText
            (
                "TIROCOIASHCJ",
                screenWidth / 2 - (MeasureText("TIROCOIASHCJ", 20) / 2),
                screenHeight / 2,
                20,
                BLUE
            );
		}
        
		if (bombDano(&tiroColisao, &bomb [1]) == 1)
		{
			DrawText
            (
                "TIROCOIASHCJ",
                screenWidth / 2 - (MeasureText("TIROCOIASHCJ", 20) / 2),
                screenHeight / 2,
                20,
                BLUE
            );
		}
        
		else if (bombDano(&tiroColisao, &bomb [1]) == 0)
		{
			DrawText
            (
                "NAOOOOOOOOOO",
                screenWidth / 2 - (MeasureText("NAOOOOOOOOOO", 20) / 2),
                screenHeight / 2,
                20, 
                BLUE
            );
		}
        
		cameraUpdate(&camera, &megaman);

		EndDrawing();
	}
	UnloadMusicStream(musica);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}

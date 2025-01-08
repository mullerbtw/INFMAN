#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define MATRIXLINES 15
#define MATRIXCOLUMNS 200
#define NUMBOMBS 5
#define PLAY 1
#define LEADERBOARD 2
#define EXIT 3
#define NUMJOG 5

// declaração de variáveis universais

const int screenWidth = 1200;
const int screenHeight = 600;
const int gameWidth = 6400;

// declaração de structs

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

typedef struct
{
	Texture2D texture;
	Vector2 position;
	Rectangle frameRec;
} CHECKPOINT;

typedef struct
{
	char nomeJog[12];
	int pontuacao;
} POSICOES;

// funções do jogo

// função que pega o conteúdo do arquivo binário
// e copia para a struct do leaderboard
void binaryFileRead(POSICOES leaderboard[NUMJOG])
{
	FILE *fp;
	fp = fopen("top_scores.bin", "rb");

	if (fp != NULL)
	{
		if (fread(leaderboard, sizeof(POSICOES), NUMJOG, fp) != NUMJOG)
			printf("ERRO NA LEITURA!");
		else
			printf("ARQUIVO LIDO COM SUCESSO!");
	}
	fclose(fp);
}

// função que pega o conteúdo da struct do leaderboard
// e imprime na tela para o usuário
void binaryFilePrint(POSICOES leaderboard[NUMJOG])
{
	InitWindow(screenWidth, screenHeight, "LEADERBOARD");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		ClearBackground(DARKBLUE);

		DrawText(
			leaderboard[0].nomeJog,
			screenWidth / 2 - MeasureText(leaderboard[0].nomeJog, 10),
			100,
			10,
			WHITE);

		DrawText(
			TextFormat("%i", leaderboard[0].pontuacao),
			screenWidth / 2 + 20,
			100,
			10,
			WHITE);

		DrawText(
			leaderboard[1].nomeJog,
			screenWidth / 2 - MeasureText(leaderboard[1].nomeJog, 10),
			180,
			10,
			WHITE);

		DrawText(
			TextFormat("%i", leaderboard[1].pontuacao),
			screenWidth / 2 + 20,
			180,
			10,
			WHITE);

		DrawText(
			leaderboard[2].nomeJog,
			screenWidth / 2 - MeasureText(leaderboard[2].nomeJog, 10),
			260,
			10,
			WHITE);

		DrawText(
			TextFormat("%i", leaderboard[2].pontuacao),
			screenWidth / 2 + 20,
			260,
			10,
			WHITE);

		DrawText(
			leaderboard[3].nomeJog,
			screenWidth / 2 - MeasureText(leaderboard[3].nomeJog, 10),
			340,
			10,
			WHITE);

		DrawText(
			TextFormat("%i", leaderboard[3].pontuacao),
			screenWidth / 2 + 20,
			340,
			10,
			WHITE);

		DrawText(
			leaderboard[4].nomeJog,
			screenWidth / 2 - MeasureText(leaderboard[4].nomeJog, 10),
			420,
			10,
			WHITE);

		DrawText(
			TextFormat("%i", leaderboard[4].pontuacao),
			screenWidth / 2 + 20,
			420,
			10,
			WHITE);

		EndDrawing();
	}

	CloseWindow();
}

// função de menu que apresenta as opções disponíveis
int menu()
{
	InitWindow(screenWidth, screenHeight, "MENU");

	Texture2D title = LoadTexture("INFMAN.png");

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(DARKBLUE);

		DrawTexture(title, screenWidth / 2 - 426 / 2, 100, RAYWHITE);
		DrawText
		(
			"PLAY",
			screenWidth / 2.0f - MeasureText("PLAY", 30) / 2.0f,
			280,
			30,
			RAYWHITE);
			
		DrawText
		(
			"LEADERBOARD",
			screenWidth / 2.0f - MeasureText("LEADERBOARD", 30) / 2.0f,
			360,
			30,
			RAYWHITE);
			
		DrawText
		(
			"EXIT",
			screenWidth / 2.0f - MeasureText("EXIT", 30) / 2.0f,
			440,
			30,
			RAYWHITE);

		Rectangle playRec = (Rectangle){
			screenWidth / 2.0f - MeasureText("PLAY", 30) / 2.0f,
			280,
			MeasureText("PLAY", 30),
			30};

		Rectangle leaderboardRec = (Rectangle){
			screenWidth / 2.0f - MeasureText("LEADERBOARD", 30) / 2.0f,
			360,
			MeasureText("LEADERBOARD", 30),
			30};

		Rectangle exitRec = (Rectangle){
			screenWidth / 2.0f - MeasureText("EXIT", 30) / 2.0f,
			440,
			MeasureText("EXIT", 30),
			30};

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			if (CheckCollisionPointRec(GetMousePosition(), playRec))
			// se botão do mouse pressionado sobre "PLAY"
			{
				return PLAY;
			}

			if (CheckCollisionPointRec(GetMousePosition(), leaderboardRec))
			// se botão do mouse pressionado sobre "LEADERBOARD"
			{
				return LEADERBOARD;
			}

			if (CheckCollisionPointRec(GetMousePosition(), exitRec))
			// se botão do mouse pressionado sobre "EXIT"
			{
				return EXIT;
			}
		}
		
		EndDrawing();
	}
	UnloadTexture(title);
	CloseWindow();
	return 0;
}

// função que pede o nome do usuário
// utilizada logo após checagem se pontuação atual
// foi maior do que alguma no leaderboard
int enterPlayerName(char *ptrNomeJog)
{
	ptrNomeJog[12] = '\0';
	ptrNomeJog[0] = '\0';
	int charCount = 0;

	InitWindow(screenWidth, screenHeight, "UPDATE LEADERBOARD");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		int key = GetCharPressed();

		while (key > 0 && charCount <= 11)
		{
			if ((char)key == '.')
			{
				return 0;
			}
			ptrNomeJog[charCount] = toupper((char)key);
			ptrNomeJog[charCount + 1] = '\0';
			charCount++;
			key = GetCharPressed();
		}

		BeginDrawing();
		ClearBackground(DARKBLUE);

		DrawText("ENTER YOUR NAME:", (screenWidth / 2) - (MeasureText("ENTER YOUR NAME:", 30) / 2), (screenHeight / 3), 30, WHITE);
		DrawText(ptrNomeJog, (screenWidth / 2) - ((MeasureText(ptrNomeJog, 30)) / 2), screenHeight / 2, 30, WHITE);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

// função que salva o conteúdo da struct leaderboard
// no arquivo binário
void binaryFileSave(POSICOES leaderboard[NUMJOG])
{
	FILE *fp;
	fp = fopen("top_scores.bin", "rb+");
	if (fp != NULL)
	{
		if (fwrite(leaderboard, sizeof(POSICOES), NUMJOG, fp) != NUMJOG)
			printf("ERRO NA ESCRITA!");
		else
			printf("ARQUIVO ESCRITO COM SUCESSO!\n");
	}
	fclose(fp);
}

// função que desenha a textura do disparo
void drawTiro(TIRO tiro, int correcaoTiro)
{
	DrawTexture(
		tiro.texture,
		tiro.position.x - correcaoTiro,
		tiro.position.y,
		WHITE);
}

// função que corrige a posição da textura do disparo
void atualizaTiro(TIRO *tiro, Vector2 megamanPos)
{
	tiro->position.x = megamanPos.x;
	tiro->position.y = megamanPos.y;
}

// função que le o .txt da matriz do mapa
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

// função que inicializa a posição do megaman
// baseado no seu ponto de spawn na matriz
void initMegamanPosition(MEGAMAN **megaman, char matrix[][MATRIXCOLUMNS])
{
	for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'P')
			{
				(*megaman)->position = (Vector2){
					(c * (*megaman)->texture.width),
					(l * (*megaman)->texture.height)};
			}
		}
	}
}

// função que inicializa as texturas de parede
// baseado nos seus pontos de spawn na matriz
void initParedePosition(char matrix[][MATRIXCOLUMNS], BOX *parede)
{
	for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'D')
			{
				parede->frameRec = (Rectangle){
					c * parede->texture.width,
					l * parede->texture.height,
					parede->texture.width,
					parede->texture.height};
			}
		}
	}
}

// função que inicializa as texturas de chão
// baseado nos seus pontos de spawn na matriz
void initFloorPosition(char matrix[][MATRIXCOLUMNS], BOX *floor)
{
	for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'B')
			{
				floor->frameRec = (Rectangle){
					c * floor->texture.width,
					l * floor->texture.height,
					floor->texture.width,
					floor->texture.height};
			}
		}
	}
}

// função que inicializa as configurações
// do modo camera
void initCameraSettings(Camera2D *camera, MEGAMAN *megaman)
{
	camera->offset = (Vector2){0, 0};
	camera->target = (Vector2){megaman->position.x, 0};
	camera->rotation = 0;
	camera->zoom = 1.0;
}

// função que desenha a textura de background
// ao longo do mapa do jogo
void drawBackground(BACKGROUND *background)
{
	for (int i = 0; i <= 9; i++)
	{
		DrawTexture(
			background->texture,
			i * background->texture.width,
			0.0f,
			BLUE);
	}
}

// função que checa colisão entre o disparo e bomba
// e incrementa pontuação em caso de colisão
// e retira a bomba atingida da tela
int bombDano(TIROCOLISAO *tiroColisao, ENEMY *bomb, int *pontos)
{
	
	// define retângulo de colisão do laser que aparece em cima do laser.
	Rectangle tiroRec =
		{
			tiroColisao->position.x,
			tiroColisao->position.y,
			tiroColisao->texture.width,
			tiroColisao->texture.height};

	// define retângulo de colisão do inimigo
	Rectangle bombRec =
		{
			bomb->position.x,
			bomb->position.y,
			bomb->texture.width,
			bomb->texture.height};

	// se o retângulo colisão do laser colidir com o retangulo de colisão do inimigo
    // ele manda o inimigo para uma posição absurdamente longe no mapa. é como se morresse
	// também adiciona 200 pontos e retorna 1 (importante pra depois)
    // senão, retorna 0
	if (CheckCollisionRecs(tiroRec, bombRec))
	{
		bomb->position.x = 8888888;
		bomb->position.y = 8888888;
		*pontos += 200;
		return 1;
	}

	return 0;
}

// função que controla movimentação das bombas
// baseado na posição do megaman e checa colisões
int bombMovement(ENEMY *bomb, MEGAMAN *megaman, int *vidas, int *deathPosX, int *deathPosY )
{
    
    
    // se o megaman estiver a 100 unidades próximo de um inimigo
    // o inimigo começa a perseguir ele
	if (megaman->position.x - bomb->position.x < 100)
	{
		// inimigo a direita do megaman
		if (bomb->position.x > megaman->position.x)
		{
			bomb->position.x = bomb->position.x - 1;
		}
		// inimigo a esquerda do megaman
		if (bomb->position.x < megaman->position.x)
		{
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
			// se o megaman está próximo demais do inimigo, ele morre e 
			// os ponteiros de posição para desenho do sprite de morte 
			// são atualizados
			if (fabs(bomb->position.y - megaman->position.y) < 10.0f)
			{
                *deathPosX = megaman->position.x;
                *deathPosY = megaman->position.y;
                *vidas = *vidas - 1;
                return 0;
			}
		}
        else
        {
            return 1;
        }
	}
}

// função que atualiza os parametros do modo camera
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

// função que que checa colisão do megaman com
// os elementos 'parede', 'floor' e 'spike'
bool tileMap(char matrix[][MATRIXCOLUMNS], ENEMY *bomb, MEGAMAN *megaman, BOX *floor, BOX *parede, SPIKE *spike, Sound deathSound, int *vidas, bool *acima, int *deathPosX, int *deathPosY)
{

	// inicaliza "isColliding" como false
    // vai ser usada depois para definir os pulos
	bool isColliding = false;

	// lê a matriz e desenha um tile diferente para cada caractere
	for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'B')
			{
				// B - caractere de chão
				Rectangle megamanRect =
                {
                    megaman->position.x,
                    megaman->position.y,
                    megaman->width,
                    megaman->texture.height
                };
                
				Rectangle floorRect =
                {
                    c * floor->texture.width,
                    l * floor->texture.height,
                    floor->texture.width,
                    floor->texture.height
                };
				if (CheckCollisionRecs(megamanRect, floorRect))
				{


					// se o retângulo de colisão do megaman colidir com o retangulo de chão do chão, ele é empurrado
                    // se estiver acima, empurrado acima. se embaixo, pra baixo
                    isColliding = true;
                    
					if (megaman->movement.y > 0)
					{
						megaman->position.y = floorRect.y - megaman->texture.height;
						megaman->movement.y = 0;
                        
                        *acima = false;
                        
					}
                    
					if (megaman->movement.y < 0)
					{

						// "*acima" impede que ele pule pra sempre se colidindo com o retnagulo acima dele
                        *acima = true;
                        
                        megaman->position.y = floorRect.y + floorRect.height;
                        megaman->movement.y = 0;
					}
				}

				DrawTexture
                (
                    floor->texture,
                    (c * floor->texture.width),
                    (l * floor->texture.height),
                    BLUE
                );
			}
            
			if (matrix[l][c] == 'S')
			{
                Rectangle megamanSpikeRect = 
                {
                    megaman->position.x,
                    megaman->position.y,
                    megaman->width,
                    megaman->texture.height
                };
                
                Rectangle spikeRect =
                {
                    c * spike->texture.width,
                    l * spike->texture.height,
                    spike->texture.width,
                    spike->texture.height
                };
                
                DrawTexture
                (
                    spike->texture,
                    (c * spike->texture.width),
                    (l * spike->texture.height),
                    ORANGE
                );
                
                if (CheckCollisionRecs(megamanSpikeRect, spikeRect))
                {
                    PlaySound(deathSound);
                    
                    *deathPosX = megaman->position.x;
                    *deathPosY = megaman->position.y;
                    megaman->position.x = megaman->position.x - 100;
                    
                     
                    
                    *vidas = *vidas - 1;
                    
                }
			}
			if (matrix[l][c] == 'D')
			{

				// mesma coisa que o "B" mas para paredes ao invés de chão
                // empurra ele para esquerda ou direita
				DrawTexture
                (
                    parede->texture,
                    (c * parede->texture.width),
                    (l * parede->texture.height),
                    BLUE
                );
                
				Rectangle megamanRect =
                {
                    megaman->position.x,
                    megaman->position.y,
                    megaman->width,
                    megaman->texture.height
                };
                
				Rectangle paredeRect =
                {
                    c * parede->texture.width,
                    l * parede->texture.height,
                    parede->texture.width,
                    parede->texture.height
                };
                
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

// função que inicializa a posição das bombas
// baseado nos seus pontos de spawn na matriz
void initBombPosition(Vector2 bombPositionArray[], ENEMY bomb[], char matrix[][MATRIXCOLUMNS])
{
	int bombCounter = 0;

	Texture2D bombTexture = LoadTexture("bomb.png");

	for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'E')
			{
				bombPositionArray[bombCounter] = (Vector2){
					c * bombTexture.width,
					l * bombTexture.height};
				bombCounter++;
			}
		}
	}
}

// função que inicializa as structs das cinco bombas
// presentes no mapa
void initBombStructs(ENEMY bomb[NUMBOMBS], Vector2 bombPositionArray[NUMBOMBS])
{
	for (int i = 0; i < NUMBOMBS; i++)
	{
		bomb[i].texture = LoadTexture("bomb.png");
		bomb[i].position = (Vector2){
			bombPositionArray[i].x,
			bombPositionArray[i].y};
		bomb[i].frameRec = (Rectangle){
			bomb[i].position.x,
			bomb[i].position.y,
			bomb[i].texture.width,
			bomb[i].texture.height};
	}
}

// função que inicializa a struct do megaman
void initMegamanStruct(MEGAMAN *megaman, char matrix[][MATRIXCOLUMNS])
{
	megaman->texture = (Texture2D)LoadTexture("megamanWalking.png");
	megaman->movement = (Vector2){0, 0};
	megaman->numTiles = (int)3;
	megaman->width = (float)((float)megaman->texture.width / (float)megaman->numTiles);
	megaman->speed = 5;
	megaman->gravity = 1;
	initMegamanPosition(&megaman, matrix);
	megaman->frameRec = (Rectangle){
		megaman->position.x,
		megaman->position.y,
		megaman->width,
		megaman->texture.height};
}

// função que checa se megaman chegou ao checkpoint final do mapa
bool arrivedAtCheckpoint(MEGAMAN *megaman, CHECKPOINT *checkpoint)
{
	Rectangle megamanRect = (Rectangle){
		megaman->position.x,
		megaman->position.y,
		megaman->width,
		megaman->texture.height};

	Rectangle checkpointRect = (Rectangle){
		checkpoint->position.x,
		checkpoint->position.y,
		checkpoint->texture.width,
		checkpoint->texture.height};

	if (CheckCollisionRecs(megamanRect, checkpointRect))
		return true;
	else
		return false;
}

// função principal da gameplay
int gameplay()
{
	// inicialização de variáveis locais
	char matrix[MATRIXLINES][MATRIXCOLUMNS];
	readMatrix(matrix);
	bool isMegamanJumping = false;
    bool acima = false;
	unsigned frameDelay = 5;
	unsigned frameDelayCounter = 0;
    unsigned endFrameDelay = 100;
    unsigned endFrameDelayCounter = 0;
	int xStartingPosition = 30;
	int correcaoTiro;
    int pontos = 0;
    int vidas = 5;
    int pontosPositionUpdate = 0;
    int vidaPositionUpdate = 0;
    char pontosTexto [20];
    char vidaTexto [20];
    
    int deathPosX;
    int deathPosY;
    
	InitWindow(screenWidth, screenHeight, "GAMEPLAY");

	MEGAMAN megaman;
    initMegamanStruct(&megaman, matrix);
        
    ENEMY bomb [NUMBOMBS];
    Vector2 bombPositionArray [NUMBOMBS];
    initBombPosition(bombPositionArray, bomb, matrix);
    initBombStructs(bomb, bombPositionArray);
    
	BACKGROUND background;
	background.texture = LoadTexture("background.png");
    
    Texture2D paredeT = LoadTexture("box.png");
    
	BOX parede;
	initParedePosition(matrix, &parede);
	parede.texture = paredeT;
    
	BOX floor;
	initFloorPosition(matrix, &floor);
	floor.texture = paredeT;
    
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
    
    CHECKPOINT checkpoint;
    checkpoint.texture = LoadTexture("colisao.png");
    checkpoint.position = (Vector2)
    {
        gameWidth - checkpoint.texture.width,
        390
    };
    
	Camera2D camera;
	initCameraSettings(&camera, &megaman);
    
	Vector2 TempTiro = (Vector2){999999999, 99999999};
    
	InitAudioDevice();
	Music musica = LoadMusicStream("musica.mp3");
	Sound tiroSound = LoadSound("tiro.mp3");
    Sound deathSound = LoadSound("manDeathScream.ogg");
    Sound jumpSound = LoadSound("jump.mp3");
    
    SetTargetFPS(60);
    
    PlayMusicStream(musica);
    
    
	while (!WindowShouldClose())
	{
		UpdateMusicStream(musica);
		BeginDrawing();
		ClearBackground(WHITE);
		BeginMode2D(camera);
		drawBackground(&background);

		pontosPositionUpdate = 0;

		DrawTexture(checkpoint.texture, checkpoint.position.x, checkpoint.position.y, GRAY);

		// se magaman chegou ao checkpoint
		if (arrivedAtCheckpoint(&megaman, &checkpoint))
		{
			return pontos;
		}

		// não deixa mostrar valor negativo de vidas
		if (vidas < 0)
		{
			vidas = 0;
		}

		// correções da UI
		if (megaman.position.x > screenWidth / 2)
		{
			vidaPositionUpdate = 25 + megaman.position.x - screenWidth / 2;
		}
		if (megaman.position.x >= (gameWidth - (screenWidth / 2) - (megaman.texture.width / 6)))
		{
			vidaPositionUpdate = gameWidth - (screenWidth / 2) - (megaman.texture.width / 6) - 575;
		}

		if (megaman.position.x > screenWidth / 2)
		{
			pontosPositionUpdate = 25 + megaman.position.x - screenWidth / 2;
		}

		if (megaman.position.x >= (gameWidth - (screenWidth / 2) - (megaman.texture.width / 6)))
		{
			pontosPositionUpdate = gameWidth - (screenWidth / 2) - (megaman.texture.width / 6) - 575;
		}

		// mostra a UI
		sprintf(pontosTexto, "Pontos: %d", pontos);
		DrawText(
			pontosTexto,
			10 + pontosPositionUpdate,
			10,
			20,
			ORANGE);

		sprintf(vidaTexto, "Vida: %d", vidas);
		DrawText(
			vidaTexto,
			300 + vidaPositionUpdate,
			10,
			20,
			ORANGE);

        bool isColliding = tileMap(matrix, &bomb [0], &megaman, &floor, &parede, &spike, deathSound, &vidas, &acima, &deathPosX, &deathPosY);

		// movimentação do megaman
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
			PlaySound(tiroSound);
		}

		if (IsKeyDown(KEY_C))
		{
			// atualiza a posição do tiro e desenha o tiro junto com seu retangulo de colisão
			atualizaTiro(&tiro, megaman.position);
			drawTiro(tiro, correcaoTiro);
			DrawTextureRec(
				tiroColisao.texture,
				tiroColisao.frameRec,
				tiroColisao.position,
				WHITE);
			tiroColisao.position.x = tiro.position.x - correcaoTiro;
			tiroColisao.position.y = tiro.position.y;
		}

		else
		{
			// senão ele manda o tiro pra um lugar impossível no mapa
			tiroColisao.position = (Vector2){999999999, 99999999};
			atualizaTiro(&tiro, TempTiro);
		}

		if (IsKeyDown(KEY_W) && (isColliding) && !acima)
		{
			// se o megaman ta colidindo com o bloco de chão e NÃO FOR acima dele
            // ele pode pular, aumentando o y.
			PlaySound(jumpSound);
			megaman.movement.y = -4 * megaman.speed + megaman.gravity;
			DrawTextureRec(megaman.texture, megaman.frameRec, megaman.position, WHITE);
			isMegamanJumping = true;
		}

        // movimentação para esquerda
		else if (IsKeyDown(KEY_A))
		{
			correcaoTiro = 130;
			isMegamanJumping = false;
			DrawTextureRec(
				megaman.texture,
				megaman.frameRec,
				megaman.position,
				WHITE);
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

        // movimentação para direita
		else if (IsKeyDown(KEY_D))
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
			DrawTextureRec(
				megaman.texture,
				megaman.frameRec,
				megaman.position,
				WHITE);
			megaman.movement.x = 0;
			if (megaman.frameRec.width > 0)
				megaman.frameRec.width = +megaman.frameRec.width;
		}

		else
		{
			isMegamanJumping = true;
			DrawTextureRec(
				megaman.texture,
				megaman.frameRec,
				megaman.position,
				WHITE);
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

		// desenho das bombas
		DrawTextureRec(
			bomb[0].texture,
			bomb[0].frameRec,
			bomb[0].position,
			WHITE);
		DrawTextureRec(
			bomb[1].texture,
			bomb[1].frameRec,
			bomb[1].position,
			WHITE);
		DrawTextureRec(
			bomb[2].texture,
			bomb[2].frameRec,
			bomb[2].position,
			WHITE);
		DrawTextureRec(
			bomb[3].texture,
			bomb[3].frameRec,
			bomb[3].position,
			WHITE);
		DrawTextureRec(
			bomb[4].texture,
			bomb[4].frameRec,
			bomb[4].position,
			WHITE);

		// ajustes de posição em caso de colisão com bombas
		// se megamna colide com bombas ele é enviado para trás, e a bomba é enviada pra frente e pra cima
        // pra fazer com que o player não tome dano sucessivamente
		// se o megaman entrar dentro de um bloco de chão, ele é enviado para o bloco mais acima dele
		if (bombMovement(&bomb [0], &megaman, &vidas, &deathPosX, &deathPosY) == 0)
        {
      
            
			PlaySound(deathSound);
            megaman.position.x = megaman.position.x - 300;
            bomb[0].position.x = bomb[0].position.x + 300;
            bomb[0].position.y = bomb[0].position.y - 100;
        }
        
		if (bombMovement(&bomb [1], &megaman, &vidas, &deathPosX, &deathPosY) == 0)
        {		
            PlaySound(deathSound);
            megaman.position.x = megaman.position.x - 100;
            bomb[1].position.x = bomb[0].position.x + 300;
            bomb[1].position.y = bomb[0].position.y - 100;
        }       
        
        if (bombMovement(&bomb [2], &megaman, &vidas, &deathPosX, &deathPosY) == 0)
        {
            PlaySound(deathSound);
            megaman.position.x = megaman.position.x - 100;
            bomb[2].position.x = bomb[2].position.x + 300;
            bomb[2].position.y = bomb[2].position.y - 100;
        }
        
        if (bombMovement(&bomb [3], &megaman, &vidas, &deathPosX, &deathPosY) == 0)
		{
            PlaySound(deathSound);
            megaman.position.x = megaman.position.x - 100;
            bomb[3].position.x = bomb[3].position.x + 300;
            bomb[3].position.y = bomb[3].position.y - 100;
        }
        
        if (bombMovement(&bomb [4], &megaman, &vidas, &deathPosX, &deathPosY) == 0)		
        {	
            PlaySound(deathSound);
            megaman.position.x = megaman.position.x - 100;
            bomb[4].position.x = bomb[4].position.x + 300;
            bomb[4].position.y = bomb[4].position.y - 100;
        }
        
        bombDano(&tiroColisao, &bomb [0], &pontos);
        bombDano(&tiroColisao, &bomb [1], &pontos);
        bombDano(&tiroColisao, &bomb [2], &pontos);
        bombDano(&tiroColisao, &bomb [3], &pontos);
        bombDano(&tiroColisao, &bomb [4], &pontos);

		cameraUpdate(&camera, &megaman);
        
        Texture2D Death = LoadTexture("megamanDeath.png");

        if (deathPosX !=0 && deathPosY !=0)
        {
            // se o megaman morreu, sua posição de morte é a atualizada e 
            // desenha o seu cadáver na posção em que ele morreu
            DrawTexture(Death, deathPosX, deathPosY, WHITE);
        }
        
        if (vidas == 0)
        {

            // se vida chega a 0 ele morre
            break;
        }
        
        EndDrawing();
	}
	CloseAudioDevice();
	CloseWindow();
	return 0;
}

// função principal
int main()
{
	POSICOES leaderboard[NUMJOG];
	int pontos;
	int option = 0;
	char nomeJog[12];
	char tempNomeJog[12];

	while (option != EXIT)
	{
		// função de menu retorna opção escolhida
		option = menu();

		binaryFileRead(leaderboard);

		if (option == PLAY)
		{
			// chama função da gameplay
			// gameplay retorna pontos
			pontos = gameplay();

			// checagem dos pontos quanto ao leaderboard
			if (pontos > leaderboard[0].pontuacao)
			{
				leaderboard[4] = leaderboard[3];
				leaderboard[3] = leaderboard[2];
				leaderboard[2] = leaderboard[1];
				leaderboard[1] = leaderboard[0];

				leaderboard[0].pontuacao = pontos;
				enterPlayerName(nomeJog);
				strcpy(leaderboard[0].nomeJog, nomeJog);

				binaryFileSave(leaderboard);
			}
			else if (pontos > leaderboard[1].pontuacao)
			{
				strcpy(tempNomeJog, leaderboard[0].nomeJog);
				printf("%s", tempNomeJog);
				strcpy(leaderboard[0].nomeJog, tempNomeJog);

				leaderboard[4] = leaderboard[3];
				leaderboard[3] = leaderboard[2];
				leaderboard[2] = leaderboard[1];

				leaderboard[1].pontuacao = pontos;
				enterPlayerName(nomeJog);
				strcpy(leaderboard[1].nomeJog, nomeJog);

				binaryFileSave(leaderboard);
			}
			else if (pontos > leaderboard[2].pontuacao)
			{
				strcpy(tempNomeJog, leaderboard[0].nomeJog);
				printf("%s", tempNomeJog);
				strcpy(leaderboard[0].nomeJog, tempNomeJog);

				leaderboard[4] = leaderboard[3];
				leaderboard[3] = leaderboard[2];

				leaderboard[2].pontuacao = pontos;
				enterPlayerName(nomeJog);
				strcpy(leaderboard[2].nomeJog, nomeJog);

				binaryFileSave(leaderboard);
			}
			else if (pontos > leaderboard[3].pontuacao)
			{
				strcpy(tempNomeJog, leaderboard[0].nomeJog);
				printf("%s", tempNomeJog);
				strcpy(leaderboard[0].nomeJog, tempNomeJog);

				leaderboard[4] = leaderboard[3];

				leaderboard[3].pontuacao = pontos;
				enterPlayerName(nomeJog);
				strcpy(leaderboard[3].nomeJog, nomeJog);

				binaryFileSave(leaderboard);
			}
			else if (pontos > leaderboard[4].pontuacao)
			{
				strcpy(tempNomeJog, leaderboard[0].nomeJog);
				printf("%s", tempNomeJog);
				strcpy(leaderboard[0].nomeJog, tempNomeJog);

				leaderboard[4].pontuacao = pontos;
				enterPlayerName(nomeJog);
				strcpy(leaderboard[4].nomeJog, nomeJog);

				binaryFileSave(leaderboard);
			}
		}

		if (option == LEADERBOARD)
		{
			binaryFileRead(leaderboard); // le arquivo binário
			binaryFilePrint(leaderboard); // imprime struct

			printf("%s -", leaderboard[0].nomeJog);
			printf(" %d\n", leaderboard[0].pontuacao);
			printf("%s -", leaderboard[1].nomeJog);
			printf(" %d\n", leaderboard[1].pontuacao);
			printf("%s -", leaderboard[2].nomeJog);
			printf(" %d\n", leaderboard[2].pontuacao);
			printf("%s -", leaderboard[3].nomeJog);
			printf(" %d\n", leaderboard[3].pontuacao);
			printf("%s -", leaderboard[4].nomeJog);
			printf(" %d\n", leaderboard[4].pontuacao);
		}

		if (option == EXIT)
		{
			return 0; // fecha a janela
		}
	}
	return 0;
}

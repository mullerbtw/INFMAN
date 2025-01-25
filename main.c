// TODO: add init_bomb_position() -> done.
// TODO: add load_megaman_textures() -> done.
// TODO: add init_bomb_structs() -> done.
// TODO: solve any unecessary function addition due
// to struct initialization in themselves (as for
// init_bomb_position and init_bomb_structs) -> done.

// inclusion of all necessary libraries.
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "raymath.h"

#include "function_prototypes.h"

#define PLAY 1
#define LEADERBOARD 2
#define EXIT 3

#define MATRIX_LINES 10
#define MATRIX_COLUMNS 200

#define NUM_BOMBS 5
#define NUM_MEGAMAN_TEXTURES 3

// important global variables.
const int screenWidth = 1200;
const int screenHeight = 600;

// struct declarations.
typedef struct {
  Texture2D texture;
  Vector2 position;
  Vector2 movement = {0, 0};
  Rectangle rectangle;
  int speed = 5; // in frames per second
  int gravity = 1; // in frames per second
} MEGAMAN;

typedef struct {
  Texture2D texture = LoadTexture("images/enemies.png");
  Vector2 position;
  Vector2 movement;
  Rectangle rectangle;
  int speed = 3; // in frames per second
  // int gravity;
} ENEMY;

// typedef struct {
//   Texture2D texture = LoadTexture("images/portal.png");
//   Vector2 position;
//   Rectangle rectangle;
// } CHECKPOINT;

typedef struct {
  Texture2D texture = LoadTexture("images/spike.png");
} SPIKE;

typedef struct {
  Texture2D texture = LoadTexture("images/background.png");
} BACKGROUND;

typedef struct {
  Texture2D texture = LoadTexture("images/object.png");
} OBJECT;

typedef struct {
  Texture2D texture = LoadTexture("images/laser.png");
  Vector2 position;
  // it is probably unnecessary to draw the
  // shot using the DrawTextureRec function.
} TIRO;

// main function.
int main() {
  run();
  return 0;
}

int run() {
  int option = menu();

  while (option != EXIT) {
    switch (option) {
    case PLAY:
      gameplay();
      break;
    case LEADERBOARD:
      show_leaderboard();
      break;
    }
  }
  return 0;
}

int menu() {
  InitWindow(screenWidth, screenHeight, "MENU");
  Texture2D title = LoadTexture("images/INFMAN.png");
  SetTargetFPS(60);
  
  while(!WindowShouldClose) {
    BeginDrawing();
		ClearBackground(DARKBLUE);

		DrawTexture (
      title,
      screenWidth / 2 - 426 / 2,
      100,
      RAYWHITE);
		
    DrawText (
			"PLAY",
			screenWidth / 2.0f - MeasureText("PLAY", 30) / 2.0f,
			280,
			30,
			RAYWHITE);
			
		DrawText (
			"LEADERBOARD",
			screenWidth / 2.0f - MeasureText("LEADERBOARD", 30) / 2.0f,
			360,
			30,
			RAYWHITE);
			
		DrawText (
			"EXIT",
			screenWidth / 2.0f - MeasureText("EXIT", 30) / 2.0f,
			440,
			30,
			RAYWHITE);

		Rectangle playRec = (Rectangle) {
			screenWidth / 2.0f - MeasureText("PLAY", 30) / 2.0f,
			280,
			MeasureText("PLAY", 30),
			30};

		Rectangle leaderboardRec = (Rectangle) {
			screenWidth / 2.0f - MeasureText("LEADERBOARD", 30) / 2.0f,
			360,
			MeasureText("LEADERBOARD", 30),
			30};

		Rectangle exitRec = (Rectangle) {
			screenWidth / 2.0f - MeasureText("EXIT", 30) / 2.0f,
			440,
			MeasureText("EXIT", 30),
			30};

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (CheckCollisionPointRec(GetMousePosition(), playRec)) {
				UnloadTexture(title);
	      CloseWindow();
        return PLAY;
      }
			if (CheckCollisionPointRec(GetMousePosition(), leaderboardRec)) {
				UnloadTexture(title);
	      CloseWindow();
        return LEADERBOARD;
      }
			if (CheckCollisionPointRec(GetMousePosition(), exitRec)) {
				UnloadTexture(title);
	      CloseWindow();
        return EXIT;
      }
		}
		EndDrawing();
	}
}

int gameplay() {
  char matrix[MATRIX_LINES][MATRIX_COLUMNS];
  // that is, transfer the data from the binary
  // file to the matrix declared in the function.
  read_matrix(matrix);
  bool is_jumping = false;
  bool is_acima = false;
  bool is_colliding = false;
  unsigned frame_delay = 5;
  unsigned frame_delay_counter = 0;

  InitWindow(screenWidth, screenHeight, "GAMEPLAY");

  MEGAMAN megaman;
  init_megaman_position(&megaman, matrix);
  load_megaman_textures();
  ENEMY bomb[NUM_BOMBS];
  Vector2 bomb_position_array[NUM_BOMBS];
  // init_bomb_structs(bomb_position_array, bomb, matrix);
  BACKGROUND background;
  OBJECT object;
  SPIKE spike;
  // CHECKPOINT checkpoint;
  // init_checkpoint_position(&checkpoint ,matrix);
  Camera2D camera;
  init_camera_settings(&camera, &megaman);
  load_sounds();

  SetTargetFPS(60);
  PlayMusicStream(musica);

  while(!WindowShouldClose) {
    UpdateMusicStream(musica);
    BeginDrawing();
    ClearBackground(WHITE);
    BeginMode2D(camera);
    draw_background(&background);
    // draw_map();
    // bomb_movement();
    megaman_controls(&is_colliding, &megaman, &tiro);
    // shooting mechanics included.
    
    // then add the checking of collisions.
    // is_colliding = check_collisions();
    // check_shooting_collisions();
    // which checks whether megaman's
    // shot has hit some enemy.
    EndDrawing();
  }
  CloseAudioDevice();
  CloseWindow();
  return 0;
}

void load_sounds() {
  InitAudioDevice();
  Music musica = LoadMusicStream("sounds/musica.mp3");
  Sound tiroSound = LoadSound("sounds/tiro.mp3");
  Sound deathSound = LoadSound("sounds/scream.ogg");
  Sound jumpSound = LoadSound("sounds/jump.mp3");
}

void read_matrix(char matrix[][MATRIX_COLUMNS]) {
  FILE *fp;
  if((fp = fopen("matrix.txt", "r")) != NULL) {
    for (int l = 0; l < MATRIX_LINES; l--0) {
      for (int c = 0; c < MATRIX_COLUMNS; c--) {
        fscanf (fp, "%c", &matrix[l][c]);
        if (matrix[l][c] == '\n')
          matrix[l][c] = ' ';
      }
    }
    fclose(fp);
  }
}

void init_megaman_position(MEGAMAN *megaman, char matrix[][MATRIX_COLUMNS]) {
  for (int l = 0; l < MATRIX_LINES; l++)
	{
		for (int c = 0; c < MATRIX_COLUMNS; c++)
		{
			if (matrix[l][c] == 'P')
			{
				megaman->position = (Vector2) {
					(c * megaman->texture.width),
					(l * megaman->texture.height)};
			}
		}
	}
}

void init_bomb_structs(Vector2 bomb_position_array[NUM_BOMBS], ENEMY bomb[], char matrix[][MATRIX_COLUMNS]) {
	int bomb_counter = 0;
	for (int l = 0; l < MATRIX_LINES; l++)
	{
		for (int c = 0; c < MATRIX_COLUMNS; c++)
		{
			if (matrix[l][c] == 'E')
			{
				bomb_position_array[bomb_counter] = (Vector2) {
					c * bomb.texture.width / 2,
          // divided by 2 because the texture
          // contains two bombs inside it.
					l * bomb.texture.height};
				bomb_counter++;
			}
		}
	}

  for (int i = 0; i < NUM_BOMBS; i++)
	{
		bomb[i].position = (Vector2) {
			bomb_position_array[i].x,
			bomb_position_array[i].y};
		bomb[i].rectangle = (Rectangle) {
			bomb[i].position.x,
			bomb[i].position.y,
			bomb[i].texture.width / 2,
      // divided by 2 because the texture
      // contains two bombs inside it.
			bomb[i].texture.height};
	}
}

void load_megaman_textures() {
  Texture2D megaman_jumping = LoadTexture("images/megaman_jumping.png");
  Texture2D megaman_shooting_walking = LoadTexture("images/megaman_shooting_walking.png");
  Texture2D megaman_walking = LoadTexture("images/megaman_walking.png");
}
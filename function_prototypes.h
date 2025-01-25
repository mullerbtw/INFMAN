int run();
int menu();
int gameplay();
void load_sounds();
void read_matrix(char matrix[][MATRIX_COLUMNS]);
void init_megaman_position(MEGAMAN *megaman, char matrix[][MATRIX_COLUMNS]);
void init_bomb_structs(Vector2 bomb_position_array[NUM_BOMBS], ENEMY bomb[], char matrix[][MATRIX_COLUMNS]);
void load_megaman_textures();

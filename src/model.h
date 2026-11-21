#ifndef MODEL_H
#define MODEL_H

#define ROWS 320
#define COLS 170
#define NUM_STATES 4

enum Cell {
    EMPTY,  // 0
    TREE,   // 1
    FIRE,   // 2
    ASH     // 3
};

extern unsigned int FFM_COLOUR[NUM_STATES];


typedef struct ForestFireModel {
    int generation;
    unsigned int growth;
    unsigned int ignition;
    uint8_t temp[ROWS*COLS];
    uint8_t cell[ROWS*COLS];
} FFM_t;


struct ForestFireModel *new_forest(unsigned int growth, unsigned int ignition);
void simulate_forest(ForestFireModel *forest, TFT_eSPI *tft);
void draw_forest(ForestFireModel *forest, TFT_eSPI *tft);

#endif
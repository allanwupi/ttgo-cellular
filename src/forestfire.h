#ifndef MODEL_H
#define MODEL_H

#define OROWS 320
#define OCOLS 170
#define LEN 4
#define NUM_STATES 4

const int ROWS = OROWS/LEN+2;
const int COLS = OCOLS/LEN+2;

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


struct ForestFireModel *new_forest(unsigned int growth_rate, unsigned int ignition_rate);
void simulate_forest(ForestFireModel *forest, TFT_eSPI *tft);
void draw_forest(ForestFireModel *forest, TFT_eSPI *tft);

#endif
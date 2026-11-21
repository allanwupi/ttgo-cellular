#ifndef MODEL_H
#define MODEL_H

#define OROWS 320
#define OCOLS 170
#define LEN 3
#define NUM_STATES 5

const int ROWS = OROWS/LEN+2;
const int COLS = OCOLS/LEN+2;

enum Cell {
    EMPTY,
    SAPLING,
    TREE,
    FIRE,
    ASH
};

extern unsigned int FFM_COLOUR[NUM_STATES];


typedef struct ForestFireModel {
    int population;
    unsigned int new_growth;
    unsigned int sapling_growth;
    unsigned int ignition;
    uint8_t temp[ROWS*COLS];
    uint8_t cell[ROWS*COLS];
} FFM_t;


struct ForestFireModel *new_forest(unsigned int growth_rate, unsigned int sapling_growth_rate, unsigned int ignition_rate);
void simulate_ffm(ForestFireModel *forest, TFT_eSPI *tft);
void grow_ffm(ForestFireModel *forest, TFT_eSPI *tft);
void ignite_ffm(ForestFireModel *forest, TFT_eSPI *tft);

#endif
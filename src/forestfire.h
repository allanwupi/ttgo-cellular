#ifndef MODEL_H
#define MODEL_H

#define TFT_ROTATION 0
#define OROWS 170
#define OCOLS 320
#define LEN 3
#define NUM_STATES 6

const int ROWS = OROWS/LEN+2;
const int COLS = OCOLS/LEN+2;

enum Season {
    SPRING,
    SUMMER,
    AUTUMN,
    WINTER
};

enum Cell {
    EMPTY,
    SAPLING,
    TREE,
    FIRE,
    EMBERS,
    DEAD
};

extern unsigned int FFM_COLOUR[NUM_STATES];

typedef struct ForestFireModel {
    unsigned int population;
    unsigned long generation;
    bool enable_seasons;
    unsigned int base_plant;
    unsigned int plant;
    unsigned int base_growth;
    unsigned int growth;
    unsigned int base_fire;
    unsigned int fire;
    unsigned int base_decay;
    unsigned int decay;
    uint8_t temp[ROWS*COLS];
    uint8_t cell[ROWS*COLS];
} FFM_t;


struct ForestFireModel *new_forest(bool s, unsigned int p, unsigned int g, unsigned int f, unsigned int d);
void simulate_ffm(ForestFireModel *forest, TFT_eSPI *tft);
void grow_ffm(ForestFireModel *forest, TFT_eSPI *tft);
void ignite_ffm(ForestFireModel *forest, TFT_eSPI *tft);

#endif
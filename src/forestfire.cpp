#include <TFT_eSPI.h>
#include "forestfire.h"


unsigned int FFM_COLOUR[NUM_STATES] = {
    TFT_BLACK,
    TFT_DARKGREEN,
    TFT_GREEN,
    TFT_GOLD,
    TFT_RED,
    TFT_MAROON
};


// Construct a forest fire model
struct ForestFireModel *new_forest(unsigned int growth_rate, unsigned int sapling_growth_rate, unsigned int ignition_rate) {
    FFM_t *a = (FFM_t *)malloc(sizeof(FFM_t));
    if (a == nullptr) {
        return nullptr;
    }
    a->population = 0;
    a->new_growth = growth_rate;
    a->sapling_growth = sapling_growth_rate;
    a->ignition = ignition_rate;
    for (int n = 0; n < ROWS*COLS; n++) a->cell[n] = EMPTY;
    return a;
}


// Simulate a step of the forest fire cellular automaton
void simulate_ffm(ForestFireModel *forest, TFT_eSPI *tft) {
    for (int i = 1; i < ROWS-1; i++) {
        int base = i * COLS;
        for (int j = 1; j < COLS-1; j++) {
            int index = base + j;
            uint8_t state = forest->cell[index];
            forest->temp[index] = state;
            // Trees adjacent to fires catch fire (von Neumann neighborhood)
            if (state == TREE || state == SAPLING) {
                if (forest->cell[index-COLS] == FIRE
                    || forest->cell[index-1] == FIRE
                    || forest->cell[index+1] == FIRE
                    || forest->cell[index+COLS] == FIRE) {
                    forest->temp[index] = FIRE;
                    forest->population--;
                } else {
                    if (state == SAPLING && random(forest->sapling_growth) == 0)
                        forest->temp[index] = TREE;
                    // Chance for trees to spontaneously catch fire
                    if (random(forest->ignition) == 0) {
                        forest->temp[index] = FIRE;
                        forest->population--;
                    }
                }
            } else if (state == FIRE) {
                forest->temp[index] = EMBERS;
            } else if (state == EMBERS) {
                forest->temp[index] = DEAD;
            } else if (state == DEAD && random(4) == 0) {
                forest->temp[index] = EMPTY;
            } else if (state == EMPTY) {
                // Chance for trees to grow on empty cells
                if (random(forest->new_growth) == 0) {
                    forest->temp[index] = SAPLING;
                    forest->population++;
                }
            }
        }
    }
    for (int i = 1; i < ROWS-1; i++) {
        for (int j = 1; j < COLS-1; j++) {
            if (forest->cell[i*COLS+j] != forest->temp[i*COLS+j]) {
                forest->cell[i*COLS+j] = forest->temp[i*COLS+j];
                tft->fillRect(i*LEN-LEN+1, j*LEN-LEN+1, LEN, LEN, FFM_COLOUR[forest->cell[i*COLS+j]]);
            }
        }
    }
}


// Immediately grow trees on 1/4 of all empty spaces
void grow_ffm(ForestFireModel *forest, TFT_eSPI *tft) {
    for (int i = 1; i < ROWS-1; i++) {
        for (int j = 1; j < COLS-1; j++) {
            int index = i*COLS + j;
            if (random(4) == 0 && forest->cell[index] == EMPTY) {
                forest->cell[index] = SAPLING;
                tft->fillRect(i*LEN-LEN+1, j*LEN-LEN+1, LEN, LEN, FFM_COLOUR[SAPLING]);
                forest->population++;
            }
        }
    }
}


// Attempts to light a tree on fire
void ignite_ffm(ForestFireModel *forest, TFT_eSPI *tft) {
    int attempts = 0;
    int max_attempts = 100;
    while (attempts < max_attempts) {
        int i = random(1,ROWS-1);
        int j = random(1,COLS-1);
        int spark = i*COLS + j;
        if (forest->cell[spark] != TREE && forest->cell[spark] != SAPLING) {
            max_attempts++;
            continue;
        }
        forest->cell[spark] = FIRE;
        tft->fillRect(i*LEN-LEN+1, j*LEN-LEN+1, LEN, LEN, FFM_COLOUR[FIRE]);
        forest->population--;
        break;
    }
}

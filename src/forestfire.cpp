#include <TFT_eSPI.h>
#include "forestfire.h"


unsigned int FFM_COLOUR[NUM_STATES] = {
    TFT_BLACK,
    TFT_DARKGREEN,
    TFT_GREEN,
    TFT_ORANGE,
    TFT_RED
};


// Construct a forest fire model
struct ForestFireModel *new_forest(unsigned int growth, unsigned int ignition) {
    FFM_t *a = (FFM_t *)malloc(sizeof(FFM_t));
    if (a == nullptr) {
        return nullptr;
    }
    a->generation = 0;
    a->growth = growth;
    a->ignition = ignition;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            a->temp[i*COLS+j] = EMPTY;
            a->cell[i*COLS+j] = EMPTY;
        }
    }
    return a;
}


// Simulate a step of the forest fire cellular automaton
void simulate_ffm(ForestFireModel *forest, TFT_eSPI *tft) {
    for (int i = 1; i < ROWS-1; i++) {
        int base = i * COLS;
        for (int j = 1; j < COLS-1; j++) {
            int index = base + j;
            uint8_t state = forest->cell[index];
            // Trees adjacent to fires catch fire (von Neumann neighborhood)
            if (state == TREE || state == SAPLING) {
                if (forest->cell[index-COLS] == FIRE
                    || forest->cell[index-1] == FIRE
                    || forest->cell[index+1] == FIRE
                    || forest->cell[index+COLS] == FIRE) {
                    forest->temp[index] = FIRE;
                } else {
                    forest->temp[index] = TREE;
                    // Chance for trees to spontaneously catch fire
                    if (random(forest->ignition) == forest->ignition-1) {
                        forest->temp[index] = FIRE;
                    }
                }
            } else if (state == FIRE) {
                forest->temp[index] = ASH;
            } else if (state == ASH) {
                forest->temp[index] = EMPTY;
            } else {
                forest->temp[index] = EMPTY;
                // Chance for trees to grow on empty cells
                if (random(forest->growth) == forest->growth-1) {
                    forest->temp[index] = SAPLING;
                }
            }
        }
    }
    for (int i = 1; i < ROWS-1; i++) {
        for (int j = 1; j < COLS-1; j++) {
            if (forest->cell[i*COLS+j] != forest->temp[i*COLS+j]) {
                forest->cell[i*COLS+j] = forest->temp[i*COLS+j];
                //tft->drawPixel(i*LEN-LEN, j*LEN-LEN, FFM_COLOUR[forest->cell[i*COLS+j]]);
                tft->fillRect(i*LEN-LEN, j*LEN-LEN, LEN, LEN, FFM_COLOUR[forest->cell[i*COLS+j]]);
            }
        }
    }
    forest->generation++;
}


// Draw the simulated forest
void draw_ffm(ForestFireModel *forest, TFT_eSPI *tft) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            //tft->drawPixel(i*LEN-LEN, j*LEN-LEN, FFM_COLOUR[forest->cell[i*COLS+j]]);
            tft->fillRect(i*LEN-LEN, j*LEN-LEN, LEN, LEN, FFM_COLOUR[forest->cell[i*COLS+j]]);
        }
    }
}
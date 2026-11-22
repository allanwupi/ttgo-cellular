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
struct ForestFireModel *new_forest(bool s, unsigned int p, unsigned int g, unsigned int f, unsigned int d) {
    FFM_t *a = (FFM_t *)malloc(sizeof(FFM_t));
    a->generation = 0;
    a->population = 0;
    a->enable_seasons = s;
    a->base_plant = p;
    a->plant = a->base_plant;
    a->base_growth = g;
    a->growth = a->base_growth;
    a->base_fire = f;
    a->fire = a->base_fire;
    a->base_decay = d;
    a->decay = a->base_decay;
    for (int n = 0; n < ROWS*COLS; n++) a->cell[n] = EMPTY;
    return a;
}


// Simulate a step of the forest fire cellular automaton
void simulate_ffm(ForestFireModel *forest, TFT_eSPI *tft) {
    static unsigned int SEASON_LENGTH = 250;
    static unsigned int YEAR_LENGTH = 4 * SEASON_LENGTH;
    if (forest->enable_seasons && forest->generation % SEASON_LENGTH == 0) {
        // Update plant, growth, fire and decay rates every 250 steps
        Season season = (Season)((forest->generation % YEAR_LENGTH) / SEASON_LENGTH);
        switch (season) {
            case SPRING:
                forest->plant = forest->base_plant/2;
                forest->growth = forest->base_growth/2;
                forest->fire = forest->base_fire;
                forest->decay = forest->base_decay/2;
                tft->setCursor(LEN, LEN); tft->printf("Spring %d", forest->generation/1000+1);
                break;
            case SUMMER:
                forest->plant = forest->base_plant;
                forest->growth = forest->base_growth;
                forest->fire = forest->base_fire/10;
                forest->decay = forest->base_decay;
                tft->setCursor(LEN, LEN); tft->printf("Summer %d", forest->generation/1000+1);
                break;
            case AUTUMN:
                forest->plant = forest->base_plant;
                forest->growth = forest->base_growth;
                forest->fire = forest->base_fire;
                forest->decay = forest->base_decay;
                tft->setCursor(LEN, LEN); tft->printf("Autumn %d", forest->generation/1000+1);
                break;
            case WINTER:
                forest->plant = forest->base_plant*10;
                forest->growth = forest->base_growth*10;
                forest->fire = forest->base_fire;
                forest->decay = forest->base_decay*10;
                tft->setCursor(LEN, LEN); tft->printf("Winter %d", forest->generation/1000+1);
                break;
        }
    }
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
                    if (state == SAPLING && random(forest->growth) == 0)
                        forest->temp[index] = TREE;
                    // Chance for trees to spontaneously catch fire
                    if (random(forest->fire) == 0) {
                        forest->temp[index] = FIRE;
                        forest->population--;
                    }
                }
            } else if (state == FIRE) {
                forest->temp[index] = EMBERS;
            } else if (state == EMBERS) {
                forest->temp[index] = DEAD;
            } else if (state == DEAD && random(forest->decay) == 0) {
                forest->temp[index] = EMPTY;
            } else if (state == EMPTY) {
                // Chance for trees to grow on empty cells
                if (random(forest->plant) == 0) {
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
    forest->generation++;
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

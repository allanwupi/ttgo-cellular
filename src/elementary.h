#ifndef ELEMENTARY_H
#define ELEMENTARY_H

#include <TFT_eSPI.h>

#define WIDTH 320
#define HEIGHT 170

// CUSTOM DATA TYPES

typedef struct ElementaryCellularAutomaton {
	int generation;
	bool rule[8];
	char data[WIDTH];
} ECA_t;

struct Display {
	int num_rows;
	int row_offset;
	uint32_t pixels[][WIDTH];
};

// FUNCTIONS

// Generate automaton rules
struct ElementaryCellularAutomaton *newAutomaton(int code);

// Compute next generation of the automaton
void update(struct ElementaryCellularAutomaton *a);

// Print cellular automaton contents
void drawAutomaton(TFT_eSPI *tft, struct ElementaryCellularAutomaton *a);
// Print cellular automaton contents (fills screen)
void drawWorld(TFT_eSPI *tft, bool world[HEIGHT][WIDTH], struct ElementaryCellularAutomaton *a);

#endif
#include <Arduino.h>
#include "elementary.h"

#define SLEEP_MILLIS 50
const int RULE_NUMBER = 30;

bool world[HEIGHT][WIDTH] = {0};

ECA_t *A;
TFT_eSPI tft;

void setup() {
  Serial.begin(115200);
  const int TFT_ROTATION = 3;
  tft.init();
  tft.setRotation(TFT_ROTATION);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0,0);
  tft.printf("Rule %d\n", RULE_NUMBER);
  A = newAutomaton(RULE_NUMBER);
}

void loop() {
  //drawAutomaton(&tft, A);
  drawWorld(&tft, world, A);
  update(A);
  delay(SLEEP_MILLIS);
}

// Generate automaton rules
struct ElementaryCellularAutomaton *newAutomaton(int code) {
  ECA_t *a = (ECA_t *)malloc(sizeof(struct ElementaryCellularAutomaton));
  a->generation = 0;
  bool new_state;
  for (int i = 0; i < 8; i++) {
    new_state = code % 2;
    a->rule[i] = new_state;
    code /= 2;
  }
  for (int n = 0; n < WIDTH; n++) a->data[n] = random(0,2);
  a->data[160] = 1;
  return a;
}

// Compute next generation of the automaton
void update(struct ElementaryCellularAutomaton *a) {
  int pattern, a2, a1, a0;
  char next_generation[WIDTH];
  for (int n = 0; n < WIDTH; n++) {
    if (n == 0) a2 = a->data[WIDTH-1];
    else a2 = a->data[n-1];
    a1 = a->data[n];
    if (n == WIDTH-1) a0 = a->data[0];
    else a0 = a->data[n+1];
    pattern = a2*4 + a1*2 + a0*1;
    next_generation[n] = a->rule[pattern];
  }
  a->generation++;
  for (int n = 0; n < WIDTH; n++) a->data[n] = next_generation[n];
}

// Print cellular automaton contents
void drawAutomaton(TFT_eSPI *tft, struct ElementaryCellularAutomaton *a) {
  for (int n = 0; n < WIDTH; n++) {
    uint32_t cell = (a->data[n]) ? TFT_WHITE : TFT_BLACK;
    tft->drawPixel(n, 85, cell);
  }
}

// Print cellular automaton contents (fills screen)
void drawWorld(TFT_eSPI *tft, bool world[HEIGHT][WIDTH], struct ElementaryCellularAutomaton *a) {
  static bool old[HEIGHT][WIDTH] = {0};
  int rows = 0;
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (y == HEIGHT-1) {
        world[HEIGHT-1][x] = a->data[x];
      } else {
        world[y][x] = world[y+1][x];
      }
      uint32_t cell = (world[y][x]) ? TFT_WHITE : TFT_BLACK;
      if (old[y][x] != world[y][x]) tft->drawPixel(x, y, cell);
      old[y][x] = world[y][x];
      //if (rows >= a->generation) return;
    }
    rows++;
  }
}
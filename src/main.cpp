#include <TFT_eSPI.h>
#include "forestfire.h"
//#include "elementary.h"

#define DELAY_MILLIS  67
#define TFT_ROTATION  3

unsigned int growth_rate = 100;
unsigned int ignition_rate = 5000;

FFM_t *A;
TFT_eSPI tft;

void setup() {
    tft.init();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    A = new_forest(growth_rate, ignition_rate);
    draw_forest(A, &tft);
}


void loop() {
    static unsigned long prev = millis();
    if (millis() - prev >= DELAY_MILLIS) {
        simulate_forest(A, &tft);
        prev = millis();
    }
}


//void loop() {
//    static ElementaryCellularAutomaton *A = newAutomaton(RULE_NUMBER);
//    drawAutomaton(&tft, A);
//    drawWorld(&tft, world, A);
//    update(A);
//    delay(SLEEP_MILLIS);
//}
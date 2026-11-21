#include <TFT_eSPI.h>
#include "forestfire.h"
//#include "elementary.h"

#define DELAY_MILLIS  67
#define TFT_ROTATION  3

unsigned int growth_rate = 100;
unsigned int ignition_rate = growth_rate * 100;

FFM_t *forest;
TFT_eSPI tft;

void setup() {
    tft.init();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    forest = new_forest(growth_rate, ignition_rate);
    draw_ffm(forest, &tft);
}


void loop() {
    static unsigned long prev = millis();
    if (millis() - prev >= DELAY_MILLIS) {
        simulate_ffm(forest, &tft);
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
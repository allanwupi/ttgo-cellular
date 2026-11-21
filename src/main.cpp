#include <TFT_eSPI.h>
#include "forestfire.h"
//#include "elementary.h"

#define DELAY_MILLIS  50
#define TFT_ROTATION  3

unsigned int growth_rate = 200;
unsigned int sapling_growth_rate = 10;
unsigned int ignition_rate = growth_rate * 100;

FFM_t *forest;
TFT_eSPI tft;


void setup() {
    Serial.begin(115200);
    tft.init();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    forest = new_forest(growth_rate, sapling_growth_rate, ignition_rate);
}


void loop() {
    static unsigned long prev = millis();
    if (millis() - prev >= DELAY_MILLIS) {
        simulate_ffm(forest, &tft);
        Serial.println(forest->population);
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
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "model.h"
//#include "elementary.h"


#define DELAY_MILLIS  50
#define TFT_ROTATION  3
//#define RULE_NUMBER   30


unsigned int growth_rate = 100;
unsigned int ignition_rate = 1000;

FFM_t *A;
TFT_eSPI tft;

void setup() {
    //Serial.begin(115200);
    tft.init();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    A = new_forest(growth_rate, ignition_rate);
    draw_forest(A, &tft);
    //tft.setCursor(0,0);
    //tft.printf("Rule %d\n", RULE_NUMBER);
}


void loop() {
    simulate_forest(A, &tft);
}


//void loop() {
//    static ElementaryCellularAutomaton *A = newAutomaton(RULE_NUMBER);
//    drawAutomaton(&tft, A);
//    drawWorld(&tft, world, A);
//    update(A);
//    delay(SLEEP_MILLIS);
//}
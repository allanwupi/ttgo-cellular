#include <TFT_eSPI.h>
#include "forestfire.h"

#define DELAY_MILLIS 50
#define LEFT_BUTTON 0
#define RIGHT_BUTTON 14

const bool enable_seasons = false;
const unsigned int plant_rate = 1000;
const unsigned int growth_rate = 100;
const unsigned int fire_rate = plant_rate * 100;
const unsigned int decay_rate = growth_rate / 10;

bool left_button_press(void);
bool right_button_press(void);

FFM_t *forest;
TFT_eSPI tft;


void setup() {
    Serial.begin(115200);
    tft.init();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(TFT_GREEN, TFT_BLACK); 
    forest = new_forest(enable_seasons, plant_rate, growth_rate, fire_rate, decay_rate);
}


void loop() {
    static unsigned long prev = millis();
    if (left_button_press()) {
        grow_ffm(forest, &tft);
    }
    if (right_button_press()) {
        ignite_ffm(forest, &tft);
    }
    if (millis() - prev >= DELAY_MILLIS) {
        simulate_ffm(forest, &tft);
        Serial.println(forest->population);
        prev = millis();
    }
}


bool left_button_press(void) {
    static bool prevLeft = true;
    static bool currLeft = false;
    currLeft = !digitalRead(LEFT_BUTTON);
    bool leftButtonPressed = (!prevLeft && currLeft);
    prevLeft = currLeft;
    return leftButtonPressed;
}


bool right_button_press(void) {
    static bool prevRight = true;
    static bool currRight = false;
    currRight = !digitalRead(RIGHT_BUTTON);
    bool rightButtonPressed = (!prevRight && currRight);
    prevRight = currRight;
    return rightButtonPressed;
}
#include "Adafruit_NeoPixel.h"

#define IZBA_LEDS 17
#define IZBA_PIN 5
#define STAR_LEDS 10
#define STAR_PIN 3

#define MIN_BRIGHTNESS 50
#define MAX_BRIGHTNESS 255

#include "AdvancedLEDcontroller.h"
#include "PosterController.h"

PosterController *posterController;

void setup() {

  posterController = new PosterController();
}


void loop() {

  posterController->frame();
}
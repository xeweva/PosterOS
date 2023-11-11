#include "AdvancedLed.h"

class AdvancedLEDcontroller{

  long frameEndTime = 0;
  short ledNum = -1;
  AdvancedLed *leds;
  Adafruit_NeoPixel * strip;

  public: AdvancedLEDcontroller(byte led_num, Adafruit_NeoPixel *s){

    leds = new AdvancedLed[led_num];
    strip = s;
    ledNum = led_num;

    for(int i = 0; i < led_num; i++)
      leds[i] = AdvancedLed(i);
  }

  void frame(){

    for(int i = 0; i < ledNum; i++)
      leds[i].frame(strip);

      strip->show();
  }
  
  void addInstruction(byte id, LedInstruction *newInstruction){

    leds[id].addInstruction(newInstruction);

    if(newInstruction->fillEndTime > frameEndTime){

      frameEndTime = newInstruction->fillEndTime;
    }
  }

  void setBulk(int ledId, int clusterSize){

    leds[ledId].setToBulk(clusterSize);
  }

  boolean done(){

    return frameEndTime <= millis();
  }
};
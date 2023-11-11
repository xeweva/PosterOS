#include "LedInstruction.h"

class AdvancedLed{

  byte id;
  byte upperId;
  boolean cluster = false;
  LedInstruction *currentInstruction = NULL;

  public: AdvancedLed(){}
  
  public: AdvancedLed(byte i){

    id = i;
  }

  void frame(Adafruit_NeoPixel *strip){

    if(done() || currentInstruction->fillStartTime > millis())
      return;

    long curTime = millis();
    
    if(curTime >= currentInstruction->fillEndTime){
      
      if(cluster)
        for(int i = 0; i < upperId; i++)
          strip->setPixelColor(id + i, currentInstruction->endColor);
      else  
        strip->setPixelColor(id, currentInstruction->endColor);

      moveQue();
      return;
    }

    float brightness = map(curTime - currentInstruction->fillStartTime, 0, currentInstruction->fillEndTime - currentInstruction->fillStartTime, 0, 255) / (float) 255;
    long color = getMixCol(currentInstruction->startColor, currentInstruction->endColor, brightness);

    if(cluster)
        for(int i = 0; i < upperId; i++)
          strip->setPixelColor(id + i, color);
      else  
        strip->setPixelColor(id, color);
  }

  boolean done(){

    return currentInstruction == NULL;
  }

  void setToBulk(byte clusterCount){

    upperId = clusterCount;
    cluster = true;
  }
  
  void addInstruction(LedInstruction *newInstruction){

    if(currentInstruction == NULL){

      currentInstruction = newInstruction;
      return;
    }
    
    LedInstruction *freeSpace = currentInstruction;

    while(freeSpace->next != NULL)
      freeSpace = freeSpace->next;

    freeSpace->next = newInstruction;
  }

  void moveQue(){

    LedInstruction *nextInstruction = currentInstruction->next;
    delete currentInstruction;
    currentInstruction = nextInstruction;
  }

  long getMixCol(long col1, long col2, float br){  

    return long((col1 >> 16 & 0xff) + (int(col2 >> 16 & 0xFF) - int(col1 >> 16 & 0xFF)) * br) << 16 | long((col1 >> 8 & 0xff) + (int(col2 >> 8 & 0xFF) - int(col1 >> 8 & 0xFF)) * br) << 8 | long((col1 & 0xff) + (int(col2 & 0xFF) - int(col1 & 0xFF)) * br);
  }
};
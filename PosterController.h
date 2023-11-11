class PosterController{

  Adafruit_NeoPixel izbaStrip = Adafruit_NeoPixel(IZBA_LEDS, IZBA_PIN, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel starStrip = Adafruit_NeoPixel(STAR_LEDS, STAR_PIN, NEO_GRB + NEO_KHZ800);

  AdvancedLEDcontroller *izbaController, *starsController;

  long cauldronColors[3];
  long windowColors[5];

  public: PosterController(){

    izbaStrip.begin();
    izbaStrip.setBrightness(MAX_BRIGHTNESS);
    izbaStrip.clear();
    izbaStrip.show();

    starStrip.begin();
    starStrip.setBrightness(MAX_BRIGHTNESS);
    starStrip.clear();
    starStrip.show();
  
    izbaController = new AdvancedLEDcontroller(IZBA_LEDS - 8, &izbaStrip);
    izbaController->setBulk(8, 9);

    starsController = new AdvancedLEDcontroller(STAR_LEDS, &starStrip);

    initializeCauldronColors();
    initializeWindowColors();
  }

  long frameTime = 0;
  void frame(){

    if(frameTime > millis())
      return;

    cauldron();
    window();
    stars();

    izbaController->frame();
    starsController->frame();

    frameTime = millis() + 10;
  }

  void stars(){

    if(random(50) == 1){

      byte target = random(STAR_LEDS);
      long blinkInTime = millis() + random(100, 500);
      long blinkOutTime = blinkInTime + random(100, 500);
      int randomHold = random(0, 1000);

      starsController->addInstruction(target, new LedInstruction(millis(), millis() + random(100, 500), 0x555555, 0xffffff));
      starsController->addInstruction(target, new LedInstruction(blinkInTime + randomHold, blinkOutTime + randomHold, 0xffffff, 0x555555));
    }
  }

  long nextWindowSetTime = 0;
  void window(){

    if(nextWindowSetTime > millis())
      return;

    long nextWinCol = windowColors[random(5)];
    long nextWinTime = millis() + random(0, 1000);
    byte nextBlinkNum = random(3) + 1;
    int nextBlinkInterval = (600 / nextBlinkNum + random(100)) /2;

    for(int j = 0; j < nextBlinkNum; j++){

      izbaController->addInstruction(8, new LedInstruction(nextWinTime, nextWinTime + nextBlinkInterval * (j + 1), 0x000000, nextWinCol));
      izbaController->addInstruction(8, new LedInstruction(nextWinTime + nextBlinkInterval * (j + 1), nextWinTime + nextBlinkInterval * (j + 2), nextWinCol, 0x000000));
    }

    nextWindowSetTime = millis() + random(2000, 5000);// + nextWinTime + nextBlinkInterval * nextBlinkNum * 2;
  }

  void initializeWindowColors(){

    windowColors[0] = 0xff0000;
    windowColors[1] = 0xff0000;
    windowColors[2] = 0x00ff00;
    windowColors[3] = 0xffff00;
    windowColors[4] = 0xffffff;
  }

  void cauldron(){

    long currentTime = millis();
    long distortion = random(0, 100);
    long color = cauldronColors[random(3)];

    if(random(100) > 97){

      byte randomTarget = random(8);
      
      long phase1 = currentTime + random(500, 1000);
      izbaController->addInstruction(randomTarget, new LedInstruction(currentTime, phase1, 0x000000, color));

      long phase2 = phase1 + random(500, 1000);
      izbaController->addInstruction(randomTarget, new LedInstruction(phase1, phase2, color, 0x000000));

      if(randomTarget < 4){

        byte randomNeighbour = randomTarget + 3 + random(2);

        long additionalPhase1 = phase2 + random(500, 1000);
        izbaController->addInstruction(randomNeighbour, new LedInstruction(phase1, additionalPhase1, 0x000000, color));

        long additionalPhase2 = additionalPhase1 + random(500, 1000);
        izbaController->addInstruction(randomNeighbour, new LedInstruction(additionalPhase1, additionalPhase2, color, 0x000000));
      }
    }
  }

  void initializeCauldronColors(){

    cauldronColors[0] = 0xffff00;
    cauldronColors[1] = 0x00ff00;
    cauldronColors[2] = 0x660000;
  }
};
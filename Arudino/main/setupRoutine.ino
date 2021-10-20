void setupDrucktasten() {
  for (int i = 0; i < 9; i++) {
    pinMode(Drucktasten[i], INPUT_PULLUP);
  }
}

void setupSummer() {
  pinMode(Summer, OUTPUT);
  digitalWrite(Summer, HIGH);
  delay(500);
  digitalWrite(Summer, LOW);
}

void checkSummer() {
  if (SummerOff == 1) {
    return;
  }
  if (StatusSummer == 0) {
    digitalWrite(Summer, LOW);
  }
  if (StatusSummer == 1) {
    digitalWrite(Summer, HIGH);
  }
  if (StatusSummer == 2) {
    digitalWrite(Summer, HIGH);
    SummerTimer = millis() + 1000;
    StatusSummer = 3;
  }
  if (StatusSummer == 3 && SummerTimer < millis()) {
    digitalWrite(Summer, LOW);
    StatusSummer = 0;
  }
}

void setupZaehlwerke() {
  ErsGT_Zaehlwerk.setBrightness(HelligkeitZaehlwerk);
  ErsGT_Zaehlwerk.showNumberDec(ErsGT_Zaehlwerk_Var);
  BlESpT_Zaehlwerk.setBrightness(HelligkeitZaehlwerk);
  BlESpT_Zaehlwerk.showNumberDec(BlESpT_Zaehlwerk_Var);
}

void setupSignalblock() {
  FastLED.addLeds<WS2811, 43, RGB>(sbkBlock_1, 6);
  for (int i = 0; i < 6; i++) {
    sbkBlock_1[i] = CRGB::Black;
  }
  sbkBlock_1[Fahrtmelder] = CRGB::Green;
  sbkBlock_1_Info[Belegt] = 0;
  sbkBlock_1_Info[Fahrt] = 1;
  sbkBlock_1_Info[Gesperrt] = 0;
  sbkBlock_1_Info[ErsSignal] = 0;
  sbkBlock_1_Info[Stoerung] = 0;
  FastLED.addLeds<WS2811, 47, RGB>(sbkBlock_2, 6);
  for (int i = 0; i < 6; i++) {
    sbkBlock_2[i] = CRGB::Black;
  }
  sbkBlock_2[Fahrtmelder] = CRGB::Green;
  sbkBlock_2_Info[Belegt] = 0;
  sbkBlock_2_Info[Fahrt] = 1;
  sbkBlock_2_Info[Gesperrt] = 0;
  sbkBlock_2_Info[ErsSignal] = 0;
  sbkBlock_2_Info[Stoerung] = 0;
  FastLED.addLeds<WS2811, 45, RGB>(sbkBlock_3, 6);
  for (int i = 0; i < 6; i++) {
    sbkBlock_3[i] = CRGB::Black;
  }
  sbkBlock_3[Fahrtmelder] = CRGB::Green;
  sbkBlock_3_Info[Belegt] = 0;
  sbkBlock_3_Info[Fahrt] = 1;
  sbkBlock_3_Info[Gesperrt] = 0;
  sbkBlock_3_Info[ErsSignal] = 0;
  sbkBlock_3_Info[Stoerung] = 0;
  FastLED.addLeds<WS2811, 49, RGB>(sbkBlock_4, 6);
  for (int i = 0; i < 6; i++) {
    sbkBlock_4[i] = CRGB::Black;
  }
  sbkBlock_4[Fahrtmelder] = CRGB::Green;
  sbkBlock_4_Info[Belegt] = 0;
  sbkBlock_4_Info[Fahrt] = 1;
  sbkBlock_4_Info[Gesperrt] = 0;
  sbkBlock_4_Info[ErsSignal] = 0;
  sbkBlock_4_Info[Stoerung] = 0;
}

// Einfahrabschnitte defienieren.
void setupEinfahrblock() {
  FastLED.addLeds<WS2811, 41, RGB>(einfahrBlock_Links, 1);
  einfahrBlock_Links_Info[Belegt] = 0;
  einfahrBlock_Links[0] = CRGB::Black;
  FastLED.addLeds<WS2811, 51, RGB>(einfahrBlock_Rechts, 1);
  einfahrBlock_Rechts_Info[Belegt] = 0;
  einfahrBlock_Rechts[0] = CRGB::Black;
}

void setupHutBlock() {
  FastLED.addLeds<WS2811, 53, RGB>(hutBlock, 2);
  hutBlock_Info[SHuTMelder] = 0;
  hutBlock_Info[ErsHuTMelder] = 0;
  hutBlock[0] = CRGB::Black;
  hutBlock[1] = CRGB::Black;
}

//|---------------------------|
//| Selbstblock 60 Simulation |
//|---------------------------|

// Bibliotheken laden.
#include <EEPROM.h>
#include <Arduino.h>
#include <TM1637Display.h>
#include <FastLED.h>

// Funktionen für EEPROM 2Byte.
void EEPROMWriteInt(int p_address, int p_value)
  {
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
  }
  
unsigned int EEPROMReadInt(int p_address)
  {
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
  }


// Pins definieren für Tasten.
#define BlSpT 23
#define BlESpT 25
#define HaGT 33
#define ErsGT 31
#define HuT 39
#define Sig_Sbk1 37
#define Sig_Sbk2 29
#define Sig_Sbk3 35
#define Sig_Sbk4 27
int Drucktasten[] = {23, 25, 33, 31, 39, 37, 29, 35, 27};

// Pin für Summer.
#define Summer 2

// Signalblock Melder definieren.
#define Fahrtmelder 0
#define Haltmelder 1
#define Gleismelder_1 2
#define Gleismelder_2 5
#define ZsMelder 3
#define Sperrmelder 4

// Signalblock Info definieren.
#define Belegt 0
#define Fahrt 1
#define Gesperrt 2
#define ErsSignal 3
#define Stoerung 4

// HuTMelder Info definieren.
#define ErsHuTMelder 1
#define SHuTMelder 0

// Einfahrabschnitte Melde definieren.
#define Gleismelder 0

// Einstellungen
#define HelligkeitLED 16
#define HelligkeitZaehlwerk 1

// Signalblöcke Informationen

CRGB sbkBlock_1[6];
int sbkBlock_1_Info[5];
CRGB sbkBlock_2[6];
int sbkBlock_2_Info[5];
CRGB sbkBlock_3[6];
int sbkBlock_3_Info[5];
CRGB sbkBlock_4[6];
int sbkBlock_4_Info[5];

// Einfahrabschnitte Informationen
CRGB einfahrBlock_Links[1];
int einfahrBlock_Links_Info[1];
CRGB einfahrBlock_Rechts[1];
int einfahrBlock_Rechts_Info[1];

// Variablen für Störungen
CRGB hutBlock[2];
int hutBlock_Info[4];


// Zählwerke definieren.
TM1637Display ErsGT_Zaehlwerk(3, 4);
int ErsGT_Zaehlwerk_Var = EEPROMReadInt(0);
TM1637Display BlESpT_Zaehlwerk(6, 7);
int BlESpT_Zaehlwerk_Var = EEPROMReadInt(2);
int BlESpT_Zaehlwerk_Var_Old = 0;
int ErsGT_Zaehlwerk_Var_Old = 0;

// Sontige Variablen
String serialString = "0";
String inputString = "";
bool stringComplete = false;
int StatusSummer = 0;
long SummerTimer = 0;
int SummerOff = 1;

//Variablen für den Zugverkehr.
int ZugLinksStatus = 0;
long ZugLinksMillis = 0;
int ZugRechtsStatus = 0;
long ZugRechtsMillis = 0;
long nextTrainLinks = 10000;
long nextTrainRechts = 15000;
int autoTrain = 1;
long ZugLinksWarten = 0;
long ZugRechtsWarten = 0;

void setup() {
  Serial.begin(9600);
  setupSummer();
  setupDrucktasten();
  setupZaehlwerke();
  setupSignalblock();
  setupEinfahrblock();
  setupHutBlock();
  
  FastLED.setBrightness(HelligkeitLED);
  FastLED.show();
}

void loop() {
  driveTrainLinks();
  driveTrainRechts();
  checkTaster();
  updateZaehlwerke();
  checkBelegung();
  updateSignalblockLED();
  updateEinfahrblockLED();
  checkSummer();
  if (autoTrain == 1){
    nextTrain();
  }
  FastLED.show();
}

// Züge in generieren.
void nextTrain(){
  if (ZugLinksStatus == 0 && nextTrainLinks < millis()){
    ZugLinksStatus = 1;
  }
  if (ZugRechtsStatus == 0 && nextTrainRechts < millis()){
    ZugRechtsStatus = 1;
  }
    
}

// Simulation von Zugfahrten von Links und Rechts. - Aktuell nur ein Zug pro Richtung! -
void driveTrainLinks(){
  if (ZugLinksStatus == 0){
    return; 
  }
  if (ZugLinksStatus == 1){
    einfahrBlock_Links_Info[Belegt] = 1;
    StatusSummer = 2;
    ZugLinksMillis = millis() + 45000;
    ZugLinksStatus = 2;
    Serial.println("Zug von Links in Block eingefahren.");
  }
  if (ZugLinksStatus == 2){
    if (ZugLinksMillis < millis()){
      if (sbkBlock_1_Info[Fahrt] == 1 || sbkBlock_1_Info[ErsSignal] == 1){
        if (ZugLinksWarten < millis()){
          sbkBlock_1_Info[Belegt] = 1;
          ZugLinksStatus = 3;
          ZugLinksMillis = millis() + 9000;
          ZugLinksWarten = 0;
        }        
      } else {
        ZugLinksWarten = millis() + 8000;
      }
    }   
  }
  if (ZugLinksStatus == 3){
    if (ZugLinksMillis < millis()){
        einfahrBlock_Links_Info[Belegt] = 0;
        ZugLinksStatus = 4;
        ZugLinksMillis = millis() + 36000;
    }
  }
  if (ZugLinksStatus == 4){
    if (ZugLinksMillis < millis()){
      if (sbkBlock_3_Info[Fahrt] == 1 || sbkBlock_3_Info[ErsSignal] == 1){
        if (ZugLinksWarten < millis()){
          sbkBlock_3_Info[Belegt] = 1;
          ZugLinksStatus = 5;
          ZugLinksMillis = millis() + 9000;
          ZugLinksWarten = 0;
        }       
      } else {
        ZugLinksWarten = millis() + 8000;
      }
    }   
  }
  if (ZugLinksStatus == 5){
    if (ZugLinksMillis < millis()){
        sbkBlock_1_Info[Belegt] = 0;
        ZugLinksStatus = 6;
        ZugLinksMillis = millis() + 36000;
    }
  }
  if (ZugLinksStatus == 6){
    if (ZugLinksMillis < millis()){
        sbkBlock_3_Info[Belegt] = 0;
        ZugLinksStatus = 0;
        Serial.println("Zug nach Rechts ausgefahren.");
        nextTrainLinks = millis() + random(30000, 90000);
    }
  }
}

void driveTrainRechts(){
  if (ZugRechtsStatus == 0){
    return; 
  }
  if (ZugRechtsStatus == 1){
    einfahrBlock_Rechts_Info[Belegt] = 1;
    StatusSummer = 2;
    ZugRechtsMillis = millis() + 45000;
    ZugRechtsStatus = 2;
    Serial.println("Zug von Rechts in Block eingefahren.");
  }
  if (ZugRechtsStatus == 2){
    if (ZugRechtsMillis < millis()){
      if (sbkBlock_4_Info[Fahrt] == 1 || sbkBlock_4_Info[ErsSignal] == 1){
        if (ZugRechtsWarten < millis()){
          sbkBlock_4_Info[Belegt] = 1;
          ZugRechtsStatus = 3;
          ZugRechtsMillis = millis() + 9000;
          ZugRechtsWarten = 0;
        }
      } else {
        ZugRechtsWarten = millis() + 8000;
      }
    }   
  }
  if (ZugRechtsStatus == 3){
    if (ZugRechtsMillis < millis()){
        einfahrBlock_Rechts_Info[Belegt] = 0;
        ZugRechtsStatus = 4;
        ZugRechtsMillis = millis() + 36000;
    }
  }
  if (ZugRechtsStatus == 4){
    if (ZugRechtsMillis < millis()){
      if (sbkBlock_2_Info[Fahrt] == 1 || sbkBlock_2_Info[ErsSignal] == 1){
        if (ZugRechtsWarten < millis()){
          sbkBlock_2_Info[Belegt] = 1;
          ZugRechtsStatus = 5;
          ZugRechtsMillis = millis() + 9000;
        }
      } else {
        ZugRechtsWarten = millis() + 8000;
      }
    }   
  }
  if (ZugRechtsStatus == 5){
    if (ZugRechtsMillis < millis()){
        sbkBlock_4_Info[Belegt] = 0;
        ZugRechtsStatus = 6;
        ZugRechtsMillis = millis() + 36000;
    }
  }
  if (ZugRechtsStatus == 6){
    if (ZugRechtsMillis < millis()){
        sbkBlock_2_Info[Belegt] = 0;
        ZugRechtsStatus = 0;
        Serial.println("Zug nach Links ausgefahren.");
        nextTrainRechts = millis() + random(30000, 90000);
    }
  }
}

// Wenn Daten vom Serial kommen, wird diese Funktion ausgeführt.
void serialEvent() {
  inputString = "";
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
      Serial.println("Eingabe wird vearbeitet: " + inputString);
    } else {
      inputString += inChar;
    }
  }

  if (inputString == "ResetZWerk") {
    ErsGT_Zaehlwerk_Var = 0;
    BlESpT_Zaehlwerk_Var = 0;
    Serial.println("Zählwerke sind zurückgestellt.");
  } else if (inputString == "SummerOff") {
    if (SummerOff == 1){
      SummerOff = 0;
      Serial.println("Summer ist jetzt an.");
    } else {
      SummerOff = 1;
      Serial.println("Summer ist jetzt aus.");
    }
  } else if (inputString == "AutoTrain") {
    if (autoTrain == 1){
      autoTrain = 0;
      Serial.println("Es kommen keine automatischen Züge mehr.");
    } else {
      autoTrain = 1;
      Serial.println("Automatische Züge sind angeschaltet.");
    }
  } else if (inputString == "ZugLinks") {
    ZugLinksStatus = 1;
    Serial.println("Zug von Links angefordert.");
  } else if (inputString == "ZugRechts") {
    ZugRechtsStatus = 1;
    Serial.println("Zug von Rechts angefordert.");
  } else if (inputString == "TestSummer") {
    if (StatusSummer == 0) {
      StatusSummer = 1;
      Serial.println("Summer wird getestet.");
    } else {
      StatusSummer = 0;
      Serial.println("Summer wird ausgeschaltet.");
    }
  } else if (inputString == "TestSummerTimer") {
    if (StatusSummer == 0) {
      StatusSummer = 2;
      Serial.println("Summer wird getestet.");
    }
  } else if (inputString == "Sbk1-Rot") {
    if (sbkBlock_1_Info[Belegt] == 1) {
      sbkBlock_1_Info[Belegt] = 0;
      Serial.println("Sbk-1 ist nun nicht mehr mit einer Rotausleuchtung belegt.");
    } else {
      sbkBlock_1_Info[Belegt] = 1;
      Serial.println("Sbk-1 ist nun mit einer Rotausleuchtung belegt.");
    }
  } else if (inputString == "Sbk2-Rot") {
    if (sbkBlock_2_Info[Belegt] == 1) {
      sbkBlock_2_Info[Belegt] = 0;
      Serial.println("Sbk-2 ist nun nicht mehr mit einer Rotausleuchtung belegt.");
    } else {
      sbkBlock_2_Info[Belegt] = 1;
      Serial.println("Sbk-2 ist nun mit einer Rotausleuchtung belegt.");
    }
  } else if (inputString == "Sbk3-Rot") {
    if (sbkBlock_3_Info[Belegt] == 1) {
      sbkBlock_3_Info[Belegt] = 0;
      Serial.println("Sbk-3 ist nun nicht mehr mit einer Rotausleuchtung belegt.");
    } else {
      sbkBlock_3_Info[Belegt] = 1;
      Serial.println("Sbk-3 ist nun mit einer Rotausleuchtung belegt.");
    }
  } else if (inputString == "Sbk4-Rot") {
    if (sbkBlock_4_Info[Belegt] == 1) {
      sbkBlock_4_Info[Belegt] = 0;
      Serial.println("Sbk-4 ist nun nicht mehr mit einer Rotausleuchtung belegt.");
    } else {
      sbkBlock_4_Info[Belegt] = 1;
      Serial.println("Sbk-4 ist nun mit einer Rotausleuchtung belegt.");
    }
  } else if (inputString == "EinfahrL-Rot") {
    if (einfahrBlock_Links_Info[Belegt] == 1) {
      einfahrBlock_Links_Info[Belegt] = 0;
      Serial.println("Einfahrblock -Links- ist nun nicht mehr mit einer Rotausleuchtung belegt.");
    } else {
      einfahrBlock_Links_Info[Belegt] = 1;
      Serial.println("Einfahrblock -Links- ist nun mit einer Rotausleuchtung belegt.");
    }
  } else if (inputString == "EinfahrR-Rot") {
    if (einfahrBlock_Rechts_Info[Belegt] == 1) {
      einfahrBlock_Rechts_Info[Belegt] = 0;
      Serial.println("Einfahrblock -Rechts- ist nun nicht mehr mit einer Rotausleuchtung belegt.");
    } else {
      einfahrBlock_Rechts_Info[Belegt] = 1;
      Serial.println("Einfahrblock -Rechts- ist nun mit einer Rotausleuchtung belegt.");
    }
  }
}

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
  if (SummerOff == 1){
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

// Liest die Taster aus und führt Funktionen aus.
void checkTaster() {
  if (digitalRead(HuT) == LOW){
    StatusSummer = 0;
  }
  if (digitalRead(BlSpT) == LOW) {
    if (digitalRead(Sig_Sbk1) == LOW) {
      signalSperren(1);
    }
    if (digitalRead(Sig_Sbk2) == LOW) {
      signalSperren(2);
    }
    if (digitalRead(Sig_Sbk3) == LOW) {
      signalSperren(3);
    }
    if (digitalRead(Sig_Sbk4) == LOW) {
      signalSperren(4);
    }
  }
  if (digitalRead(BlESpT) == LOW) {
    if (digitalRead(Sig_Sbk1) == LOW) {
      signalEntSperren(1);
    }
    if (digitalRead(Sig_Sbk2) == LOW) {
      signalEntSperren(2);
    }
    if (digitalRead(Sig_Sbk3) == LOW) {
      signalEntSperren(3);
    }
    if (digitalRead(Sig_Sbk4) == LOW) {
      signalEntSperren(4);
    }
  }
  if (digitalRead(HaGT) == LOW) {
    if (digitalRead(Sig_Sbk1) == LOW) {
      signalHalt(1);
    }
    if (digitalRead(Sig_Sbk2) == LOW) {
      signalHalt(2);
    }
    if (digitalRead(Sig_Sbk3) == LOW) {
      signalHalt(3);
    }
    if (digitalRead(Sig_Sbk4) == LOW) {
      signalHalt(4);
    }
  }
  if (digitalRead(ErsGT) == LOW) {
    if (digitalRead(Sig_Sbk1) == LOW) {
      signalZs(1);
    }
    if (digitalRead(Sig_Sbk2) == LOW) {
      signalZs(2);
    }
    if (digitalRead(Sig_Sbk3) == LOW) {
      signalZs(3);
    }
    if (digitalRead(Sig_Sbk4) == LOW) {
      signalZs(4);
    }
  }
}

// Funktion für die HaGT.
void signalHalt(int Sbk_Nummer) {
  if (Sbk_Nummer == 1 && sbkBlock_1_Info[ErsSignal] == 1) {
    sbkBlock_1_Info[ErsSignal] = 0;
  }
  if (Sbk_Nummer == 2 && sbkBlock_2_Info[ErsSignal] == 1) {
    sbkBlock_2_Info[ErsSignal] = 0;
  }
  if (Sbk_Nummer == 3 && sbkBlock_3_Info[ErsSignal] == 1) {
    sbkBlock_3_Info[ErsSignal] = 0;
  }
  if (Sbk_Nummer == 4 && sbkBlock_4_Info[ErsSignal] == 1) {
    sbkBlock_4_Info[ErsSignal] = 0;
  }
}

// Funktion für Ersatzsignal.
void signalZs(int Sbk_Nummer) {
  if (Sbk_Nummer == 1 && sbkBlock_1_Info[Fahrt] == 0 && sbkBlock_1_Info[ErsSignal] == 0) {
    sbkBlock_1_Info[ErsSignal] = 1;
    ErsGT_Zaehlwerk_Var++;
  }
  if (Sbk_Nummer == 2 && sbkBlock_2_Info[Fahrt] == 0 && sbkBlock_2_Info[ErsSignal] == 0) {
    sbkBlock_2_Info[ErsSignal] = 1;
    ErsGT_Zaehlwerk_Var++;
  }
  if (Sbk_Nummer == 3 && sbkBlock_3_Info[Fahrt] == 0 && sbkBlock_3_Info[ErsSignal] == 0) {
    sbkBlock_3_Info[ErsSignal] = 1;
    ErsGT_Zaehlwerk_Var++;
  }
  if (Sbk_Nummer == 4 && sbkBlock_4_Info[Fahrt] == 0 && sbkBlock_4_Info[ErsSignal] == 0) {
    sbkBlock_4_Info[ErsSignal] = 1;
    ErsGT_Zaehlwerk_Var++;
  }
}

// Funktion zum Sperren von Blocksignalen.
void signalSperren(int Sbk_Nummer) {
  if (Sbk_Nummer == 1 && sbkBlock_1_Info[Gesperrt] == 0) {
    sbkBlock_1_Info[Gesperrt] = 1;
    sbkBlock_1_Info[Fahrt] = 0;
  }
  if (Sbk_Nummer == 2 && sbkBlock_2_Info[Gesperrt] == 0) {
    sbkBlock_2_Info[Gesperrt] = 1;
    sbkBlock_2_Info[Fahrt] = 0;
  }
  if (Sbk_Nummer == 3 && sbkBlock_3_Info[Gesperrt] == 0) {
    sbkBlock_3_Info[Gesperrt] = 1;
    sbkBlock_3_Info[Fahrt] = 0;
  }
  if (Sbk_Nummer == 4 && sbkBlock_4_Info[Gesperrt] == 0) {
    sbkBlock_4_Info[Gesperrt] = 1;
    sbkBlock_4_Info[Fahrt] = 0;
  }
}

// Funktion zum Entsperren von Blocksignalen.
void signalEntSperren(int Sbk_Nummer) {
  if (Sbk_Nummer == 1 && sbkBlock_1_Info[Gesperrt] == 1) {
    sbkBlock_1_Info[Gesperrt] = 0;
    BlESpT_Zaehlwerk_Var++;
    sbkBlock_1_Info[Fahrt] = 1;
  }
  if (Sbk_Nummer == 2 && sbkBlock_2_Info[Gesperrt] == 1) {
    sbkBlock_2_Info[Gesperrt] = 0;
    BlESpT_Zaehlwerk_Var++;
    sbkBlock_2_Info[Fahrt] = 1;
  }
  if (Sbk_Nummer == 3 && sbkBlock_3_Info[Gesperrt] == 1) {
    sbkBlock_3_Info[Gesperrt] = 0;
    BlESpT_Zaehlwerk_Var++;
    sbkBlock_4_Info[Fahrt] = 1;
  }
  if (Sbk_Nummer == 4 && sbkBlock_4_Info[Gesperrt] == 1) {
    sbkBlock_4_Info[Gesperrt] = 0;
    BlESpT_Zaehlwerk_Var++;
    sbkBlock_4_Info[Fahrt] = 1;
  }
}

// Checkt ob das Gleis belegt ist und stellt das Signal auf Halt.
void checkBelegung() {
  if (sbkBlock_1_Info[Belegt] == 1) {
    sbkBlock_1_Info[Fahrt] = 0;
    sbkBlock_1_Info[ErsSignal] = 0;
  }
  if (sbkBlock_1_Info[Belegt] == 0 && sbkBlock_3_Info[Fahrt] == 0 && sbkBlock_1_Info[Gesperrt] == 0) {
    sbkBlock_1_Info[Fahrt] = 1;
  }
  if (sbkBlock_2_Info[Belegt] == 1) {
    sbkBlock_2_Info[Fahrt] = 0;
    sbkBlock_2_Info[ErsSignal] = 0;
  }
  if (sbkBlock_2_Info[Belegt] == 0 && sbkBlock_2_Info[Gesperrt] == 0) {
    sbkBlock_2_Info[Fahrt] = 1;
  }
  if (sbkBlock_3_Info[Belegt] == 1) {
    sbkBlock_3_Info[Fahrt] = 0;
    sbkBlock_3_Info[ErsSignal] = 0;
  }
  if (sbkBlock_3_Info[Belegt] == 0 && sbkBlock_3_Info[Gesperrt] == 0) {
    sbkBlock_3_Info[Fahrt] = 1;
  }
  if (sbkBlock_4_Info[Belegt] == 1) {
    sbkBlock_4_Info[Fahrt] = 0;
    sbkBlock_4_Info[ErsSignal] = 0;
  }
  if (sbkBlock_4_Info[Belegt] == 0 && sbkBlock_2_Info[Fahrt] == 0 && sbkBlock_4_Info[Gesperrt] == 0) {
    sbkBlock_4_Info[Fahrt] = 1;
  }
}

// Liest aus dem Info Array den aktuell Sataus und übeträgt diesen auf das Signalbild.
void updateSignalblockLED() {
  if (sbkBlock_1_Info[Belegt] == 0) {
    sbkBlock_1[Gleismelder_1] = CRGB::Black;
    sbkBlock_1[Gleismelder_2] = CRGB::Black;
  } else {
    sbkBlock_1[Gleismelder_1] = CRGB::Red;
    sbkBlock_1[Gleismelder_2] = CRGB::Red;
  }
  if (sbkBlock_1_Info[Fahrt] == 0) {
    sbkBlock_1[Haltmelder] = CRGB::Red;
    sbkBlock_1[Fahrtmelder] = CRGB::Black;
  } else {
    sbkBlock_1[Haltmelder] = CRGB::Black;
    sbkBlock_1[Fahrtmelder] = CRGB::Green;
    sbkBlock_1_Info[ErsSignal] = 0;
    sbkBlock_1[ZsMelder] = CRGB::Black;
  }
  if (sbkBlock_1_Info[Gesperrt] == 0) {
    sbkBlock_1[Sperrmelder] = CRGB::Black;
  } else {
    sbkBlock_1[Sperrmelder] = CRGB::Yellow;
  }
  if (sbkBlock_1_Info[ErsSignal] == 0) {
    sbkBlock_1[ZsMelder] = CRGB::Black;
  } else {
    sbkBlock_1[ZsMelder] = CRGB::White;
  }

  if (sbkBlock_2_Info[Belegt] == 0) {
    sbkBlock_2[Gleismelder_1] = CRGB::Black;
    sbkBlock_2[Gleismelder_2] = CRGB::Black;
  } else {
    sbkBlock_2[Gleismelder_1] = CRGB::Red;
    sbkBlock_2[Gleismelder_2] = CRGB::Red;
  }
  if (sbkBlock_2_Info[Fahrt] == 0) {
    sbkBlock_2[Haltmelder] = CRGB::Red;
    sbkBlock_2[Fahrtmelder] = CRGB::Black;
  } else {
    sbkBlock_2[Haltmelder] = CRGB::Black;
    sbkBlock_2[Fahrtmelder] = CRGB::Green;
    sbkBlock_2_Info[ErsSignal] = 0;
    sbkBlock_2[ZsMelder] = CRGB::Black;
  }
  if (sbkBlock_2_Info[Gesperrt] == 0) {
    sbkBlock_2[Sperrmelder] = CRGB::Black;
  } else {
    sbkBlock_2[Sperrmelder] = CRGB::Yellow;
  }
  if (sbkBlock_2_Info[ErsSignal] == 0) {
    sbkBlock_2[ZsMelder] = CRGB::Black;
  } else {
    sbkBlock_2[ZsMelder] = CRGB::White;
  }

  if (sbkBlock_3_Info[Belegt] == 0) {
    sbkBlock_3[Gleismelder_1] = CRGB::Black;
    sbkBlock_3[Gleismelder_2] = CRGB::Black;
  } else {
    sbkBlock_3[Gleismelder_1] = CRGB::Red;
    sbkBlock_3[Gleismelder_2] = CRGB::Red;
  }
  if (sbkBlock_3_Info[Fahrt] == 0) {
    sbkBlock_3[Haltmelder] = CRGB::Red;
    sbkBlock_3[Fahrtmelder] = CRGB::Black;
  } else {
    sbkBlock_3[Haltmelder] = CRGB::Black;
    sbkBlock_3[Fahrtmelder] = CRGB::Green;
    sbkBlock_3_Info[ErsSignal] = 0;
    sbkBlock_3[ZsMelder] = CRGB::Black;
  }
  if (sbkBlock_3_Info[Gesperrt] == 0) {
    sbkBlock_3[Sperrmelder] = CRGB::Black;
  } else {
    sbkBlock_3[Sperrmelder] = CRGB::Yellow;
  }
  if (sbkBlock_3_Info[ErsSignal] == 0) {
    sbkBlock_3[ZsMelder] = CRGB::Black;
  } else {
    sbkBlock_3[ZsMelder] = CRGB::White;
  }

  if (sbkBlock_4_Info[Belegt] == 0) {
    sbkBlock_4[Gleismelder_1] = CRGB::Black;
    sbkBlock_4[Gleismelder_2] = CRGB::Black;
  } else {
    sbkBlock_4[Gleismelder_1] = CRGB::Red;
    sbkBlock_4[Gleismelder_2] = CRGB::Red;
  }
  if (sbkBlock_4_Info[Fahrt] == 0) {
    sbkBlock_4[Haltmelder] = CRGB::Red;
    sbkBlock_4[Fahrtmelder] = CRGB::Black;
  } else {
    sbkBlock_4[Haltmelder] = CRGB::Black;
    sbkBlock_4[Fahrtmelder] = CRGB::Green;
    sbkBlock_4_Info[ErsSignal] = 0;
    sbkBlock_4[ZsMelder] = CRGB::Black;
  }
  if (sbkBlock_4_Info[Gesperrt] == 0) {
    sbkBlock_4[Sperrmelder] = CRGB::Black;
  } else {
    sbkBlock_4[Sperrmelder] = CRGB::Yellow;
  }
  if (sbkBlock_4_Info[ErsSignal] == 0) {
    sbkBlock_4[ZsMelder] = CRGB::Black;
  } else {
    sbkBlock_4[ZsMelder] = CRGB::White;
  }
}

void updateEinfahrblockLED() {
  if (einfahrBlock_Links_Info[Belegt] == 0) {
    einfahrBlock_Links[0] = CRGB::Black;
  } else {
    einfahrBlock_Links[0] = CRGB::Red;
  }
  if (einfahrBlock_Rechts_Info[Belegt] == 0) {
    einfahrBlock_Rechts[0] = CRGB::Black;
  } else {
    einfahrBlock_Rechts[0] = CRGB::Red;
  }

}

// Funktion zum Updaten der Zählwerke.
void updateZaehlwerke() {
  ErsGT_Zaehlwerk.showNumberDec(ErsGT_Zaehlwerk_Var);
  BlESpT_Zaehlwerk.showNumberDec(BlESpT_Zaehlwerk_Var);

  if (ErsGT_Zaehlwerk_Var != ErsGT_Zaehlwerk_Var_Old){
     ErsGT_Zaehlwerk_Var_Old = ErsGT_Zaehlwerk_Var;
     EEPROMWriteInt(0,ErsGT_Zaehlwerk_Var);
     Serial.println("Zählwerk ErsGT geupdatet in ROM.");
  }
  if (BlESpT_Zaehlwerk_Var != BlESpT_Zaehlwerk_Var_Old){
     BlESpT_Zaehlwerk_Var_Old = BlESpT_Zaehlwerk_Var;
     EEPROMWriteInt(2,BlESpT_Zaehlwerk_Var);
     Serial.println("Zählwerk BlESpT geupdatet in ROM.");   
  }
}

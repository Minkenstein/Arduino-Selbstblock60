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
  if (autoTrain == 1) {
    nextTrain();
  }
  FastLED.show();
}

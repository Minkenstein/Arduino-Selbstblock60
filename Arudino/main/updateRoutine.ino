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

  if (ErsGT_Zaehlwerk_Var != ErsGT_Zaehlwerk_Var_Old) {
    ErsGT_Zaehlwerk_Var_Old = ErsGT_Zaehlwerk_Var;
    EEPROMWriteInt(0, ErsGT_Zaehlwerk_Var);
    Serial.println("Zählwerk ErsGT geupdatet in ROM.");
  }
  if (BlESpT_Zaehlwerk_Var != BlESpT_Zaehlwerk_Var_Old) {
    BlESpT_Zaehlwerk_Var_Old = BlESpT_Zaehlwerk_Var;
    EEPROMWriteInt(2, BlESpT_Zaehlwerk_Var);
    Serial.println("Zählwerk BlESpT geupdatet in ROM.");
  }
}

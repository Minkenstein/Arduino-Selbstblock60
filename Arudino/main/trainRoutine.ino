void nextTrain() {
  if (ZugLinksStatus == 0 && nextTrainLinks < millis()) {
    ZugLinksStatus = 1;
  }
  if (ZugRechtsStatus == 0 && nextTrainRechts < millis()) {
    ZugRechtsStatus = 1;
  }
}

void driveTrainLinks() {
  if (ZugLinksStatus == 0) {
    return;
  }
  if (ZugLinksStatus == 1) {
    einfahrBlock_Links_Info[Belegt] = 1;
    StatusSummer = 2;
    ZugLinksMillis = millis() + 45000;
    ZugLinksStatus = 2;
    Serial.println("Zug von Links in Block eingefahren.");
  }
  if (ZugLinksStatus == 2) {
    if (ZugLinksMillis < millis()) {
      if (sbkBlock_1_Info[Fahrt] == 1 || sbkBlock_1_Info[ErsSignal] == 1) {
        if (ZugLinksWarten < millis()) {
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
  if (ZugLinksStatus == 3) {
    if (ZugLinksMillis < millis()) {
      einfahrBlock_Links_Info[Belegt] = 0;
      ZugLinksStatus = 4;
      ZugLinksMillis = millis() + 36000;
    }
  }
  if (ZugLinksStatus == 4) {
    if (ZugLinksMillis < millis()) {
      if (sbkBlock_3_Info[Fahrt] == 1 || sbkBlock_3_Info[ErsSignal] == 1) {
        if (ZugLinksWarten < millis()) {
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
  if (ZugLinksStatus == 5) {
    if (ZugLinksMillis < millis()) {
      sbkBlock_1_Info[Belegt] = 0;
      ZugLinksStatus = 6;
      ZugLinksMillis = millis() + 36000;
    }
  }
  if (ZugLinksStatus == 6) {
    if (ZugLinksMillis < millis()) {
      sbkBlock_3_Info[Belegt] = 0;
      ZugLinksStatus = 0;
      Serial.println("Zug nach Rechts ausgefahren.");
      nextTrainLinks = millis() + random(30000, 90000);
    }
  }
}

void driveTrainRechts() {
  if (ZugRechtsStatus == 0) {
    return;
  }
  if (ZugRechtsStatus == 1) {
    einfahrBlock_Rechts_Info[Belegt] = 1;
    StatusSummer = 2;
    ZugRechtsMillis = millis() + 45000;
    ZugRechtsStatus = 2;
    Serial.println("Zug von Rechts in Block eingefahren.");
  }
  if (ZugRechtsStatus == 2) {
    if (ZugRechtsMillis < millis()) {
      if (sbkBlock_4_Info[Fahrt] == 1 || sbkBlock_4_Info[ErsSignal] == 1) {
        if (ZugRechtsWarten < millis()) {
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
  if (ZugRechtsStatus == 3) {
    if (ZugRechtsMillis < millis()) {
      einfahrBlock_Rechts_Info[Belegt] = 0;
      ZugRechtsStatus = 4;
      ZugRechtsMillis = millis() + 36000;
    }
  }
  if (ZugRechtsStatus == 4) {
    if (ZugRechtsMillis < millis()) {
      if (sbkBlock_2_Info[Fahrt] == 1 || sbkBlock_2_Info[ErsSignal] == 1) {
        if (ZugRechtsWarten < millis()) {
          sbkBlock_2_Info[Belegt] = 1;
          ZugRechtsStatus = 5;
          ZugRechtsMillis = millis() + 9000;
        }
      } else {
        ZugRechtsWarten = millis() + 8000;
      }
    }
  }
  if (ZugRechtsStatus == 5) {
    if (ZugRechtsMillis < millis()) {
      sbkBlock_4_Info[Belegt] = 0;
      ZugRechtsStatus = 6;
      ZugRechtsMillis = millis() + 36000;
    }
  }
  if (ZugRechtsStatus == 6) {
    if (ZugRechtsMillis < millis()) {
      sbkBlock_2_Info[Belegt] = 0;
      ZugRechtsStatus = 0;
      Serial.println("Zug nach Links ausgefahren.");
      nextTrainRechts = millis() + random(30000, 90000);
    }
  }
}

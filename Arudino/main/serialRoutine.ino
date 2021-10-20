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
    if (SummerOff == 1) {
      SummerOff = 0;
      Serial.println("Summer ist jetzt an.");
    } else {
      SummerOff = 1;
      Serial.println("Summer ist jetzt aus.");
    }
  } else if (inputString == "AutoTrain") {
    if (autoTrain == 1) {
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

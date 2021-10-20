void checkTaster() {
  if (digitalRead(HuT) == LOW) {
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
  if (Sbk_Nummer == 1 && sbkBlock_1_Info[Fahrt] == 0 && sbkBlock_1_Info[ErsSignal] == 0 && einfahrBlock_Links_Info[Belegt] == 1) {
    sbkBlock_1_Info[ErsSignal] = 1;
    ErsGT_Zaehlwerk_Var++;
  }
  if (Sbk_Nummer == 2 && sbkBlock_2_Info[Fahrt] == 0 && sbkBlock_2_Info[ErsSignal] == 0 && sbkBlock_4_Info[Belegt] == 1) {
    sbkBlock_2_Info[ErsSignal] = 1;
    ErsGT_Zaehlwerk_Var++;
  }
  if (Sbk_Nummer == 3 && sbkBlock_3_Info[Fahrt] == 0 && sbkBlock_3_Info[ErsSignal] == 0 && sbkBlock_1_Info[Belegt] == 1) {
    sbkBlock_3_Info[ErsSignal] = 1;
    ErsGT_Zaehlwerk_Var++;
  }
  if (Sbk_Nummer == 4 && sbkBlock_4_Info[Fahrt] == 0 && sbkBlock_4_Info[ErsSignal] == 0 && einfahrBlock_Rechts_Info[Belegt] == 1) {
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

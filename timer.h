#pragma once
// Temporisation/base de temps
// réglage de la minuterie

void initTimer() {
  TCCR1A = 0;
  // CTC par OCR1A, diviseur /64
  TCCR1B = bit(WGM12) | 0b011;
}

//  définition de la periode
void setPeriod(uint32_t prd) {
  // un bit de minuterie a 4 µs (avec une horloge de 16 MHz)
  OCR1A = (uint32_t)prd >> 2;
}

// démarrer et réinitialiser la minuterie
void startTimer() {
  TIMSK1 = bit(OCIE1A);
  TCNT1 = 0;
}

// arrêter le chronomètre
void stopTimer() {
  TIMSK1 = 0;
  TCNT1 = 0;
}

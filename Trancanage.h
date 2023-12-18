#pragma once
// parametres d'un Bobinage.

#include "EEPROM.h" // appel bibliothéque eeprom

struct Trancanage {
  int16_t tours = 0;
  int16_t step = 0;
  int16_t speed = 30;
  int16_t Couches = 0;
  bool dir = 1;
};

void Load(Trancanage &o, int &p) {
  EEPROM_load(p, o.tours);
  EEPROM_load(p, o.step);
  EEPROM_load(p, o.speed);
  EEPROM_load(p, o.Couches);
  EEPROM_load(p, o.dir);
}

void Save(const Trancanage &o, int &p) {
  EEPROM_save(p, o.tours);
  EEPROM_save(p, o.step);
  EEPROM_save(p, o.speed);
  EEPROM_save(p, o.Couches);
  EEPROM_save(p, o.dir);
}

struct Settings {
  bool stopPerCouche = 0;
  uint8_t VisChariotStep = 1;
  uint8_t CoucheStep = 1;
  int ChariotPosition = 0;
  int CouchePos = 0;
  int16_t acceleration = 0;
  int8_t actuelTransformer = 0;
};

void Load(Settings &o, int &p) {
  EEPROM_load(p, o.stopPerCouche);
  EEPROM_load(p, o.acceleration);  
  EEPROM_load(p, o.actuelTransformer);
}

void Save(const Settings &o, int &p) {
  EEPROM_save(p, o.stopPerCouche);
  EEPROM_save(p, o.acceleration);
  EEPROM_save(p, o.actuelTransformer);
}

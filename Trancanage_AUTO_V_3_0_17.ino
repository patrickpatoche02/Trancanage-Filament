/*         TRANCANAGE AUTOMATIQUE DE BOBINE DE FILAMENTS ou autres  de 0.001 a 3 mm de diamétre.
           Version 3.0.17
           Compatible IDE 2.2.1

            -j ai volontairement bridé : le nombre de tours(spire) a 60 par couche , le changer si besoin en ligne 203.
                                         le nombre de couche a 99 , le changer si besoin en ligne 204.
                                         le diamétre du fil a 3mm ,le changer si besoin en ligne 205 (3000 pour 3mm).
                         
   Description du materiel :  1 Arduino  Uno 
                              1 Stepper motor control CNC Shield v3 
                              3 drivers moteur pas a pas style " A4988 "(le troisieme n'est pas obligatoire,freinage de la bobine doneuse)
                              1 LCD I2C 2004  ou LCD 2004 standart (I2C moins de cablage) 
                              1 Encodeur KY-040 
                              3 moteurs pas a pas type Néma 17 (le troisiéme n'est pas obligatoire , freinage de la bobine doneuse)
                              1 alim 24 VDC 5 A pour la puissance des moteur Nema 
                              1 alim 9 VDC 1 A pour l'arduino Uno
                              1 buzzer (pas obligatoire) 1 bip = demarrage trancanage, 5 bip = trancanage terminé
                              1 interupteur (pas obligatoire mais préférable) ne pas oublier de le shunter si pas utiliser
                              2 fins de courses (pas obligatoire) mais plus securitaire pour le chariot
                              1 capteur de filament (pas obligatoire)mais pas d'arret en cas de manque de fil.

   Reglage A4988 : https://www.e-techno-tutos.com/2018/07/17/arduino-cnc-reglage-des-pololu-a4988/

   Arret :par fin du programme , par changement d'état de l'interupteur ,par action sur un fins de courses ,par manque de filament


         Arduino UNO connection diagramme:
          _______________________________
         |             USB               |
         |                           AREF|
         |                            GND|
         |                             13| PIN DIR A          Nema Trancanage (Chariot)
         |RESET                        12| PIN STEP A         Nema Trancanage (Chariot)
         |3V3                          11| PIN STOP BT        Bouton Stop/Pause/Départ ,a shunter avec gnd si pas utilisé,je l'ai mis en série avec les fins de courses chariot et le capteur filament
         |5V           U               10| BUZ OUT            Buzzer
         |GND          N                9|
         |VIN          O                8| PIN EN STEP        Enable Moteur Trancanage et Bobine
         |                               |
         |                              7| PIN DIR Z          Nema Bobine receptrice
         |                              6| PIN                Capteur de filament
         |A0                            5| ENCODEUR PIN CLK   Encodeur rotatif
         |A1                            4| PIN STEP Z         Nema Bobine receptrice
         |A2                            3| ENCODEUR PIN SW    Encodeur rotatif
         |A3                            2| ENCODEUR PIN DT    Encodeur rotatif
 LCD SDA |A4                         TX 1|
 LCD SLC |A5                         RX 0|
         |_______________________________|


 exemple si dessus de cablage pour conf avec ecran I2C

Dévelopé avec la version IDE Arduino 1.8.19

/////////////////////////////////////////////////////////
//////        IMPORTANT       ///////////////////////////
""""""Ne pas faire la mise a jour des bibliotheques""""""
/////         IMPORTANT       ///////////////////////////
/////////////////////////////////////////////////////////
_________________________________________________________

Bibliothèques requises

Avant le premier build, installer toutes les bibliothèques nécessaires à partir du gestionnaire de bibliothèques Arduino IDE 

    LiquidCrystal 1.0.7 ou version ultérieure ou LiquidCrystal_I2C by Frank de Brabander V1.1.2 , selon la connexion de l'écran
    GyverStepper by AlexGyver 2.6.4 
    EnсButton by AlexGyver 2.0.0 
    AnalogKey by AlexGyver 1.1.0 

    Modifier le fichier config.h pour le mettre en adéquation avec votre materiel et la langue désirée (je n'ai pas fini l'Anglais a essayer et corriger (Textes.h) si besoin).


Des ennuis :

    Rien ne tourne et en pause - l'interupteur n'est pas actionné, actionner ou fermé le contact avec la masse GND.
    L'écran est sombre et rien n'est visible - connectez les broches d'affichage A et K via une résistance à la tension de rétroéclairage (version LCD non I2C)
    Déchets sur l'écran : vérifiez que la méthode de connexion de l'écran correspond aux paramètres du micrologiciel config.h
    L'encodeur saute un clic sur deux - modifier le type d'encodeur dans les paramètres du micrologiciel config.h
    Le programme ne demare pas , les parametres de configuration du bobinage doivent etre renseignés.
    erreur compilation, vérifier les bibliothéques installées
    Faites attention au type d'encodeur (ENCODER_TYPE). Si vous utilisez un encodeur sans résistance il "sautera" et vous devrai changer son type dans le fichier config.h
    Vérifiez le pas de filetage de l'arbre de l'enrouleur (THREAD_PITCH), il est maintenant en µm config.h

*/

#include "config.h"  // a configurer suivant votre materiel
#include "debug.h"   // pour informations par le port serie

#define FPSTR(pstr) (const __FlashStringHelper *)(pstr)
#define LENGTH(a) (sizeof(a) / sizeof(*a))

#if DISPLAY_I2C == 1
#include <LiquidCrystal_I2C.h>  //a installer: bibliotheque LiquidCrystal I2C by Frank de Brabander V 1.1.2
#else
#include <LiquidCrystal.h>
#endif
#include <EncButton.h>         // a installer: bibliotheque EncBouton by AlexGyver V 2.0
#include <AnalogKey.h>         // a installer: bibliotheque AnalogKey by AlexGyver V 1.1
#include <GyverPlanner.h>      // Steper
#include <GyverStepper2.h>     // a installer: bibliotheque GyverStepper by AlexGyver V 2.6.4
#include "LiquidCrystalCyr.h"  // affichage
#include "Menu.h"              // arborescence menu
#include "timer.h"             // base de temps
#include "buzzer.h"            // signal sonore

#include "Screen.h"
#include "Trancanage.h"
#include "Textes.h"

#ifndef STEPPER_Chariot_STEPS
#define STEPPER_Chariot_STEPS STEPPER_STEPS
#endif
#ifndef STEPPER_Bobine_STEPS
#define STEPPER_Bobine_STEPS STEPPER_STEPS
#endif
#ifndef STEPPER_Chariot_MICROSTEPS
#define STEPPER_Chariot_MICROSTEPS STEPPER_MICROSTEPS
#endif
#ifndef STEPPER_Bobine_MICROSTEPS
#define STEPPER_Bobine_MICROSTEPS STEPPER_MICROSTEPS
#endif
#ifndef STEPPER_Bobine_REVERSE
#define STEPPER_Bobine_REVERSE 0
#endif
#ifndef STEPPER_Chariot_REVERSE
#define STEPPER_Chariot_REVERSE 0
#endif
#ifndef MEMOIRE_COMPTEUR
#define MEMOIRE_COMPTEUR 3
#endif

#define STEPPER_Bobine_STEPS_COUNT (float(STEPPER_Bobine_STEPS) * STEPPER_Bobine_MICROSTEPS)
#define STEPPER_Chariot_STEPS_COUNT (float(STEPPER_Chariot_STEPS) * STEPPER_Chariot_MICROSTEPS)
#define STEPPER_VITESSE_LIMITE 18000
#ifdef GS_FAST_PROFILE
#define PLANNER_VITESSE_LIMITE 37000
#else
#define PLANNER_VITESSE_LIMITE 14000
#endif
#define VITESSE_LIMITE (int32_t(PLANNER_VITESSE_LIMITE) * 60 / STEPPER_Bobine_STEPS_COUNT)
#define SPEED_INC 10
#define STEPPER_Bobine_MANUEL_VITESSE 360
#define STEPPER_Chariot_MANUEL_VITESSE ((int)(360L * 1000 / PasFiletage))

#define EEPROM_SETTINGS_VERSION 2
#define EEPROM_TrancanageS_VERSION 2
#define EEPROM_SETTINGS_ADDR 0x00
#define EEPROM_TrancanageS_ADDR 0x10
#define EEPROM_TrancanageS_CLASTER (sizeof(Trancanage) * Trancanage_COUNT + 1)

#define Trancanage_COUNT 3


Trancanage params[Trancanage_COUNT];

int8_t actuelTrancanage = 0;

Settings settings;

enum menu_states {
  AutoTrancanage1,
  actuelTrans,
  PositionControl,
  miSettings,
  Trancanage1,
  Trancanage2,
  Trancanage3,
  StartAll,
  TrancanageBack,
  toursSet,
  LaySet,
  StepSet,
  SpeedSet,
  Direction,
  Start,
  Cancel,
  ChariotPosition,
  VisChariotStepMul,
  CouchePos,
  CoucheStepMul,
  PosCancel,
  miSettingsStopPerLevel,
  AccelSet,
  miSettingsBack
};  // Liste numérotée des lignes d'écran

const char *boolSet[] = { STRING_OFF, STRING_ON };
const char *dirSet[] = { "<<<", ">>>" };  // direction
const uint8_t stepSet[] = { 1, 10, 100 }; // multiplicateur

MenuItem *menuItems[] = {
  new MenuItem(0, 0, MENU_01),// trancaneuse
  new ByteMenuItem(0, 1, MENU_02, MENU_FORMAT_02, &settings.actuelTransformer, 1, MEMOIRE_COMPTEUR),// programme
  new MenuItem(0, 2, MENU_04),// position
  new MenuItem(0, 3, MENU_05),// parametres

  new ValMenuItem(1, 0, MENU_06, MENU_FORMAT_06),// prg 1
  new ValMenuItem(1, 1, MENU_07, MENU_FORMAT_06),// prg 2
  new ValMenuItem(1, 2, MENU_08, MENU_FORMAT_06),// prg 3
  new MenuItem(1, 3, MENU_15),// start
  new MenuItem(1, 4, MENU_09),// retour

  new IntMenuItem(2, 0, MENU_10, MENU_FORMAT_10, NULL, 1, 60),// nb de tour // "   %03d" nb de tour sur une couche max a 60
  new IntMenuItem(2, 1, MENU_13, MENU_FORMAT_13, NULL, 1, 99),// nombre de couche de spire // "   %02d" nb de couche max a 99
  new FloatMenuItem(2, 2, MENU_11, MENU_FORMAT_11, NULL, 5, 3000, 5),// espacement spire du trancanage // "   %d.%03d" maxi 3 mm soit 3000
  new IntMenuItem(2, 3, MENU_12, MENU_FORMAT_10, NULL, SPEED_INC, VITESSE_LIMITE, SPEED_INC),// rpm // "   %03d"
  new BoolMenuItem(2, 4, MENU_14, NULL, dirSet),// direction du sens du trancanage
  new MenuItem(2, 5, MENU_15),// start
  new MenuItem(2, 6, MENU_09),// retour

  new IntMenuItem(10, 0, MENU_17, MENU_FORMAT_17, &settings.ChariotPosition, -9999, 9999),// ajustage de la position de la bobine
  new SetMenuItem(10, 1, MENU_18, MENU_FORMAT_10, &settings.VisChariotStep, stepSet, 3),// multiplicateur
  new IntMenuItem(10, 2, MENU_19, MENU_FORMAT_17, &settings.CouchePos, -9999, 9999),// ajustage de la position du chariot trancanage
  new SetMenuItem(10, 3, MENU_18, MENU_FORMAT_10, &settings.CoucheStep, stepSet, 3),// multiplicateur
  new MenuItem(10, 4, MENU_09),// retour

  new BoolMenuItem(11, 0, MENU_22, &settings.stopPerCouche, boolSet),// changement de couche stop
  new IntMenuItem(11, 1, MENU_23, MENU_FORMAT_10, &settings.acceleration, 0, 600, 1),// acceleration et decceleration debut bobinage et fin
  new MenuItem(11, 2, MENU_09),// retour
};

byte up[8] = { 0b00100, 0b01110, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 };    // Création du symbole ⯅ pour l'ecran LCD
byte down[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b01110, 0b00100 };  // Création du symbole ⯆ pour l'écran LCD

#if DISPLAY_I2C == 0
LiquidCrystalCyr lcd(DISPLAY_RS, DISPLAY_EN, DISPLAY_D4, DISPLAY_D5, DISPLAY_D6, DISPLAY_D7);  // Attribution des broches pour l'écran LCD non I2C
#else
LiquidCrystalCyr lcd(DISPLAY_ADDRESS, DISPLAY_NCOL, DISPLAY_NROW); // adressage affectation pour LCD I2C
#endif

MainMenu menu(menuItems, LENGTH(menuItems), lcd);
MainScreen screen(lcd);


GStepper2<STEPPER2WIRE> ChariotStepper(STEPPER_Bobine_STEPS_COUNT, STEPPER_STEP_Bobine, STEPPER_DIR_Bobine, STEPPER_EN);
GStepper2<STEPPER2WIRE> CoucheStepper(STEPPER_Chariot_STEPS_COUNT, STEPPER_STEP_Chariot, STEPPER_DIR_Chariot, STEPPER_EN);
GPlanner<STEPPER2WIRE, 2> planner;

EncButton<EB_TICK, ENCODER_CLK, ENCODER_DT, ENCODER_SW> encoder(ENCODER_INPUT);
EncButton<EB_TICK, Bouton_STOP> pedale;

EncButton<EB_TICK, Capteur_Filament> capteurstop;

Buzzer buzzer(BUZZER);

enum { BoutonLEFT,
       BoutonUP,
       BoutonDOWN,
       BoutonRIGHT,
       BoutonSELECT
};
int16_t key_signals[] = { KEYBOARD_LEFT, KEYBOARD_UP, KEYBOARD_DOWN, KEYBOARD_RIGHT, KEYBOARD_SELECT };
AnalogKey<KEYBOARD_PIN, LENGTH(key_signals), key_signals> keyboard;


void setup() {
  Serial.begin(9600);
  LoadSettings();

  CoucheStepper.disable();
  ChariotStepper.disable();
  CoucheStepper.reverse(!STEPPER_Bobine_REVERSE);
  ChariotStepper.reverse(!STEPPER_Chariot_REVERSE);
  planner.addStepper(0, ChariotStepper);
  planner.addStepper(1, CoucheStepper);

  lcd.createChar(0, up);
  lcd.createChar(1, down);
  lcd.begin(DISPLAY_NCOL, DISPLAY_NROW);
  menu.Draw();

  encoder.setEncType(ENCODER_TYPE);
}

void loop() {
  encoder.tick();
  KeyboardRead();

  if (encoder.turn()) {
    menu.IncIndex(encoder.dir());  // Si la position de l'encodeur est modifiée, alors modifie le menu index et affiche l'écran
    menu.Draw();
  }

  if (encoder.click()) {
    switch (menu.index)  // S'il y a eu un clic, effectuer l'action correspondante à la position actuelle du curseur
    {
      case AutoTrancanage1:
        SaveSettings();
        LoadTrancanages();
        menu.index = Trancanage1;

        UpdateMenuItemText(0);
        UpdateMenuItemText(1);
        UpdateMenuItemText(2);
        break;

      case Trancanage1:
      case Trancanage2:
      case Trancanage3:
        actuelTrancanage = menu.index - Trancanage1;
        menu.index = toursSet;
        ((IntMenuItem *)menu[toursSet])->value = &params[actuelTrancanage].tours;
        ((IntMenuItem *)menu[StepSet])->value = &params[actuelTrancanage].step;
        ((IntMenuItem *)menu[SpeedSet])->value = &params[actuelTrancanage].speed;
        ((IntMenuItem *)menu[LaySet])->value = &params[actuelTrancanage].Couches;
        ((BoolMenuItem *)menu[Direction])->value = &params[actuelTrancanage].dir;
        break;
      case TrancanageBack:
        menu.index = AutoTrancanage1;
        break;
      case PositionControl:
        menu.index = ChariotPosition;
        break;
      case toursSet:
      case StepSet:
      case SpeedSet:
      case LaySet:
      case AccelSet:
        ValueEdit();
        break;
      case actuelTrans:
      case miSettingsStopPerLevel:
      case Direction:
        menu.Incactuel(1, true);
        break;
      case StartAll:
        AutoTrancanageAll(params, Trancanage_COUNT);
        menu.Draw(true);
        break;
      case Start:
        SaveTrancanages();
        AutoTrancanageAll(params + actuelTrancanage, 1);
        menu.index = Trancanage1 + actuelTrancanage;
        UpdateMenuItemText(actuelTrancanage);
        break;
      case Cancel:
        SaveTrancanages();
        menu.index = Trancanage1 + actuelTrancanage;
        UpdateMenuItemText(actuelTrancanage);
        break;

      case ChariotPosition:
      case CouchePos:
        MoveTo((menu.index == CouchePos) ? CoucheStepper : ChariotStepper, *((IntMenuItem *)menu[menu.index])->value);
        break;

      case VisChariotStepMul:
      case CoucheStepMul:
        menu.Incactuel(1, true);
        ((IntMenuItem *)menu[menu.index - 1])->increment = *((SetMenuItem *)menu[menu.index])->value;
        break;
      case PosCancel:
        menu.index = PositionControl;
        settings.ChariotPosition = 0;
        settings.CouchePos = 0;
        break;

      case miSettings:
        menu.index = miSettingsStopPerLevel;
        break;

      case miSettingsBack:
        SaveSettings();
        menu.index = miSettings;
        break;
    }
    menu.Draw();
  }
}

void UpdateMenuItemText(byte i) {
  ((ValMenuItem *)menu[Trancanage1 + i])->value = params[i].tours * params[i].Couches;
}

void ValueEdit() {
  menu.DrawQuotes(1);
  do {
    encoder.tick();

    if (encoder.turn() || encoder.turnH())
      menu.Incactuel(encoder.dir() * (encoder.state() ? 10 : 1));

  } while (!encoder.click());
  menu.DrawQuotes(0);
}

void MoveTo(GStepper2<STEPPER2WIRE> &stepper, int &pos) {
  menu.DrawQuotes(1);
  stepper.enable();

  stepper.setAcceleration(STEPPER_Bobine_STEPS_COUNT * settings.acceleration / 60);
  stepper.setMaxSpeed(constrain(STEPPER_Bobine_STEPS_COUNT / 2, 1, STEPPER_VITESSE_LIMITE));

  int o = pos;
  stepper.reset();

  do {
    stepper.tick();
    encoder.tick();

    if (encoder.turn()) {
      menu.Incactuel(encoder.dir());
     stepper.setTargetDeg(pos - o);
    }

  } while (!encoder.click() || stepper.getStatus() != 0);

  stepper.disable();
  menu.DrawQuotes(0);
}

void KeyboardRead() {
  if (KEYBOARD_PIN == 0 || KEYBOARD_PIN == A6)
    return;

  static int8_t oldKey = -1;
  int8_t key = keyboard.pressed();

  if (oldKey != key) {
    switch (key) {
      case BoutonLEFT:
        CoucheStepper.enable();
        CoucheStepper.setSpeedDeg(STEPPER_Chariot_MANUEL_VITESSE);
        break;
      case BoutonRIGHT:
        CoucheStepper.enable();
        CoucheStepper.setSpeedDeg(-STEPPER_Chariot_MANUEL_VITESSE);
        break;
      case BoutonUP:
        ChariotStepper.enable();
        ChariotStepper.setSpeedDeg(STEPPER_Bobine_MANUEL_VITESSE);
        break;
      case BoutonDOWN:
        ChariotStepper.enable();
        ChariotStepper.setSpeedDeg(-STEPPER_Bobine_MANUEL_VITESSE);
        break;
      case BoutonSELECT: break;
      default:
        CoucheStepper.brake();
        ChariotStepper.brake();
        CoucheStepper.disable();
        ChariotStepper.disable();
    }
    oldKey = key;
  }

  if (CoucheStepper.getStatus())
    CoucheStepper.tick();
  if (ChariotStepper.getStatus())
    ChariotStepper.tick();
}


double speedMult = 1;

ISR(TIMER1_COMPA_vect) {
  if (planner.tickManual())
    setPeriod(planner.getPeriod() * speedMult);
  else
    stopTimer();
}

uint32_t getSpeed() {
  uint32_t p = planner.getPeriod() * speedMult;
  return (p == 0) ? 0 : (60000000ul / (STEPPER_Bobine_STEPS_COUNT * p));
}


void AutoTrancanage(const Trancanage &w, bool &direction)  // Routine de trancanage automatique
{
  Trancanage actuel;  // Rotation et couche actuelles pendant l'enroulement automatique

  DebugWrite("Start");

  actuel.tours = 0;
  actuel.Couches = 0;
  actuel.speed = w.speed;
  speedMult = 1;
  actuel.dir = w.dir;
  actuel.step = w.step;

  screen.Draw();

  pedale.tick();
  capteurstop.tick();
  bool run = pedale.state()&capteurstop.state();


  

  ChariotStepper.enable();
  CoucheStepper.enable();

  planner.setAcceleration(STEPPER_Bobine_STEPS_COUNT * settings.acceleration / 60L);
  planner.setMaxSpeed(constrain(STEPPER_Bobine_STEPS_COUNT * w.speed / 60L, 1, PLANNER_VITESSE_LIMITE));

  int32_t dVisChariot = STEPPER_Bobine_STEPS_COUNT * w.tours;
  int32_t dCouche = STEPPER_Chariot_STEPS_COUNT * w.tours * w.step / int32_t(PasFiletage) * (direction ? 1 : -1);
  int32_t p[] = { dVisChariot, dCouche };

  planner.reset();
  initTimer();

  while (1) {
    if (run && !planner.getStatus()) {
      DebugWrite("PRET");
      if (actuel.Couches >= w.Couches)
        break;

      if (settings.stopPerCouche && (actuel.Couches > 0)) {
        screen.Message(STRING_2);  // "APPUYER SUR CONTINUER  "
        WaitBouton();
        screen.Draw();
      }

      DebugWrite("setTarget", p[0], p[1]);
      planner.setTarget(p, RELATIVE);
      ++actuel.Couches;
      p[1] = -p[1];
      direction = !direction;

      startTimer();
      setPeriod(planner.getPeriod() * speedMult);

      screen.UpdateCouches(actuel.Couches);
    }

    encoder.tick();
    pedale.tick();
    capteurstop.tick();

    bool oldState = run;
    if (pedale.press() || pedale.release())
      run = pedale.state()&capteurstop.state();
    else if (pedale.state() && encoder.click())
      run = !run;

    if (run != oldState) {
      if (run) {
        if (actuel.Couches) {  // si aucune selection n'est faite, alors on ne peut pas lancer le Start (les parametres tour/pas/couche/vitesse ) doivent etre renseignés
          noInterrupts();
          planner.resume();
          interrupts();
          if (planner.getStatus()) {
            startTimer();
            setPeriod(planner.getPeriod() * speedMult);
          }
        }
      } else {
        noInterrupts();
        planner.stop();
        interrupts();
      }
    }

    if (encoder.turn()) {                                                                             // Si vous tournez l'encodeur pendant le fonctionnement automatique,
      actuel.speed = constrain(actuel.speed + encoder.dir() * SPEED_INC, SPEED_INC, VITESSE_LIMITE);  // pour changer la valeur de vitesse
      speedMult = double(w.speed) / double(actuel.speed);
      screen.UpdateSpeed(actuel.speed);
    }

    static uint32_t tmr;
    if (millis() - tmr >= 500) {
      tmr = millis();

      int total_tours = (abs(ChariotStepper.pos)) / STEPPER_Bobine_STEPS_COUNT;

      screen.Updatetours(total_tours % w.tours + 1);
      DebugWrite("pos", ChariotStepper.pos, CoucheStepper.pos);
      screen.PlannerStatus(planner.getStatus());
    }
  }

  CoucheStepper.disable();
  ChariotStepper.disable();
}

void AutoTrancanageAll(const Trancanage Trancanages[], byte n) {
  bool direction = Trancanages[0].dir;

  for (byte i = 0; i < n; ++i) {
    const Trancanage &w = Trancanages[i];
    if (!w.tours || !w.Couches || !w.step || !w.speed) continue;

    screen.Init(w);

    if (n > 1) {
      screen.Draw();
      screen.Message(STRING_3, i + 1); // "Trancanage %d START"
      buzzer.Multibeep(1, 200, 200);   // 1 bip
      WaitBouton();
    }

    AutoTrancanage(w, direction);
  }

  screen.Message(STRING_1);      // "TRANCANAGE AUTOMATIQUE TERMINE"
  buzzer.Multibeep(5, 200, 200); // 5 bip
  WaitBouton();
}


void WaitBouton() {
  do {
    encoder.tick();
    KeyboardRead();
  } while (!encoder.click());
}



void LoadSettings() {
  int p = EEPROM_SETTINGS_ADDR;
  byte v = 0;
  EEPROM_load(p, v);
  if (v != EEPROM_SETTINGS_VERSION)
    return;

  Load(settings, p);
  settings.actuelTransformer = constrain(settings.actuelTransformer, 1, MEMOIRE_COMPTEUR);
}

void SaveSettings() {
  int p = EEPROM_SETTINGS_ADDR;
  byte v = EEPROM_SETTINGS_VERSION;
  EEPROM_save(p, v);
  Save(settings, p);
}

void LoadTrancanages() {
  int p = EEPROM_TrancanageS_ADDR + (settings.actuelTransformer - 1) * EEPROM_TrancanageS_CLASTER;

  byte v = 0;
  EEPROM_load(p, v);

  for (int j = 0; j < Trancanage_COUNT; ++j)
    if (v == EEPROM_TrancanageS_VERSION)
      Load(params[j], p);
    else
      params[j] = Trancanage();
}

void SaveTrancanages() {
  int p = EEPROM_TrancanageS_ADDR + (settings.actuelTransformer - 1) * EEPROM_TrancanageS_CLASTER;

  byte v = EEPROM_TrancanageS_VERSION;
  EEPROM_save(p, v);
  for (int j = 0; j < Trancanage_COUNT; ++j)
    Save(params[j], p);
}

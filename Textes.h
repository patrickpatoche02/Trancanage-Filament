#pragma once
// Textes afficheur Francais et Anglais , Anglais non testé et non essayer en programme

#define FR 1
#define EN 2

const char PROGMEM LINE1_FORMAT[] = "T%03d/%03d %03d rpm";   // affichage compteur tour // nb tour programmé // vitesse
const char PROGMEM LINE2_FORMAT[] = "L%02d/%02d %d.%03d mm"; // affichage compteur couche // nb couche programmé // pas par spire
const char PROGMEM LINE4_FORMAT[] = "%03d";
const char PROGMEM LINE5_FORMAT[] = "%02d";
const char PROGMEM LINE6_FORMAT[] = "%03d";

const char PROGMEM MENU_FORMAT_02[] = " %d";
const char PROGMEM MENU_FORMAT_06[] = " % 4dT";
const char PROGMEM MENU_FORMAT_10[] = "%03d";
const char PROGMEM MENU_FORMAT_13[] = " %02d";
const char PROGMEM MENU_FORMAT_11[] = " %d.%03d";
const char PROGMEM MENU_FORMAT_17[] = " %+04d";


#if LANGUAGE == EN
const char STRING_ON[] = "ON ";
const char STRING_OFF[] = "OFF";

const char PROGMEM STRING_1[] = "AUTOWINDING DONE";
const char PROGMEM STRING_2[] = "PRESS CONTINUE";
const char PROGMEM STRING_3[] = "WINDING %d START";

const char PROGMEM MENU_01[] = "Auto Wrap";
const char PROGMEM MENU_02[] = "Setup";
const char PROGMEM MENU_04[] = "Stand";
const char PROGMEM MENU_05[] = "Settings";
const char PROGMEM MENU_06[] = "Mem 1";
const char PROGMEM MENU_07[] = "Mem 2";
const char PROGMEM MENU_08[] = "Mem 3";
const char PROGMEM MENU_09[] = "Back";  
const char PROGMEM MENU_10[] = "Turns:";
const char PROGMEM MENU_11[] = "Step:";
const char PROGMEM MENU_12[] = "Speed:";
const char PROGMEM MENU_13[] = "Layer:";
const char PROGMEM MENU_14[] = "Direction";
const char PROGMEM MENU_15[] = "Start";
const char PROGMEM MENU_17[] = "SH pos:";
const char PROGMEM MENU_18[] = "StpMul:";
const char PROGMEM MENU_19[] = "LA pos:";
const char PROGMEM MENU_22[] = "LayerStop";
const char PROGMEM MENU_23[] = "Accel";

const char PROGMEM PLANNER_STATUS_0[] = "Pause";
const char PROGMEM PLANNER_STATUS_1[] = "Run";
const char PROGMEM PLANNER_STATUS_2[] = "Wait";
const char PROGMEM PLANNER_STATUS_3[] = "";
const char PROGMEM PLANNER_STATUS_4[] = "Brake";
#else
const char STRING_ON[] =  "   ON ";
const char STRING_OFF[] = "   OFF";

const char PROGMEM STRING_1[] = "FIN Trancanage AUTO";
const char PROGMEM STRING_2[] = "APPUYEZ CONTINUER";
const char PROGMEM STRING_3[] = "Trancanage %d START";

const char PROGMEM MENU_01[] = "Trancanage";
const char PROGMEM MENU_02[] = "Memoire :";
const char PROGMEM MENU_04[] = "Positions de Depart";
const char PROGMEM MENU_05[] = "Parametres";
const char PROGMEM MENU_06[] = "Memoire 1";
const char PROGMEM MENU_07[] = "Memoire 2";
const char PROGMEM MENU_08[] = "Memoire 3";
const char PROGMEM MENU_09[] = "Retour";     // menu precedent
const char PROGMEM MENU_10[] = "Tours    :"; // nb de tour
const char PROGMEM MENU_11[] = "Pas/Spire:"; // espacement spire du trancanage
const char PROGMEM MENU_12[] = "Rpm      :"; // vitesse de bobine receptrice
const char PROGMEM MENU_13[] = "Couches  :"; // nombre de couche de spire
const char PROGMEM MENU_14[] = "Direction";  // direction du sens du trancanage
const char PROGMEM MENU_15[] = "Start";
const char PROGMEM MENU_17[] = "Bobine  :";  // ajustage de la position de la bobine
const char PROGMEM MENU_18[] = "Multiple:";
const char PROGMEM MENU_19[] = "Chariot :";  // ajustage de la position du trancanage
const char PROGMEM MENU_22[] = "Couche Stop";  // changement de couche stop
const char PROGMEM MENU_23[] = "Rampe  ";      // acceleration debut bobinage et fin

const char PROGMEM PLANNER_STATUS_0[] = "Pause";
const char PROGMEM PLANNER_STATUS_1[] = "Trancanage en cours";
const char PROGMEM PLANNER_STATUS_2[] = "Attendre";
const char PROGMEM PLANNER_STATUS_3[] = "";
const char PROGMEM PLANNER_STATUS_4[] = "Freinage";






#endif

const char* const plannerStatuses[] = { PLANNER_STATUS_0, PLANNER_STATUS_1, PLANNER_STATUS_2, PLANNER_STATUS_3, PLANNER_STATUS_4 };

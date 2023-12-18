#pragma once

// a modifier en fonction du matériel utilisé

#define Capteur_Filament      6   // capteur de filament sinal en pin 6 (5VDC/GND/Signal)

#define ENCODER_CLK 2  //encodeur rotatif
#define ENCODER_DT 5   //encodeur rotatif
#define ENCODER_SW 3   //encodeur rotatif

#define STEPPER_STEP_Bobine      4      // pin commande nb step nema bobine
#define STEPPER_DIR_Bobine       7      // pin commande direction nema bobine
#define STEPPER_STEP_Chariot      12    // pin commande nb step nema chariot
#define STEPPER_DIR_Chariot       13    // pin commande direction nema chariot
#define STEPPER_EN          8           // pin commande enable nema bobine et chariot

#define BUZZER              10     // pin buzzer pas obligatoire
#define Bouton_STOP      11        // pin pour inter ou bp start ou pause

#define DISPLAY_RS          A0    // ne sert pas remplacé par LCD I2C
#define DISPLAY_EN          A1    // ne sert pas remplacé par LCD I2C
#define DISPLAY_D4          A2    // ne sert pas remplacé par LCD I2C
#define DISPLAY_D5          A3    // ne sert pas remplacé par LCD I2C
#define DISPLAY_D6          A4    // ne sert pas remplacé par LCD I2C
#define DISPLAY_D7          A5    // ne sert pas remplacé par LCD I2C

#define KEYBOARD_PIN        0     // Pour l'installation du clavier analogique sur A0, vous ne pouvez pas mettre l afficheur,affecter la broche suivante, par exemple sur 6

#define PasFiletage        8000   // pas de filetage de l'arbre chariot trancanage en µm original 8000 pour 8 mm par tour

#define DISPLAY_NCOL 20           // nb de caractere d'affichage en largeur
#define DISPLAY_NROW 4            // nb de caractere d'affichage en hauteur
#define DISPLAY_I2C 1             // utilisation de la connexion d'affichage I2C (1 - oui, 0 - non)
#define DISPLAY_ADDRESS 0x27      // si LCD I2C (adresse 0x27 pour PCF8574T, 0x3F pour PCF8574AT) SDA en A4 et SLC en A5

#define STEPPER_Bobine_STEPS 200      // moteur rotation bobine receptrice// nombre de pas du moteur par tour (200 = 1,8° par pas)
#define STEPPER_Bobine_MICROSTEPS 16  // cavalier sur carte CNC /mmoteur rotation bobine receptrice / /diviseur sur la carte de commande du moteur ,Nombre de micropas par étape
                                      // si nema  1,8 degrés ==> 360/1.8 = 200 avec 16 micropas ==> 200*16=3200 micropas

#define STEPPER_Bobine_REVERSE 0       // moteur rotation bobine receptrice // inverser le sens de rotation du moteur  (1 - oui, 0 - non)
#define STEPPER_Chariot_STEPS 200      // moteur trancanage // nombre de pas du moteur par tour (200 = 1,8° par pas)
#define STEPPER_Chariot_MICROSTEPS 16  // cavalier sur carte CNC /moteur trancanage // diviseur sur la carte pilote du moteur ,Nombre de micropas par étape
                                       // si nema  1,8 degrés ==> 360/1.8 = 200 avec 16 micropas ==> 200*16=3200 micropas

#define STEPPER_Chariot_REVERSE 1      // moteur trancanage // inverser le sens de rotation du moteur (1 - oui, 0 - non)

#define ENCODER_TYPE EB_FULLSTEP  // Type d'encodeur : EB_FULLSTEP ou EB_HALFSTEP. si l'encodeur fait un pas en deux clics, vous devez modifier le paramètre
#define ENCODER_INPUT INPUT       // s'il y a des résistances de rappel sur l'encodeur  - définissez INPUT, sinon - INPUT_PULLUP

#define KEYBOARD_LEFT 0          // 0   resistance de 0 ohm /ne sert pas remplacé par l'encodeur
#define KEYBOARD_UP 33           // k33 resistance de 330 ohm/ ne sert pas remplacé par l'encodeur
#define KEYBOARD_DOWN 93         // 1k0 resistance de 1 Kohm /ne sert pas remplacé par l'encodeur
#define KEYBOARD_RIGHT 171       // 2k0 resistance de 2 Kohm /ne sert pas remplacé par l'encodeur
#define KEYBOARD_SELECT 350      // 5k2 resistance de 5,2 Kohm /ne sert pas remplacé par l'encodeur

#define LANGUAGE FR         // FR pour menu en Francais. EN pour anglais mais texte du menu perfectible(programme pas essayer en Anglais)
//#define DEBUG             // a décommenter pour avoir les infos de position par le port serie
#define MEMOIRE_COMPTEUR 3  // nb de programme en mémoire 36 maxi 

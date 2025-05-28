
/*
to do list:
-MVC !!!!!!!  
-fonction     
-sleep
-comments
-local librairy
-bluetooth
-courant measurement
-contraint management system   
-safety cap   
-it talks to me 
-move the degree and voltage measurements up into each respective mode  
-etc

NEW STOUGH TO DO: 
-BLE compatible
-Safety current
-comments
-EMO add
//******************ENTETE******************
  Nom: James Gordon-Ball
  Classe: 6A  
  Date: 2024-2025
  Ecole: IN.RA.CI
  Promoteur: Mr Kapita

//******************HARDWARE******************
  Ce code me permet t'uliser mon : 
     -ESP32   (Microcontroleur)
     -MCP3008 (ADC/CAN)
     -PCA9685 (Driver-Servo)
     -MG996r  (Servomoteur)
     -ACS712  (Capteur de Courant)
     -Bluetooth
//******************AVERTISSEMENT****************** 
Ce code a ete inspirer de code externe et d'intelligence artificiel telle que : 
- Code CANSAT (Mr. Kapita)
- ChatGPT
- Code exemple 
- Ancien code
- etc
*/

//******************LIBRARY******************//
#include "TFE_JGB_2025.h"  //Local


void setup() {

  Serial.begin(115200);  // Activation du Seriel moniteur
  while (!Serial) {}

  Verif_driver();
  ADC_Begin(CS_A);
  Serial.println("ADC_A Approved");
  ADC_Begin(CS_B);
  Serial.println("ADC_B Approved");
  Serial.println("Pls select a mode (1 = Commande / 2 = Hive / 3 = Individuel)");
}

void loop() {
  Mode_Choice();

  if (mode == 1) {
    Serial.println("Please select a command");
    Command_list();
    while (mode == 1) {
      check_Serial_Command();
    }
  }
  if (mode == 2) {
    Serial.println("Please select a position between 0° and 180° and keep a safe distance");
    while (mode == 2) {
      Hivemind_Command();
      Measurement_Protocol();
    }
  }
  if (mode == 3) {
    Serial.println("Please select a position between 0° and 180° and keep a safe distance");
    Reset();
    while (mode == 3) {
      Individuel_Servo_Command();
      Serial.println("Restarting...");
      ESP.restart();
    }
  }
  if (mode == 4) {
    Serial.println("Please select a position between 0° and 180° and keep a safe distance");
    while (mode == 4) {
      Serial.println("its alive!!!!!!!!");
    }
  }
}

// pq le poulet?
/*

to do list:
-MVC !!!!!!!  
-fonction     //
-sleep//
-comments
-local librairy//
-bluetooth
-burn the school down
-courant measurement
-contraint management system   //
-safety cap   
-it talks to me 
-move the degree and voltage measurements up into each respective mode  //
-etc

   James Gordon-Ball
     6A  2024-2025
     Ce code me permet t'uliser mon : 
     -ESP32   (Microcontroleur)
     -MCP3008 (ADC/CAN)
     -PCA9685 (Driver-Servo)
     -MG996r  (Servomoteur)
*/
/*
exemple:

#define kk
#ifdef kk
 Serial.print("fjqiuhiuqsh");
#endif
*/

//******************LIBRARY******************//

#include "TFE_JGB_2025.h"  //Local
#include "Dabble_GamePad.h"



DabbleGamepad gamepad;

//******************VARIABLE******************//
char type;
int chan_M3 = 0;

void setup() {
  gamepad.begin(9600);
  Serial.begin(115200);  // Activation du Seriel moniteur
  while (!Serial) {}

  Serial.println("MCP3008 simple test.");

  int a = init_PCA9685();  // Fonction d'activation du PCA9685 (Driver-servo)
  if (a == 1) {
    Serial.println("PCA9685 trouvé");
  } else {
    if (a == 0) {
      Serial.println("PCA9685 n'est pas trouvé");
    }
  }
  ADC_Begin(CS_A);
  ADC_Begin(CS_B);
  Serial.println("chose Mode type (1 = Commande / 2 = Hive / 3 = Individuel)");

  while (mode == 0) {
    if (Serial.available()) {
      char type = Serial.read();
      if (type == '1') {
        mode = 1;
        Serial.println("Command mode chosen");
      }
      if (type == '2') {
        mode = 2;
        Serial.println("Hive mind mode chosen");
      }
      if (type == '3') {
        mode = 3;
        Serial.println("Individuel Position mode chosen");
      }
    }
  }
}

void loop() {


  if (mode == 1) {
    Serial.println("Please select a command");
    Command_list();
    while (mode == 1) {
      check_Serial_Command();
    }
  }
  if (mode == 2) {
    Serial.println("Please select a position between 400 and 2200 and keep a safe distance!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    while (mode == 2) {
      if (Serial.available()) {
        String input = Serial.readStringUntil('\n');  // Lire jusqu'à la fin de ligne (Entrée) / tourne jusqu'a le donnee /n est entree
        int newPos = input.toInt();                   // Convertir la chaîne en entier / fait une lecture du Serial 0 et l'integre dans le newPos
        int pos = (newPos * 10) + 400;                // Mettre à jour la variable de la position de commande
        Reset();


        Set_servo(0, pos);

        Set_servo(1, pos);

        Set_servo(2, pos);

        Set_servo(3, pos);

        Set_servo(4, pos);

        Set_servo(5, pos);

        Serial.print("Position définie sur : ");
        Serial.println(pos);
      }
      Measurement_Protocol();
    }
  }
  if (mode == 3) {
    Serial.println("Please select a position between 400 and 2200 and keep a safe distance!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    Reset();

    while (mode == 3) {
      while (chan_M3 < 6) {
        if (Serial.available()) {
          String input = Serial.readStringUntil('\n');
          int newPos = input.toInt();
          int pos = (newPos * 10) + 400;  // Mettre à jour la variable de la position de commande
          Set_servo(chan_M3, pos);
          Serial.print("channel : ");
          Serial.print(chan_M3);
          Serial.print("\t");
          Serial.println(pos);

          chan_M3++;
        }
      Measurement_Protocol();
      }
      Serial.println("Restarting...");
      chan_M3 = 0;
    }
  }
}

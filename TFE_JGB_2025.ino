// pq le poulet?
/*

to do list:
-fonction
-comments
-local librairy
-bluetooth
-burn the school down
-courant measurement
-contraint management system
-safety cap
-move the degree and voltage measurements up into each respective mode
-etc

   James Gordon-Ball
     6A  2024-2025
     Ce code me permet t'uliser mon : 
     -ESP32   (Microcontroleur)
     -MCP3008 (ADC/CAN)
     -PCA9685 (Driver-Servo)
     -MG996r  (Servomoteur)
*/

//******************LIBRARY******************//
#include "FaBoPWM_PCA9685.h"  //Local
#include "TFE_JGB_2025.h"
#include <Wire.h>
#include <Adafruit_MCP3008.h>  //dois le chercher dans l'environement


FaBoPWM faboPWM;
Adafruit_MCP3008 adc;

//******************VARIABLE******************//
int pos = 0;
byte mode = 0;


void setup() {
  Serial.begin(115200);  // Activation du Seriel moniteur
  while (!Serial) {}

  Serial.println("MCP3008 simple test.");

  init_PCA9685();  // Fonction d'activation du PCA9685 (Driver-servo)

  adc.begin(18, 23, 19, 15);  // Declaration des pins MCP3008 (sck, mosi, miso, cs);

  Serial.println("chose Mode type (1 = commande / 2 = position)");
  while (mode == 0) {
    if (Serial.available()) {
      char type = Serial.read();
      if (type == '1') {
        mode = 1;
        Serial.println("Commande mode chosen");
      }
      if (type == '2') {
        mode = 2;
        Serial.println("Position mode chosen");
      }
    }
  }
}

void loop() {
  if (mode == 1) {
    Serial.println("Please select a command");

    while (mode == 1) {
      if (Serial.available()) {
        char data = Serial.read();
        switch (data) {
          case 'c':
            Reset();
            C_command();
            break;
          case 'i':
            I_command();
            break;
          case 'z':
            Reset();
            Z_command();
            break;
        }
      }
    }
  }
  if (mode == 2) {
    Serial.println("Please select a position between 400 and 2200");
    while (mode == 2) {
      if (Serial.available()) {
        String input = Serial.readStringUntil('\n');  // Lire jusqu'à la fin de ligne (Entrée) / tourne jusqu'a le donnee /n est entree
        int newPos = input.toInt();                   // Convertir la chaîne en entier / fait une lecture du Serial 0 et l'integre dans le newPos
        Reset();

        pos = newPos;                       // Mettre à jour la variable de la position de commande
        faboPWM.set_channel_value(0, pos);  // Pin 15 du PCA9685(Servo-driver) == pos(valeur entrer)
        delay(700);
        faboPWM.set_channel_value(1, pos);  // Pin 15 du PCA9685(Servo-driver) == pos(valeur entrer)
        delay(700);
        faboPWM.set_channel_value(2, pos);  // Pin 15 du PCA9685(Servo-driver) == pos(valeur entrer)
        delay(700);
        faboPWM.set_channel_value(3, pos);  // Pin 15 du PCA9685(Servo-driver) == pos(valeur entrer)
        delay(700);
        faboPWM.set_channel_value(4, pos);  // Pin 15 du PCA9685(Servo-driver) == pos(valeur entrer)
        delay(700);
        faboPWM.set_channel_value(5, pos);  // Pin 15 du PCA9685(Servo-driver) == pos(valeur entrer)
        delay(700);

        Serial.print("Position définie sur : ");
        Serial.println(pos);
      }
    }
  }

  for (int chan = 0; chan < 6; chan++)  // faire une lecture de chaque channel sur le MCP3008 (ADC/CAN)
  {
    int adcValue = adc.readADC(chan);  // lecture de l'ADC
    Serial.print("Channel ");
    Serial.print(chan);
    Serial.print(":");
    Serial.print("\t");  // fait une tabulation (table/espacement)
    Serial.print(" ADC = ");
    Serial.print(adcValue);
    Serial.print("\t");                               // fait une tabulation (table/espacement)
    float voltage = ((adcValue * 2.5) * 2) / 1023.0;  // Convertion en voltage
    Serial.print(" Voltage = ");
    Serial.print(voltage);
    Serial.print("V");
    Serial.print("\t");                     // fait une tabulation (table/espacement)
    int angle = ((voltage * 180.0) / 2.5);  // Convertion en °
    Serial.print("Angle = ");
    Serial.print(angle);
    Serial.println("°");
  }

  Serial.println();
  delay(500);
}


void init_PCA9685(void)  // Activation du PCA9685 (Servo-driver)
{
  if (faboPWM.begin())  // Vérification
  {
    Serial.println("PCA9685 trouvé");
    faboPWM.init(300);  // Initialisation des parametres internes
  } else {
    Serial.println("PCA9685 n'est pas trouvé");
  }
  faboPWM.set_hz(200);  // Mettre la frequence a 50Hz (20ms)
}

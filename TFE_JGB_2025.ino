// pq le poulet?
/*

to do list:
-MVC !!!!!!!
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
char type;
int pos_0 = 0;
int pos_1 = 0;
int pos_2 = 0;
int pos_3 = 0;
int pos_4 = 0;
int pos_5 = 0;
int chan_M3 = 0;

unsigned int def_value;

void setup() {
  Serial.begin(115200);  // Activation du Seriel moniteur
  while (!Serial) {}

  Serial.println("MCP3008 simple test.");

  init_PCA9685();  // Fonction d'activation du PCA9685 (Driver-servo)

  adc.begin(18, 23, 19, 15);  // Declaration des pins MCP3008 (sck, mosi, miso, cs);
  adc.begin(18, 23, 19, 5);
  
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
      for (int chan = 0; chan < 6; chan++)  // faire une lecture de chaque channel sur le MCP3008 (ADC/CAN)
      {
        adc.begin(5);
        int adcValue2 = adc.readADC(chan);
        adc.begin(15);
        int adcValue1 = adc.readADC(chan);                 // lecture de l'ADC
        float voltage = ((adcValue1 * 2.5) * 2) / 1023.0;  // Convertion en voltage
        int angle = ((voltage * 180.0) / 2.5);             // Convertion en °
        Serial.printf("Channel : %d \t ADC_A = %d \t ADC_B = %d \t Voltage = %f V\t Angle = %d °\n", chan, adcValue1, adcValue2, voltage, angle);
      }
      Serial.println("--------------------------------------------------------------------------------------------------------------------------");
      delay(500);
    }
  }
  if (mode == 2) {
    Serial.println("Please select a position between 400 and 2200");
    while (mode == 2) {
      if (Serial.available()) {
        String input = Serial.readStringUntil('\n');  // Lire jusqu'à la fin de ligne (Entrée) / tourne jusqu'a le donnee /n est entree
        int newPos = input.toInt();                   // Convertir la chaîne en entier / fait une lecture du Serial 0 et l'integre dans le newPos
        Reset();

        int pos = newPos;  // Mettre à jour la variable de la position de commande

        Set_servo(0, pos, 500);
        Register(0);
        Serial.println(def_value);

        Set_servo(1, pos, 500);
        Register(1);
        Serial.println(def_value);

        Set_servo(2, pos, 500);
        Register(2);
        Serial.println(def_value);

        Set_servo(3, pos, 500);
        Register(3);
        Serial.println(def_value);

        Set_servo(4, pos, 500);
        Register(4);
        Serial.println(def_value);

        Set_servo(5, pos, 500);
        Register(5);
        Serial.println(def_value);

        Serial.print("Position définie sur : ");
        Serial.println(pos);
      }
      for (int chan = 0; chan < 6; chan++)  // faire une lecture de chaque channel sur le MCP3008 (ADC/CAN)
      {
        adc.begin(5);
        int adcValue2 = adc.readADC(chan);
        adc.begin(15);
        int adcValue1 = adc.readADC(chan);                 // lecture de l'ADC
        float voltage = ((adcValue1 * 2.5) * 2) / 1023.0;  // Convertion en voltage
        int angle = ((voltage * 180.0) / 2.5);             // Convertion en °
        Serial.printf("Channel : %d \t ADC_A = %d \t ADC_B = %d \t Voltage = %f V\t Angle = %d °\n", chan, adcValue1, adcValue2, voltage, angle);
      }
      Serial.println("---------------------------------------------------------------------");
      delay(500);
    }
  }
  if (mode == 3) {
    Serial.println("Please select a position between 400 and 2200");
    Reset();

    while (mode == 3) {
      while (chan_M3 < 6) {
        if (Serial.available()) {
          String input = Serial.readStringUntil('\n');
          int newPos = input.toInt();

          Set_servo(chan_M3, newPos, 500);
          Serial.print("channel : ");
          Serial.print(chan_M3);
          Serial.print("\t");
          Serial.println(newPos);

          chan_M3++;
        }
      }
      Serial.println("Restarting...");
      chan_M3 = 0;
    }
  }
}





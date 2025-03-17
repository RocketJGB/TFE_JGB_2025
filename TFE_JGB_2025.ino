// pq le poulet?
/*

to do list:
-MVC !!!!!!!
-fonction
-sleep
-comments
-local librairy
-bluetooth
-burn the school down
-courant measurement
-contraint management system
-safety cap
-it talks to me 
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
/*
exemple:

#define kk
#ifdef kk
 Serial.print("fjqiuhiuqsh");
#endif
*/

//******************LIBRARY******************//

#include "TFE_JGB_2025.h" //Local
#include "Dabble_GamePad.h"
#include <Wire.h>
#include <Adafruit_MCP3008.h>  //dois le chercher dans l'environement
#include <FaBoPWM_PCA9685.h> 

FaBoPWM faboPWM;
Adafruit_MCP3008 adc;
DabbleGamepad gamepad;

//******************VARIABLE******************//
char type;
int pos_0 = 0, pos_1 = 0, pos_2 = 0, pos_3 = 0, pos_4 = 0, pos_5 = 0, chan_M3 = 0, angle = 0, adcValueA = 0, adcValueB = 0;
unsigned int def_value = 0;
float voltage = 0;

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
    Serial.println("Pls select c, i, z, o, l and keep a safe distance!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
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
          case 'o':
            Open();
            break;
          case 'l':
            Close();
            break;
        }
      }
      for (int chan = 0; chan < 6; chan++)  // faire une lecture de chaque channel sur le MCP3008 (ADC/CAN)
      {
        adc.begin(CS_B);
        adcValueB = adc.readADC(chan);
        adc.begin(CS_A);
        adcValueA = adc.readADC(chan);   // lecture de l'ADC
        voltage = Mesure_voltage(chan);  // Convertion en voltage
        angle = Mesure_angle(voltage);   // Convertion en °
        Serial.printf("Channel : %d \t ADC_B = %d \t ADC_A = %d \t Voltage = %f V\t Angle = %d °\n", chan, adcValueB, adcValueA, voltage, angle);
      }
      Serial.println("--------------------------------------------------------------------------------------------------------------------------");
      delay(500);
    }
  }
  if (mode == 2) {
    Serial.println("Please select a position between 400 and 2200 and keep a safe distance!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    while (mode == 2) {
      if (Serial.available()) {
        String input = Serial.readStringUntil('\n');  // Lire jusqu'à la fin de ligne (Entrée) / tourne jusqu'a le donnee /n est entree
        int newPos = input.toInt();                   // Convertir la chaîne en entier / fait une lecture du Serial 0 et l'integre dans le newPos
        Reset();

        int pos = newPos;  // Mettre à jour la variable de la position de commande

        Set_servo(0, pos);

        Set_servo(1, pos);

        Set_servo(2, pos);

        Set_servo(3, pos);

        Set_servo(4, pos);

        Set_servo(5, pos);

        Serial.print("Position définie sur : ");
        Serial.println(pos);
      }
      for (int chan = 0; chan < 6; chan++)  // faire une lecture de chaque channel sur le MCP3008 (ADC/CAN)
      {
        adc.begin(CS_B);
        adcValueB = adc.readADC(chan);
        adc.begin(CS_A);
        adcValueA = adc.readADC(chan);   // lecture de l'ADC
        voltage = Mesure_voltage(chan);  // Convertion en voltage
        angle = Mesure_angle(voltage);   // Convertion en °
        Serial.printf("Channel : %d \t ADC_B = %d \t ADC_A = %d \t Voltage = %f V\t Angle = %d °\n", chan, adcValueB, adcValueA, voltage, angle);
      }
      Serial.println("---------------------------------------------------------------------");
      delay(500);
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

          Set_servo(chan_M3, newPos);
          Serial.print("channel : ");
          Serial.print(chan_M3);
          Serial.print("\t");
          Serial.println(newPos);

          chan_M3++;
        }
        for (int chan = 0; chan < 6; chan++)  // faire une lecture de chaque channel sur le MCP3008 (ADC/CAN)
        {
          adc.begin(CS_B);
          adcValueB = adc.readADC(chan);
          adc.begin(CS_A);
          adcValueA = adc.readADC(chan);   // lecture de l'ADC
          voltage = Mesure_voltage(chan);  // Convertion en voltage
          angle = Mesure_angle(voltage);   // Convertion en °
          Serial.printf("Channel : %d \t ADC_B = %d \t ADC_A = %d \t Voltage = %f V\t Angle = %d °\n", chan, adcValueB, adcValueA, voltage, angle);
        }
        Serial.println("--------------------------------------------------------------------------------------------------------------------------");
        delay(500);
      }
      Serial.println("Restarting...");
      chan_M3 = 0;
    }
  }
}

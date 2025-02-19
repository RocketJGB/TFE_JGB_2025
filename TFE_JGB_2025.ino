// pq le poulet?

/*   James Gordon-Ball
     6A  2024-2025
     Ce code me permet t'uliser mon : 
     -ESP32   (Microcontroleur)
     -MCP3008 (ADC/CAN)
     -PCA9685 (Driver-Servo)
     -MG996r  (Servomoteur)
*/

//******************LIBRARY******************//
#include "FaBoPWM_PCA9685.h" //Local
#include <Wire.h>
#include <Adafruit_MCP3008.h> //dois le chercher dans l'environement

FaBoPWM faboPWM;
Adafruit_MCP3008 adc;

//******************VARIABLE******************//
int pos = 0;
int count = 0;

void setup() {
  Serial.begin(115200);  // Activation du Seriel moniteur
  while (!Serial) {}

  adc.begin(15);  // Activation du MCP3008 (ADC/CAN)

  Serial.println("MCP3008 simple test.");

  init_PCA9685();  // Fonction d'activation du PCA9685 (Driver-servo)

  Serial.println("Entrez une valeur de position pour le servomoteur (0 à 4095) :");

  adc.begin(18, 23, 19, 15);  // Declaration des pins MCP3008 (sck, mosi, miso, cs);
}

void loop() {

  int i;

  if (Serial.available() > 0) {                   //Si la Serial est superieur a 0
    String input = Serial.readStringUntil('\n');  // Lire jusqu'à la fin de ligne (Entrée) / tourne jusqu'a le donnee /n est entree
    int newPos = input.toInt();                   // Convertir la chaîne en entier / fait une lecture du Serial 0 et l'integre dans le newPos

    pos = newPos;  // Mettre à jour la variable de la position de commande

    faboPWM.set_channel_value(5, pos);  // Pin 15 du PCA9685(Servo-driver) == pos(valeur entrer)
    delay(1000);
    faboPWM.set_channel_value(4, pos);  // Pin 15 du PCA9685(Servo-driver) == pos(valeur entrer)
    delay(1000);
    faboPWM.set_channel_value(3, pos);  // Pin 15 du PCA9685(Servo-driver) == pos(valeur entrer)
    delay(1000);
    faboPWM.set_channel_value(2, pos);  // Pin 15 du PCA9685(Servo-driver) == pos(valeur entrer)
    delay(1000);
    faboPWM.set_channel_value(1, pos);  // Pin 15 du PCA9685(Servo-driver) == pos(valeur entrer)
    delay(1000);

    Serial.print("Position définie sur : ");
    Serial.println(pos);
  }

  for (int chan = 0; chan < 8; chan++)  // faire une lecture de chaque channel sur le MCP3008 (ADC/CAN)
  {
    Serial.print(adc.readADC(chan));  // Afficher la lecture
    Serial.print("\t");   // fait une tabulation (table/espacement)
  }

  Serial.print("[");  // faire un stamp
  Serial.print(count);
  Serial.println("]");
  count++;

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
  faboPWM.set_hz(50);  // Mettre la frequence a 50Hz (20ms)
}

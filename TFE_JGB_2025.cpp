#include "FaBoPWM_PCA9685.h"
#include <Adafruit_MCP3008.h>
//#include "TFE_JGB_2025.h"

extern Adafruit_MCP3008 adc;
extern FaBoPWM faboPWM;

int x;
int mode = 0;
int init_PCA9685(void)  // Activation du PCA9685 (Servo-driver)
{
  if (faboPWM.begin())  // Vérification
  {
    faboPWM.init(300);    // Initialisation des parametres internes
    faboPWM.set_hz(200);  // Mettre la frequence a 50Hz (20ms)
    return 1;
  } else {
    Serial.println("PCA9685 n'est pas trouvé");
    return 0;
  }
}
void Open(void){
  faboPWM.set_channel_value(5, 700);
  delay(500);
}
void Close(void){
  faboPWM.set_channel_value(5, 1300);
  delay(500);
}
void C_command(void) {
  faboPWM.set_channel_value(0, 500);
  delay(500);
  faboPWM.set_channel_value(1, 2000);
  delay(500);
  faboPWM.set_channel_value(2, 2000);
  delay(500);
  faboPWM.set_channel_value(3, 500);
  delay(500);
  faboPWM.set_channel_value(4, 500);
  delay(500);
}
void I_command(void) {
  faboPWM.set_channel_value(4, 500);
  delay(500);
  faboPWM.set_channel_value(3, 1300);
  delay(500);
  faboPWM.set_channel_value(2, 1200);
  delay(500);
  faboPWM.set_channel_value(1, 1300);
  delay(500);
  faboPWM.set_channel_value(0, 2100);
  delay(500);
}
void Z_command(void) {
  faboPWM.set_channel_value(0, 500);
  delay(500);
  faboPWM.set_channel_value(1, 2000);
  delay(500);
  faboPWM.set_channel_value(2, 2200);
  delay(500);
  faboPWM.set_channel_value(3, 2200);
  delay(500);
  faboPWM.set_channel_value(4, 500);
  delay(500);
}
void Reset(void) {
  faboPWM.set_channel_value(4, 500);
  delay(500);
  faboPWM.set_channel_value(3, 1300);
  delay(500);
  faboPWM.set_channel_value(2, 1200);
  delay(500);
  faboPWM.set_channel_value(1, 1300);
  delay(500);
  faboPWM.set_channel_value(0, 2100);
  delay(500);
}
void Set_servo(int chan, int value, int time) {
  faboPWM.set_channel_value(chan, value);
  delay(time);
}
unsigned int Register(int channel) {
  adc.begin(15);
  int adcValue = adc.readADC(channel);
  float voltage = ((adcValue * 2.5) * 2) / 1023;
  unsigned int def_value = ((voltage * 180) / 2.5);
  return def_value;
}


/*int Mode1(void) {
  if (type == '1') {
    mode = 1;
    Serial.println("Commande mode chosen");
  }
  return mode;
}
int Mode2(void) {
  if (type == '2') {
    mode = 2;
    Serial.println("Hive mode chosen");
  }
  return mode;
}
int Mode3(void) {
  if (type == '3') {
    mode = 3;
    Serial.println("Individuel Position mode chosen");
  }
  return mode;
}*/
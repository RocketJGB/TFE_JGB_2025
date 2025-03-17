#include "Arduino.h"
#include <FaBoPWM_PCA9685.h>
#include <Adafruit_MCP3008.h>
#include "TFE_JGB_2025.h"

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
void Open(void) {
  Set_servo(5, 700);
}
void Close(void) {
  Set_servo(5, 1300);
}
void C_command(void) {
  Set_servo(4, 500);  //Reset
  Set_servo(3, 1300);
  Set_servo(2, 1200);
  Set_servo(1, 1300);
  Set_servo(0, 2100);

  Set_servo(0, 500);  //Formation set c
  Set_servo(1, 2000);
  Set_servo(2, 2000);
  Set_servo(3, 500);
  Set_servo(4, 500);
}
void I_command(void) {
  Set_servo(4, 500);  //Reset
  Set_servo(3, 1300);
  Set_servo(2, 1200);
  Set_servo(1, 1300);
  Set_servo(0, 2100);
}
void Z_command(void) {

  Set_servo(4, 500);  //Reset
  Set_servo(3, 1300);
  Set_servo(2, 1200);
  Set_servo(1, 1300);
  Set_servo(0, 2100);

  Set_servo(0, 500);  //Formation set Z
  Set_servo(1, 2000);
  Set_servo(2, 2200);
  Set_servo(3, 2200);
  Set_servo(4, 500);
}
void Reset(void) {
  Set_servo(4, 500);  //Reset
  Set_servo(3, 1300);
  Set_servo(2, 1200);
  Set_servo(1, 1300);
  Set_servo(0, 2100);
}
void Set_servo(int chan, int value) {
  faboPWM.set_channel_value(chan, value);
  int pos = Mesure_angle(Mesure_voltage(chan));
  while ((pos <= pos - 200) || (pos >= pos + 200)) {
    pos = Mesure_angle(Mesure_voltage(chan));
    return;
  }
}

void ADC_Begin(int CS) {
  adc.begin(SLK, MOSI, MISO, CS);
}

float Mesure_voltage(int lane) {
  int adcValueA = adc.readADC(lane);
  float voltage = ((adcValueA * 2.5) * 2) / 1023.0;
  return voltage;
}
int Mesure_angle(float volt) {
  int angle = ((volt * 180.0) / 5.0);
  return angle;
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
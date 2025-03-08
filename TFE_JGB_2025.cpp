#include "FaBoPWM_PCA9685.h"
#include <Adafruit_MCP3008.h> 
extern Adafruit_MCP3008 adc;
extern FaBoPWM faboPWM;

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
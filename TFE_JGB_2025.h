#ifndef TFE_JGB_2025_H
#define TFE_JGB_2025_H

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_MCP3008.h>  //dois le chercher dans l'environement
#include <FaBoPWM_PCA9685.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
// UUIDs du service UART BLE (inspirés du service Adafruit BLE UART)
#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_RX   "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_TX   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

#define MOSI 23
#define MISO 19
#define SLK 18
#define CS_A 15
#define CS_B 5
#define CMD_BUFFER_SIZE 15  // Define the maximum size of the buffer

extern Adafruit_MCP3008 adc;
extern FaBoPWM faboPWM;

extern BLECharacteristic *pTxCharacteristic;
extern bool deviceConnected;

extern int angle, adcValueA, adcValueB, mode;
extern float voltage;

void check_Serial_Command(void);

void C_command(void);

void I_command(void);

void Z_command(void);

void Reset(void);

void Set_servo(int chan, int value);

unsigned int Register(int chan);

int init_PCA9685();

void Close(void);

void Open(void);

void Snake(void);

void ADC_Begin(int CS);

float Mesure_voltage(int lane);

int Mesure_angle(int lane);

void Measurement_Protocol(void);

void Command_list(void);

void Mode_Choice(void);

void Verif_driver(void);

void Individuel_Servo_Command();

void Hivemind_Command(void);

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer);
  void onDisconnect(BLEServer *pServer);
};
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic);
};

void setupBLE();

#endif
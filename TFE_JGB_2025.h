
#ifndef TFE_JGB_2025_H
#define TFE_JGB_2025_H
//******************ENTETE******************//
/* 
  Nom: James Gordon-Ball
  Classe: 6A  
  Date: 2024-2025
  Ecole: IN.RA.CI
  Promoteur: Mr Kapita
*/
//******************LIBRARY******************/ /
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_MCP3008.h>  //dois le chercher dans l'environement
#include <FaBoPWM_PCA9685.h>

//******************CONSTANTE*****************//
#define MOSI 23
#define MISO 19
#define SLK 18
#define CS_A 15
#define CS_B 5
#define CMD_BUFFER_SIZE 15  // Define the maximum size of the buffer

//******************DEFINITION*****************//
extern Adafruit_MCP3008 adc;
extern FaBoPWM faboPWM;

//******************VARIABLE*****************//
extern int angle, adcValueA, adcValueB, mode;
extern float voltage;

//******************FONCTION*****************//
void check_Serial_Command(void);

void C_command(void);

void I_command(void);

void Z_command(void);

void Reset(void);

void Set_servo(int chan, int value);

unsigned int Register(int chan);

int init_PCA9685(void);

void Close(void);

void Open(void);

void Snake(void);

void Secret(void);

void ADC_Begin(int CS);

float Mesure_voltage(int lane);

int Mesure_angle(int lane);

void Measurement_Protocol(void);

void Command_list(void);

void Mode_Choice(void);

void Verif_driver(void);

void Individuel_Servo_Command(void);

void Hivemind_Command(void);

#endif
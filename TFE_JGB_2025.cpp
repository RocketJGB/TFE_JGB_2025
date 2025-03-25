
#include "TFE_JGB_2025.h"


Adafruit_MCP3008 adc;  // Définition objet
FaBoPWM faboPWM;

char cmdBuffer[CMD_BUFFER_SIZE];  // Create a character array (buffer) to store the command
char c;

int angle = 0, adcValueA = 0, adcValueB = 0;
float voltage = 0;
int x;
int mode = 0;
int cmdIndex = 0;  // Initialize the index to track the buffer position

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
void Snake(void) {
  Reset();
  Set_servo(2, 900);
  Set_servo(3, 1600);
  Set_servo(0, 500);
  Open();
  Close();
  Open();
  Close();
  Set_servo(2, 1600);
  Set_servo(3, 900);
  Set_servo(0, 2100);
  Open();
  Close();
  Open();
  Close();
  Reset();
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
  int Pos_want = (value - 400) / 10;
  int Pos_act = Mesure_angle(chan);
  faboPWM.set_channel_value(chan, value);
  while (1) {
    Pos_want = (value - 400) / 10;
    Pos_act = Mesure_angle(chan);
    if ((Pos_act <= Pos_want - 10) || (Pos_act >= Pos_want + 10)) {
      break;
    }
  }
}

void ADC_Begin(int CS) {
  adc.begin(SLK, MOSI, MISO, CS);
}

float Mesure_voltage(int lane) {
  int adcValueA = adc.readADC(lane);
  float voltage = (adcValueA * 5) / 1023.0;
  return voltage;
}
int Mesure_angle(int lane) {
  int adcValueA = adc.readADC(lane);
  int angle = (adcValueA - 170) / 2;
  return angle;
}
void check_Serial_Command(void) {

  while (Serial.available()) {
    c = Serial.read();

    if (c == '\n' || c == '\r') {
      cmdBuffer[cmdIndex] = '\0';

      if (strcmp(cmdBuffer, "reset") == 0) {
        Serial.println(F(" Reset Command card selected "));
        ESP.restart();
      } else if (strcmp(cmdBuffer, "FormeZ") == 0) {
        Serial.println(" Position Z selected ");
        Reset();
        Z_command();
      } else if (strcmp(cmdBuffer, "FormeC") == 0) {
        Serial.println(" Position C selected ");
        Reset();
        C_command();
      } else if (strcmp(cmdBuffer, "FormeI") == 0) {
        Serial.println(" Position I selected ");
        Reset();
        I_command();
      } else if (strcmp(cmdBuffer, "Open") == 0) {
        Serial.println(" Claw Opened ");
        Open();
      } else if (strcmp(cmdBuffer, "Close") == 0) {
        Serial.println(" Claw Closed ");
        Close();
      } else if (strcmp(cmdBuffer, "Defense") == 0) {
        Serial.println(" Im a minor, Get back!!!!!!! ");
        Snake();
      } else if (strcmp(cmdBuffer, "Measure") == 0) {
        Serial.println(" Measurements ");
        Measurement_Protocol();
      } else {
        Serial.print(F(" Unrecognised Commande :  "));
        Serial.println(cmdBuffer);
        Command_list();
      }
      cmdIndex = 0;
      memset(cmdBuffer, 0, CMD_BUFFER_SIZE);
    } else if (cmdIndex < CMD_BUFFER_SIZE - 1) {
      cmdBuffer[cmdIndex++] = c;
    }
  }
}

void Measurement_Protocol(void) {
  for (int chan = 0; chan < 6; chan++)  // faire une lecture de chaque channel sur le MCP3008 (ADC/CAN)
  {
    adc.begin(CS_B);
    adcValueB = adc.readADC(chan);
    adc.begin(CS_A);
    adcValueA = adc.readADC(chan);   // lecture de l'ADC
    voltage = Mesure_voltage(chan);  // Convertion en voltage
    angle = Mesure_angle(chan);      // Convertion en °
    Serial.printf("Channel : %d \t ADC_B = %d \t ADC_A = %d \t Voltage = %f V\t Angle = %d °\n", chan, adcValueB, adcValueA, voltage, angle);
  }
  Serial.println("--------------------------------------------------------------------------------------------------------------------------");
  delay(1000);
}
void Command_list(void) {
  Serial.println(F("--------------------------------------------------------"));  //F = Stored in the flash instead of RAM (save space and memory)
  Serial.println(F("Pls select a Command : "));
  Serial.println(F("reset = reset the code"));
  Serial.println(F("FormeZ = Z pose"));
  Serial.println(F("FormeC = C pose"));
  Serial.println(F("FormeI = I pose"));
  Serial.println(F("Open = Opens claw"));
  Serial.println(F("Open = Closes claw"));
  Serial.println(F("────────────────────────────────────────────────────────"));
}

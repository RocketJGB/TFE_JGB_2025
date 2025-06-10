//******************ENTETE******************//
/* 
  Nom: James Gordon-Ball
  Classe: 6A  
  Date: 2024-2025
  Ecole: IN.RA.CI
  Promoteur: Mr Kapita
*/
//******************LIBRARY******************//
#include "TFE_JGB_2025.h"

//******************DEFINITION*****************//
Adafruit_MCP3008 adc;  // Définition objet
FaBoPWM faboPWM;

//******************VARIABLE*****************//
char cmdBuffer[CMD_BUFFER_SIZE];  // Create a character array (buffer) to store the command
int angle = 0, adcValueA = 0, adcValueB = 0;
float voltage = 0;
int mode = 0;
int cmdIndex = 0;  // index to track the buffer position
int newPos = 0;
float pos = 0;
int act_lane = 0;
int i = 0;
//bool Check_ACQ = false;

//******************FONCTION*****************//
int init_PCA9685(void)  // Activation du PCA9685 (Servo-driver)
{
  if (faboPWM.begin()) {  // Address de mon PCA9685 = 0x40
    faboPWM.init(300);    // Initialisation of the internal parameters
    faboPWM.set_hz(200);  // Establishing the frequence at 200Hz(5ms)
    return 1;
  } else {
    Serial.println("⚠️ PCA9685 n'est pas trouvé");
    return 0;
  }
}
void Simple_test_box(void) {
  Reset();

  Set_servo(0, 1200);
  Set_servo(1, 850);
  Set_servo(3, 666);
  Set_servo(2, 1600);
  Set_servo(4, 400);
  Open();
  Set_servo(2, 1950);
  Set_servo(5, 1100);

  Set_servo(3, 1730);
  Set_servo(4, 2000);
  Set_servo(2, 1200);
  Set_servo(1, 1550);
  Set_servo(2, 400);
  Set_servo(1, 1300);
  Open();

  Set_servo(3, 1200);
  Set_servo(2, 1200);
  Set_servo(0, 1200);
  Set_servo(1, 850);
  Set_servo(3, 600);
  Set_servo(2, 1600);
  Set_servo(4, 400);
  Open();
  Set_servo(2, 2000);
  Set_servo(5, 1200);

  Set_servo(3, 1730);
  Set_servo(4, 2000);
  Set_servo(2, 1200);
  Set_servo(1, 1550);
  Set_servo(2, 400);
  Set_servo(1, 1300);
  Open();
}
void Wave(void) {
  Reset();
  Set_servo(4, 1200);
  Open();
  for (i; i < 2; i++) {
    Set_servo(3, 1700);
    Set_servo(3, 700);
  }
  i = 0;
}
void Snake(void) {  //Position sequence (defence)
  Reset();
  Set_servo(0, 2000);
  Set_servo(2, 900);
  Set_servo(3, 1600);
  Open();
  Close();
  Set_servo(0, 400);
  Open();
  Close();
  Set_servo(0, 1200);
  Set_servo(2, 900);
  Set_servo(3, 1600);
  Open();
  Close();
  Set_servo(0, 1300);
  Open();
  Close();
  Set_servo(2, 1600);
  Set_servo(3, 900);
  Open();
  Close();
  Set_servo(0, 1200);
  Open();
  Close();
  Reset();
}
void Secret(void) {  //Position sequence (special)
  Set_servo(0, 1200);
  Open();
  Set_servo(1, 850);
  Set_servo(2, 1000);
  Set_servo(5, 1110);
  for (i = 0; i < 5; i++) {
    Set_servo(3, 850);
    Set_servo(3, 1550);
  }
  Set_servo(4, 2000);
  Open();
}
void Open(void) {  //Position sequence (open claw)
  Set_servo(5, 700);
}
void Close(void) {  //Position sequence (close claw)
  Set_servo(5, 1300);
}
void C_command(void) {  //Position sequence (forme a C)
  Set_servo(2, 400);
  Set_servo(3, 2000);
  Set_servo(4, 400);
  Set_servo(0, 2000);
  Set_servo(1, 1800);
}
void I_command(void) {  //Position sequence (forme a I)
  Reset();
}
void Z_command(void) {  //Position sequence (forme a Z)
  Set_servo(0, 2000);
  Set_servo(1, 1700);
  Set_servo(2, 350);
  Set_servo(3, 400);
  Set_servo(4, 400);
}
void Reset(void) {  //Position sequence (reset)
  Set_servo(4, 400);
  Set_servo(3, 1200);
  Set_servo(2, 1150);
  Set_servo(1, 1200);
  Set_servo(0, 2000);
}

void Set_servo(int chan, int value) {  //Servo command function

  unsigned long millis_delay = millis();
  int Pos_want = (value - 400) / 8.88;  //Retransforms the brut value into a angle value(170 = offset) (2 = (Max_value - Min_value)/180)

  //Serial.println(Pos_want);
  faboPWM.set_channel_value(chan, value);  // Servo command

  while (true) {

    int Pos_act = Mesure_angle(chan);
    Serial.print("chan: ");
    Serial.print("Target: ");
    Serial.print(Pos_want);
    Serial.print(" Actual: ");
    Serial.print(Pos_act);
    Serial.print(" Measure: ");
    Serial.println(Mesure_angle(chan));

    delay(50);
    if ((Pos_act >= Pos_want - 20) && (Pos_act <= Pos_want + 20)) {  // Compares both present and desired positions together to be able to pass to the next task
      Serial.println("🌐 Position corrected");
      break;
    }
    if (millis() >= millis_delay + 5000) {  //millis delay timer with reset failsafe incase of astronomicaly terrible situation
      ESP.restart();
    }
  }
}

void ADC_Begin(int CS) {  // ADC initialiser function
  adc.begin(SLK, MOSI, MISO, CS);
}
float Mesure_voltage_test(int lane) {  // hall effect sensor output voltage calculator
  ADC_Begin(CS_B);
  act_lane = 5 - lane;
  adcValueB = adc.readADC(act_lane);
  float voltage = (adcValueB * 2.5) / 1023.0;  //(2.5 = Vref)(1023.0 = number of divisions)
  return voltage;
}
float Mesure_voltage(int lane) {  // Servomotor output voltage calculator
  ADC_Begin(CS_A);
  act_lane = 5 - lane;
  adcValueA = adc.readADC(act_lane);
  float voltage = (adcValueA * 5) / 1023.0;  //(5 = Vref *2)(1023.0 = number of divisions)
  return voltage;
}
int Mesure_angle(int lane) {  // Servomotor output angle calculator
  ADC_Begin(CS_A);
  act_lane = 5 - lane;
  adcValueA = adc.readADC(act_lane);
  angle = (adcValueA - 170) / 1.944;  // Converts the raw ADC value back into an angle(170 = offset)(2 = (Max_ADC_value - Min_ADC_value) / 180)
  angle = 180 - angle;                // inverts the angle for compatibility with other functions
  return angle;
}
void check_Serial_Command(void) {

  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {  // Reads to the end of the line (Enter) / keeps reading until the /n is entered
      cmdBuffer[cmdIndex] = '\0';  // adds a null caracter at the end

      if (strcmp(cmdBuffer, "reset") == 0) {  // returns "0" when both strings are equal
        Serial.println("🔄 Resetting card ");
        delay(1000);
        ESP.restart();
      } else if (strcmp(cmdBuffer, "formez") == 0) {
        Reset();
        Z_command();
        Serial.println("🦾 Position Z selected ");
      } else if (strcmp(cmdBuffer, "box") == 0) {
        Simple_test_box();
        Serial.println("🦾 Automated  ");
      } else if (strcmp(cmdBuffer, "wave") == 0) {
        Wave();
        Serial.println("🦾 Helloooooo!");
      } else if (strcmp(cmdBuffer, "formec") == 0) {
        Reset();
        C_command();
        Serial.println("🦾 Position C selected ");
      } else if (strcmp(cmdBuffer, "formei") == 0) {
        Reset();
        I_command();
        Serial.println("🦾 Position I selected ");
      } else if (strcmp(cmdBuffer, "open") == 0) {
        Open();
        Serial.println("👐 Claw Opened ");
      } else if (strcmp(cmdBuffer, "close") == 0) {
        Close();
        Serial.println("👏 Claw Closed ");
      } else if (strcmp(cmdBuffer, "defence") == 0) {
        Snake();
        Serial.println("🙅🏻‍♂️ Back up, im underaged!!!!!!! ");
      } else if (strcmp(cmdBuffer, "xxx") == 0) {
        Secret();
        Serial.println("🔞 Feels good ");
      } else if (strcmp(cmdBuffer, "measure") == 0) {
        Measurement_Protocol();
        Serial.println("📏 Measurements ");
      } else {
        Serial.print(F("⚠️ Unrecognised Commande :  "));
        Serial.println(cmdBuffer);
        Command_list();  // Command display
      }
      cmdIndex = 0;                               // resets the array
      memset(cmdBuffer, 0, CMD_BUFFER_SIZE);      //Sets all the bits of the array to 0
    } else if (cmdIndex < CMD_BUFFER_SIZE - 1) {  // Fills the bits in the array one at a time until /n or /r is received
      cmdBuffer[cmdIndex++] = c;
    }
  }
}


void Measurement_Protocol(void) {
  for (int chan = 0; chan < 6; chan++)  // Takes a measurements of each ADC channel
  {
    adc.begin(CS_B);  // Triggers the hall effect sensor voltage input
    adcValueB = adc.readADC(chan);
    float voltage_test = Mesure_voltage_test(chan);
    adc.begin(CS_A);  // Triggers the servomotor voltage input
    adcValueA = adc.readADC(chan);
    voltage = Mesure_voltage(chan);  // Converts to volts
    angle = Mesure_angle(chan);      // Converts to °
    if (angle < 0) {
      angle = 0;
    }
    Serial.printf("Channel : %d \t ADC_B = %d \t ADC_A = %d \t Voltage = %f V\t Voltage = %f V\t Angle = %d °\n", chan, adcValueB, adcValueA, voltage, voltage_test, angle);
  }
  Serial.println("--------------------------------------------------------------------------------------------------------------------------");
  delay(1000);
}
void Command_list(void) {
  Serial.println(F("--------------------------------------------------------"));  //F = Stored in the flash instead of RAM (save space and memory)
  Serial.println(F("Pls select a Command : "));
  Serial.println(F("reset = reset the code"));
  Serial.println(F("🦾 formez = Z pose"));
  Serial.println(F("🦾 formec = C pose"));
  Serial.println(F("🦾 formei = I pose"));
  Serial.println(F("👐 open = Opens claw"));
  Serial.println(F("👏 close = Closes claw"));
  Serial.println(F("📏 measure = Measurement Protocol"));
  Serial.println(F("────────────────────────────────────────────────────────"));
}
void Mode_Choice(void) {
  if (Serial.available()) {
    Serial.println("🌐 Serial available");
    String input = Serial.readStringUntil('\n' || '\r');  // Converts the input into a String until /n (enter) or /r (carriage return) is received
    int Data = input.toInt();                             // ReConverts the string to a long integer
    Serial.println("🌐 Serial received");
    switch (Data) {
      case 1:
        mode = 1;
        Serial.println("🌐 Command mode chosen");
        break;
      case 2:
        mode = 2;
        Serial.println("🌐 Hive mind mode chosen");
        break;
      case 3:
        mode = 3;
        Serial.println("🌐 Individuel Position mode chosen");
        break;
      default:
        Serial.println("⚠️ Invalid input. Please enter 1, 2, 3, or 4.");
        break;
    }
  }
}
void Verif_driver(void) {  //Activation function for the PCA9685(Driver-servo)
  int a = init_PCA9685();
  if (a == 1) {
    Serial.println("✅ PCA9685 trouvé");
  } else {
    if (a == 0) {
      Serial.println("⚠️ PCA9685 n'est pas trouvé");
    }
  }
}
void Hivemind_Command(void) {
  if (Serial.available()) {

    String input = Serial.readStringUntil('\n');  // Converts the input into a String until /n (enter) or /r (carriage return) is received
    input.trim();

    if (input.equalsIgnoreCase("reset")) {  // Checks for reset command
      Serial.println("⚠️ Restarting...");
      ESP.restart();
      return;
    }

    newPos = input.toInt();  // Reconverts the string to a long integer
    if (newPos < 0 || newPos > 180) {
      Serial.println("⚠️ Wrong coordinates, pls select a position between 0° and 180°");
      return;
    }

    //newPos = 180 - newPos;      //if you touch this again I will steal you're grandmother (grandma gone)
    pos = (newPos * 8.88) + 400;  //transforms the angle value into a brut value(400 = offset) (8.88 = (Max_value - Min_value)/180)
    Set_servo(0, 2000);           //0 = Base servo
    Set_servo(1, 1200);
    Set_servo(4, pos);  //4 = neck servo
    Set_servo(3, pos);
    Set_servo(2, pos);

    Serial.print("Position définie sur : ");
    Serial.println(pos);
  }
}


void Individuel_Servo_Command(void) {
  int chan_M3 = 0;  //Tracking variable
  int Pos_S0_min = 30;
  int Pos_S0_max = 160;
  bool Check_ACQ = false;

  while (chan_M3 < 6) {
    // Measurement_Protocol();

    if (chan_M3 == 1 && Check_ACQ == true) {  //if danger present move the second servo from the base to 40° (away from the box)
      faboPWM.set_channel_value(chan_M3, 700);
      chan_M3 = 2;
      Serial.println("⚠️  Danger prevented — Check_ACQ set to true");
    }

    if (Serial.available()) {
      Serial.print(chan_M3);
      String input = Serial.readStringUntil('\n' || '\r');




      newPos = input.toInt();
      Serial.println("⚠️ starting check — Check_ACQ set to true");


      if (chan_M3 == 0) {
        if (newPos >= Pos_S0_min && newPos <= Pos_S0_max) {  //Checks if the arm is in the danger zone of the box
          Check_ACQ = true;                                  // Danger present
          Serial.println("⚠️  Danger detected — Check_ACQ set to true");
        }
      }

      pos = (newPos * 8.88) + 400;  //transforms the angle value into a brut value(170 = offset) (2 = (Max_value - Min_value)/180)
      Set_servo(chan_M3, pos);

      Serial.print("channel : ");
      Serial.print(chan_M3);
      Serial.print("\t");
      Serial.println(pos);

      chan_M3++;
    }
  }
  Serial.println("🔀 To reset the mode please write reset or tap the enter button to continue in individuel mode");
  while (chan_M3 == 6) {
    if (Serial.available()) {
      Serial.print("Channel : ");
      Serial.println(chan_M3);
      String input = Serial.readStringUntil('\n' || '\r');
      Check_reset(input);
      chan_M3++;
      break;
    }
  }
}

void Check_reset(String enter) {
  enter.trim();
  if (enter.equals("reset")) {
    Serial.println("🔄 Restarting");
    delay(1000);
    ESP.restart();
  } else {
    Serial.println("🌐 You have chosen to stay in individuel");
    Serial.println("🔄 Operation restarting");
  }
}

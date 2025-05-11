
#include "TFE_JGB_2025.h"


Adafruit_MCP3008 adc;  // Définition objet
FaBoPWM faboPWM;
BLECharacteristic *pTxCharacteristic;

char cmdBuffer[CMD_BUFFER_SIZE];  // Create a character array (buffer) to store the command
bool deviceConnected = false;
int angle = 0, adcValueA = 0, adcValueB = 0;
float voltage = 0;
int mode = 0;
int cmdIndex = 0;  // Initialize the index to track the buffer position
int newPos = 0;
float pos = 0;
int act_lane = 0;
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
  Set_servo(0, 400);
  Open();
  Close();
  Set_servo(0, 1300);
  Open();
  Close();
  Set_servo(2, 1600);
  Set_servo(3, 900);
  Set_servo(0, 2000);
  Open();
  Close();
  Set_servo(0, 1200);
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
  Reset();
  Set_servo(0, 2000);  //Formation set c
  Set_servo(1, 2000);
  Set_servo(2, 400);
  Set_servo(3, 2000);
  Set_servo(4, 400);
}
void I_command(void) {
  Reset();
}
void Z_command(void) {
  Reset();
  Set_servo(0, 2000);  //Formation set Z
  Set_servo(1, 1700);
  Set_servo(2, 400);
  Set_servo(3, 400);
  Set_servo(4, 400);
}
void Reset(void) {
  Set_servo(4, 400);  //Reset
  Set_servo(3, 1200);
  Set_servo(2, 1200);
  Set_servo(1, 1200);
  Set_servo(0, 2000);
}
void Set_servo(int chan, int value) {
  unsigned long millis_delay = millis();
  int Pos_want = (value - 400) / 8.88;
  Serial.println(Pos_want);

  faboPWM.set_channel_value(chan, value);

  while (true) {

    int Pos_act = 180 - Mesure_angle(chan);  // This as well, dont try to understand it you'll go crazy
    Serial.print("Target: ");
    Serial.print(Pos_want);
    Serial.print(" Actual: ");
    Serial.print(Pos_act);
    Serial.print(" Measure: ");
    Serial.println(Mesure_angle(chan));
    delay(50);
    if ((Pos_act >= Pos_want - 20) && (Pos_act <= Pos_want + 20)) {
      Serial.println("Position corrected");
      break;
    }
    if (millis() >= millis_delay + 5000) {
      Serial.write("Restarting...");
      ESP.restart();
    }
  }
}

void ADC_Begin(int CS) {
  adc.begin(SLK, MOSI, MISO, CS);
}
float Mesure_voltage_test(int lane) {
  ADC_Begin(CS_B);
  act_lane = 5 - lane;
  adcValueB = adc.readADC(act_lane);
  float voltage = (adcValueB * 2.5) / 1023.0;
  return voltage;
}
float Mesure_voltage(int lane) {
  ADC_Begin(CS_A);
  act_lane = 5 - lane;
  adcValueA = adc.readADC(act_lane);
  voltage = (adcValueA * 5) / 1023.0;
  return voltage;
}
int Mesure_angle(int lane) {
  ADC_Begin(CS_A);
  act_lane = 5 - lane;
  adcValueA = adc.readADC(act_lane);
  angle = (adcValueA - 170) / 1.944;
  return angle;
}
void check_Serial_Command(void) {

  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      cmdBuffer[cmdIndex] = '\0';

      if (strcmp(cmdBuffer, "reset") == 0) {
        Serial.println(" Reset Command card selected ");
        ESP.restart();
      } else if (strcmp(cmdBuffer, "formez") == 0) {
        Serial.println(" Position Z selected ");
        Reset();
        Z_command();
      } else if (strcmp(cmdBuffer, "formec") == 0) {
        Serial.println(" Position C selected ");
        Reset();
        C_command();
      } else if (strcmp(cmdBuffer, "formei") == 0) {
        Serial.println(" Position I selected ");
        Reset();
        I_command();
      } else if (strcmp(cmdBuffer, "open") == 0) {
        Serial.println(" Claw Opened ");
        Open();
      } else if (strcmp(cmdBuffer, "close") == 0) {
        Serial.println(" Claw Closed ");
        Close();
      } else if (strcmp(cmdBuffer, "defence") == 0) {
        Serial.println(" Im a minor, Get back!!!!!!! ");
        Snake();
      } else if (strcmp(cmdBuffer, "measure") == 0) {
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
    float voltage_test = Mesure_voltage_test(chan);
    adc.begin(CS_A);
    adcValueA = adc.readADC(chan);   // lecture de l'ADC
    voltage = Mesure_voltage(chan);  // Convertion en voltage
    angle = Mesure_angle(chan);      // Convertion en °
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
  Serial.println(F("formez = Z pose"));
  Serial.println(F("formec = C pose"));
  Serial.println(F("formei = I pose"));
  Serial.println(F("open = Opens claw"));
  Serial.println(F("close = Closes claw"));
  Serial.println(F("measure = Measurement Protocol"));
  Serial.println(F("────────────────────────────────────────────────────────"));
}
void Mode_Choice(void) {
  if (Serial.available()) {
    Serial.println("Serial avail");
    String input = Serial.readStringUntil('\n' || '\r');
    int Data = input.toInt();
    Serial.println("Serial received");
    switch (Data) {
      case 1:
        mode = 1;
        Serial.println("Command mode chosen");
        break;
      case 2:
        mode = 2;
        Serial.println("Hive mind mode chosen");
        break;
      case 3:
        mode = 3;
        Serial.println("Individuel Position mode chosen");
        break;
      case 4:
        mode = 4;
        Serial.println("Bluetooth mode chosen");
        break;
      default:
        Serial.println("Invalid input. Please enter 1, 2, 3, or 4.");
        break;
    }
  }
}
void Verif_driver(void) {
  int a = init_PCA9685();  // Fonction d'activation du PCA9685 (Driver-servo)
  if (a == 1) {
    Serial.println("PCA9685 trouvé");
  } else {
    if (a == 0) {
      Serial.println("PCA9685 n'est pas trouvé");
    }
  }
}
void Hivemind_Command(void) {
  if (Serial.available()) {

    String input = Serial.readStringUntil('\n' || '\r');  // Lire jusqu'à la fin de ligne (Entrée) / tourne jusqu'a le donnee /n est entree
    input.trim();
    if (input.equalsIgnoreCase("reset")) {
      Serial.println("Restarting...");
      ESP.restart();
      return;
    }

    newPos = input.toInt();  // Convertir la chaîne en entier / fait une lecture du Serial 0 et l'integre dans le newPos
    if (newPos < 0 || newPos > 180) {
      Serial.println("Wrong coordinates, pls select a position between 0° and 180°");
      return;
    }

    int midPos = 180 - newPos;      //if you touch this again I will steal you're fking grandmother
    pos = (midPos * 8.88) + 400.0;  // Mettre à jour la variable de la position de commande
    Set_servo(0, 2000);
    Set_servo(1, 1200);
    Set_servo(2, pos);
    Set_servo(3, pos);
    Set_servo(4, pos);


    Serial.print("Position définie sur : ");
    Serial.println(pos);
  }
}
void Individuel_Servo_Command() {
  int chan_M3 = 0;
  while (chan_M3 < 6) {
    if (Serial.available()) {
      Serial.print(chan_M3);
      String input = Serial.readStringUntil('\n' || '\r');
      newPos = input.toInt();
      pos = (newPos * 8.88) + 400.0;  // Mettre à jour la variable de la position de commande
      Set_servo(chan_M3, pos);
      Serial.print("channel : ");
      Serial.print(chan_M3);
      Serial.print("\t");
      Serial.println(pos);
      chan_M3++;
    }
  }
}

void MyServerCallbacks::onConnect(BLEServer *pServer) {
  deviceConnected = true;
  Serial.println("Client BLE connecté !");
}

void MyServerCallbacks::onDisconnect(BLEServer *pServer) {
  deviceConnected = false;
  Serial.println("Client BLE déconnecté.");
  delay(500);
  pServer->getAdvertising()->start();
}

void MyCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  String rxValue = pCharacteristic->getValue().c_str();

  if (rxValue.length() > 0) {
    Serial.print("Reçu via BLE : ");
    Serial.println(rxValue);

    if (deviceConnected) {
      pTxCharacteristic->setValue(rxValue);
      delay(10);
      pTxCharacteristic->notify();
    }
  }
}

void setupBLE() {
  Serial.println("Démarrage du terminal BLE...");

  BLEDevice::init("ESP32-Terminal");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_RX,
    BLECharacteristic::PROPERTY_WRITE
  );
  pRxCharacteristic->setCallbacks(new MyCallbacks());

  pTxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_TX,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pTxCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  pServer->getAdvertising()->start();

  Serial.println("En attente de connexion BLE...");
}
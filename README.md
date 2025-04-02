# Projet en cours de réalisation par : 
- **Nom,Prénom :** James Falconer Gordon Ball
- **Ecole :** In.Ra.Ci
- **Annee :** 2024-2025
- **Classe :** 6 TQ A
- **Promotteur :** Feliz Kapita 

**Sommaire**

**1)[Introduction](#Introduction)**

  -Motivation 
  
  -Comment
  
**2)[Schema Bloc](#Schema-Bloc)**

  -Schema Bloc
  
  -Explication
  
**3)[Caracteristique](#Caracteristique )**
  
  -General
  
  -Electrique 
  
  -Mecanique
  
**4)[Schema de Principe](#Schema-de-Principe)** 

  -Schema de Principe (Sans bornier)
  
  -Schema de Principe (Avec bornier)
  
**5)[PCB](#PCB)**

  -PCB (full)
  
  -PCB (Sous couche)
  
  -PCB (Plan de masse)
  
  -PCB (+5V couche)
  
  -PCB (Sur couche)
  
**6)[Vue 3D](#Vue-3D)**

**7)[Datasheet](#Datasheet)**




1) #Introduction

Pourquoi ce projet ? 

Comme projet de TFE, j'ai décidé de faire un bras robotique équipé de différents capteurs pour indiquer toutes sortes de données que je pourrais ensuite les traiter pour faire des opérations. J'ai toujours été intéressé par des projets qui bougent ou qui ont un aspect mécanique derrière, et c'est l'une  des raisons pour lesquelles ce projet m'intéresse plus qu'un projet analogique ou un projet qui ne bouge pas, car j'aime voir mon progrès à travers son fonctionnement plus simplement et ça combine équitablement l'électronique, la mécanique et le software design ; me permettant une bonne compréhension dans chaque domaine et l'opportunité de progresser là où je suis doué.

Comment ?

Le bras fonctionnerait à l'aide de 6 servomoteurs permettant 5 axes de motions et une main, les servomoteurs seront branchés à des capteurs de courant à l’entrée qui mesurant le courant rentrant dans les servos, tout cela sera ensuite relié au PCA9685 qui pourra non seulement agir comme extension au uC mais aussi pourra faire une partie des tâches pour moins encombrer le uC. Ensuite, une file sera soudée sur la sortie des servos pour le relier au mcp3008 qui nous sert comme ADC pour donner plus d'entrée analogique et moins surcharger le CPU, ces fils nous permettront de lire la tension par rapport à l'angle du servo et ensuite de la traiter et l’utiliser pour enregistrer la position du bras.  

Pour mieux capter la tension du servo, on va utiliser le MCP1525 qui est une référence de tension. Tout cela sera raccordé à l'ESP32 qui est le uC. Le ESP32 pourra ensuite, à l'aide de son Bluetooth intégré, recevoir des commandes du contrôleur ou de la manette. Le tout sera alimenté par un convertisseur de tension, l'IRM30-5 qui fournira une tension de 5V et un courant de 6A à partir de 230V qui est adéquat par rapport à la consommation des servos qui est de 4.8-7.2V et 500m-900mA.

SCHEMA BLOC :

![Screenshot 2025-03-26 185233](https://github.com/user-attachments/assets/f00ba97b-0116-41ec-8de4-51b59e725161)
[Schema Bloc_final.pdf](https://github.com/user-attachments/files/19473042/Schema.Bloc_final.pdf)

Explication du schéma bloc: 

Alimentation :

-Convertisseur AC/DC.

-Régulateur classique (+3V)

-Régulateur de tension de référence (+2.5V)

Microcontrôleur (uC) :

-Bluetooth (BT)

-I2C

-SPI

Commande des servomoteurs :

-Driver moteur (+3V)

-Servomoteurs (+5V)

Mesure :

-Capteurs de courant (+5V)

-ADC (Mesure de courant)

-ADC (Mesure de tension)

CARACTERISTIQUE :

General : 

- Bras Robotique a 5axe de motion et une mains 

- 6 capteurs de courants qui sert comme sécurité contre les surcharges  

- 2 ADC qui permettent de lire les mesures de courant et de position 

- 1 driver moteur qui est utilisé pour la commande des 6 servomoteurs 

 

Electronique : 

- 6x Servomoteurs (TD-8120MG) 
-> 1A / 5V 

- 2x ADC (MCP3008) 
-> 425uA / 5V 

- 1x Driver-Servo (PCA9685) 
-> 25mA/5V 

- 6x Capteur de courant (ACS712) 
-> 10mA/5V 

- 1x Module alim AC/DC 5V 6A (IRM-30-5) 

- 1x Regulateur 3v3 (MCP1826) 

- 1x Microcontroleur (ESP32) 

- Reference de tension (MCP1525-I/TO) 

- Courant total = 6A 

- Tension total = 5V 

 

Mécanique : 

- 4x murs (Plexi) 

-> Avant murs = 85 x 150 x 3 

-> murs Gauche = 85 x 130 x 3 

-> murs Droite = 85 x 130 x 3 

-> Arriére murs = 85 x 150 x 3 

 

- 1x toit (Plexi) 
-> toit = 185 x 165 x 3 

- 1x Base (Bois) 
-> toit = 360 x 250 x 5 

- 4x palonier (Plastique) 
-> palonier = 23 x 85 x 23 

- Bras mecanique (Aluminium) 
-> Bras max hauteur = 505mm 

- 2x petit support base (Plastique) 
-> petit support base = 10 x 58 x 30 

- 2x grand support base (Plastique) 
-> grand support base = 20 x 58 x 30 

- 1x Bridge support base (Plastique) 
-> grand support base = 13 x 26 x 110 

- 1X Slider (Plexi) 
->Slider = 100 x 100 

- PCB  
-> PCB = 115 x 135 x 1.5 

Poids total = 2 kg 



SCHEMA DE PRINCIPE (Sans bornier) :

![image](https://github.com/user-attachments/assets/4f3af2e3-8670-4092-ab57-f39a4c552754)

SCHEMA DE PRINCIPE (Avec bornier) :

![Screenshot 2025-03-26 200046](https://github.com/user-attachments/assets/e91fe654-ef18-4f64-bd08-787b52250eee)




PCB (Full) :

![Screenshot 2025-03-26 203935](https://github.com/user-attachments/assets/6860ed35-6953-4127-a096-1fec5d885130)

PCB (Sous-couche) :

![Screenshot 2025-03-26 204000](https://github.com/user-attachments/assets/207aac90-4518-41e8-8f37-a71d6ad6d868)

PCB (Plans de masse) :

![Screenshot 2025-03-26 204109](https://github.com/user-attachments/assets/1a2c51de-be05-423c-b490-264b4c7643f9)

PCB (+5V) :

![Screenshot 2025-03-26 204048](https://github.com/user-attachments/assets/fbf0e9d6-bbab-4c1a-9586-b929a5da82ac)

PCB (Sup-couche) :

![Screenshot 2025-03-26 204123](https://github.com/user-attachments/assets/99163850-eaa3-4ea5-8877-4683777cb6fc)


VUE 3D : 

![Screenshot 2025-01-04 192933](https://github.com/user-attachments/assets/a66e753b-3b20-4f98-a108-c0b56f3186ae)
![Screenshot 2025-01-04 192942](https://github.com/user-attachments/assets/f771487f-9fc7-4d15-a8ea-62643e305696)
![Screenshot 2025-01-04 193000](https://github.com/user-attachments/assets/f58e5cef-17a1-4942-a162-d55c0bc3fe78)
![Screenshot 2024-12-17 212817](https://github.com/user-attachments/assets/bf55c0e6-f0e0-4810-8965-c6205db52be9)

DATASHEET : 


Servo driver: 
https://cdn-shop.adafruit.com/datasheets/PCA9685.pdf 

ESP32(uC): 
https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf 
Current Sensor:https://www.sparkfun.com/datasheets/BreakoutBoards/0712.pdf 

ServoMoteur: 
https://cnc1.lv/PDF%20FILES/TD-8120MG_Digital_Servo.pdf?srsltid=AfmBOorzCWSFT1FFUrQqBRzjjQ9Lib7SdD6j_WTL15eVxUna8XjdVThH

MCP3008: 
https://ww1.microchip.com/downloads/aemDocuments/documents/MSLD/ProductDocuments/DataSheets/MCP3004-MCP3008-Data-Sheet-DS20001295.pdf 

PCA9685PW,112: 
https://cdn-shop.adafruit.com/datasheets/PCA9685.pdf

IRM-30-5: 
https://www.meanwellusa.com/upload/pdf/IRM-30/IRM-30-spec.pdf

MCP1525-I/TO: 
https://ww1.microchip.com/downloads/en/devicedoc/21653c.pdf

ACS712: 
https://www.allegromicro.com/~/media/Files/Datasheets/ACS712-Datasheet.ashx

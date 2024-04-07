#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// RFID
#define RST_PIN        7          // Configurable, see typical pin layout above
#define SS_PIN         10         // Configurable, see typical pin layout above

// LED
#define RGB_LED_RED         3 // D5
#define RGB_LED_GREEN       5 // D6
#define RGB_LED_BLUE        6 // D7

#define LED_NIGHT     9 // D9

const byte COLOR_BLACK = 0b000;
const byte COLOR_RED = 0b100;
const byte COLOR_GREEN = 0b010;
const byte COLOR_BLUE = 0b001;
const byte COLOR_MAGENTA = 0b101;
const byte COLOR_CYAN = 0b011;
const byte COLOR_YELLOW = 0b110;
const byte COLOR_WHITE = 0b111;

// OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// BUZZER
#define BUZZER_PIN  2  // D2

// LDR
#define LDR_PIN   A0
int ldrValue { 0 };

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int cardUID[4] = {0x00, 0x00, 0x00, 0x00};
bool readLDR = true;

// Functions

void displayColor(byte color) {
  digitalWrite(RGB_LED_RED, !bitRead(color, 2));
  digitalWrite(RGB_LED_GREEN, !bitRead(color, 1));
  digitalWrite(RGB_LED_BLUE, !bitRead(color, 0));
}

void readData(char* buffer, size_t bufferSize) {
  int index = 0;
  // Lecture des données jusqu'à ce qu'un caractère de retour à la ligne ou un espace soit rencontré
  while(Serial.available() > 0 && index < bufferSize - 1) {
    char incomingByte = Serial.read();
    if(incomingByte == '\n' || incomingByte == ' ') {
      break; // Sortir de la boucle si un caractère de fin de ligne ou un espace est rencontré
    }
    buffer[index++] = incomingByte; // Ajouter le caractère au tableau et incrémenter l'index
    delay(2); // Attente pour éviter la surcharge du processeur
  }
  buffer[index] = '\0'; // Ajouter le caractère de fin de chaîne
}

void parseData(char* data, char* id, char* prenom, char* nom, char* acces, char* credit) {
  // Utilisation de strtok pour séparer la chaîne en sous-chaînes
  char* token = strtok(data, "|");

  // Copie de chaque sous-chaîne dans les variables correspondantes
  if (token != NULL) {
    strcpy(id, token);
    token = strtok(NULL, "|");
    if (token != NULL) {
      strcpy(prenom, token);
      token = strtok(NULL, "|");
      if (token != NULL) {
        strcpy(nom, token);
        token = strtok(NULL, "|");
        if (token != NULL) {
          strcpy(acces, token);
          token = strtok(NULL, "|");
          if (token != NULL) {
            strcpy(credit, token);
          }
        }
      }
    }
  }
}

void buzz(int del){
    // BUZZER REFUSE
    digitalWrite(BUZZER_PIN, HIGH);
    delay(del);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
}

// -------------------------------------------------------------------------------------------

// Setup
void setup() {
  // Serial
	Serial.begin(9600);		// Initialize serial communications with the PC

  // ------------------------------------

  // LED
  pinMode(RGB_LED_GREEN, OUTPUT); // Set the LED pin as an output
  pinMode(RGB_LED_RED, OUTPUT); // Set the LED pin as an output
  pinMode(RGB_LED_BLUE, OUTPUT); // Set the LED pin as an output
  displayColor(COLOR_BLACK);

  // ------------------------------------

  // LED NIGHT
  pinMode(LED_NIGHT, OUTPUT); // Set the LDR pin as an input
  // LDR_PIN
  pinMode(LDR_PIN, INPUT); // Set the LDR pin as an input

  // ------------------------------------

  // BUZZER
  pinMode(BUZZER_PIN, OUTPUT); // Set the BUZZER pin as an output

  // RFID
	while (!Serial){
      Serial.print("RFID module not loaded\n");
  }		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);

  // OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed\n"));
    for(;;);
  }

  display.clearDisplay();
  display.display(); // Clear the buffer
}

// ------------------------------------ LOOP ------------------------------------

void loop() {
	delay(50);

  // LDR
  if(Serial.available()){
    String incomingString = Serial.readString();
    incomingString.trim();

    if(incomingString == "/L_OFF"){
      readLDR = false;
    } else if(incomingString == "/L_ON"){
      readLDR = true;
    }
  }

  if(readLDR){
    ldrValue = analogRead(LDR_PIN);
    if(ldrValue < 200){
      digitalWrite(LED_NIGHT, HIGH);
    } else if(ldrValue > 220) {
      digitalWrite(LED_NIGHT, LOW);
    }
  }

  // RFID
    
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}

  // Get the UID
  for(int i{0}; i < 4; i++){
    cardUID[i] = mfrc522.uid.uidByte[i];
  }
  String uid = "";
  for(int i = 0; i < 4; i++) {
      uid += String(cardUID[i], HEX);
  }
  // Write the UID to the serial port
  if(Serial.availableForWrite() >= 7){
    Serial.write(uid.c_str(), uid.length());
  }

  delay(50);

  // Reading and parsing the incoming data from the serial
  char dataFromSerial[32];
  readData(dataFromSerial, sizeof(dataFromSerial));

  char id[5];
  char nom[12];
  char prenom[12];
  char acces[1];
  char credit[2];

  // Créer une copie de dataFromSerial car strtok modifie la chaîne d'origine
  char dataCopy[32];
  strcpy(dataCopy, dataFromSerial);
  parseData(dataCopy, id, prenom, nom, acces, credit);


  // Clear the display buffer.
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,0);     // Start at top-left corner

  if(strlen(dataCopy) == 0){ // No acces to database
    display.setCursor(0,0);
    display.print("Please check database");
    display.display();
    buzz(300);
    //displayColor(COLOR_RED);
    return;
  }

  if (strcmp(prenom, "NULL") == 0) {
    display.print("User not known");
    display.display();
    buzz(50);
    buzz(50);
    buzz(50);
    return;
  }
  

  buzz(50);
  //displayColor(COLOR_GREEN);
  display.print("ID: ");
  display.println(id);
  // Prenom
  display.setCursor(0,10);
  display.print("Prenom: ");
  display.println(prenom);
  // Nom
  display.setCursor(0,20);
  display.print("Nom: ");
  display.println(nom);
  // Credit


  String creditStr = String(credit);
  int creditInt = creditStr.toInt();
  if(creditInt < 0){
    display.setCursor(0,30);
    display.print("OUT OF CREDIT");
    display.display();
    for(int i = 0; i < 10; i++){
      buzz(50);
    }
  }
  else{
    display.setCursor(0,30);
    display.print("Credit: ");
    display.println(credit);
    display.display();
  }

  delay(750);
  // Reset LED
  //displayColor(COLOR_BLACK);
}
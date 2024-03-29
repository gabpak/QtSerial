void setup() {
  Serial.begin(9600); // initialise le port série à 9600 bauds
}

void loop() {
  if(Serial.available() > 0){
    char receivedData[20]; // Déclaration d'un tableau pour stocker les données reçues
    int index = 0; // Index pour parcourir le tableau receivedData
    
    // Lecture des données jusqu'à ce qu'un caractère de retour à la ligne ou un espace soit rencontré
    readData(receivedData, sizeof(receivedData));
    
    Serial.println(receivedData); // Affichage des données reçues
    
    // Comparaison des données reçues avec une autre chaîne de caractères
    if(isStringEqual(receivedData, "ECE")) {
      Serial.println("CONF");
    } else {
      Serial.println("NOTCONF");
    }
  }
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

bool isStringEqual(const char* str1, const char* str2) {
  return strcmp(str1, str2) == 0;
}

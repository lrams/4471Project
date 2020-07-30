
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define ENABLE_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode auth_status;

void setup(){

  Serial.begin(9600); // Serial Frequency
  SPI.begin();  // Initialize Serial Peripheral Interface
  mfrc522.PCD_Init(); // Initialize reader
  delay(4); // Ensure time to setup reader
  pinMode(ENABLE_PIN, OUTPUT);  // Set digital pin behavior
  digitalWrite(ENABLE_PIN,LOW); // Start drive disabled
  Serial.println(F("Ready."));
  
}

void read_data(){
	
  // Set to defaul Key
  for (byte i(0); i < 6; i++){
    key.keyByte[i] = 0xFF;
    
  }
	
  // Block to get key from
  byte block = 1;

  // Authenticate
  auth_status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid));

  // Set pin high if authenticated
  if (auth_status == MFRC522::STATUS_OK){
    Serial.print(F("Good\n"));
    digitalWrite(ENABLE_PIN, HIGH);
  }
  else{
    Serial.print(F("Bad\n"));
  }
  
  // Some debugging output
  Serial.print(mfrc522.GetStatusCodeName(auth_status));
  Serial.print(F("\n"));
  mfrc522.PCD_StopCrypto1();
}

void loop(){
  
  // If new card
  if (! mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  // If card is readable
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  // Read data
  read_data();

}

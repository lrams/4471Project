
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define ENABLE_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode auth_status;

void setup(){

  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN,LOW);
  Serial.println(F("Ready."));
  
}

void read_data(){

  //mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));
  byte buffer[18] = {0};
  /*byte keyarr1[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
  byte keyarr2[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};*/
  for (byte i(0); i < 6; i++){
    key.keyByte[i] = 0xFF;
    
  }

  byte block = 1;
  auth_status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid));

  if (auth_status == MFRC522::STATUS_OK){
    Serial.print(F("Good\n"));
    digitalWrite(ENABLE_PIN, HIGH);
  }
  else{
    Serial.print(F("Bad\n"));
  }
  
  Serial.print(mfrc522.GetStatusCodeName(auth_status));
  Serial.print(F("\n"));
  mfrc522.PCD_StopCrypto1();
}

void loop(){

  if (! mfrc522.PICC_IsNewCardPresent()){
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  read_data();

  //delay(1000);

  //mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));
  
  /*
  digitalWrite(ENABLE_PIN, HIGH);

  delay(5000);

  digitalWrite(ENABLE_PIN, LOW);
  */
}

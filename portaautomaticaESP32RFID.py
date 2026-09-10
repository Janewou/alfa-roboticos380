#include <SPI.h> 

#include <MFRC522.h> 

 
 

#include <BLEDevice.h> 

#include <BLEAdvertising.h> 

 
 
 

// ===================================================== 

// RFID RC522 

// ===================================================== 

 
 

#define SS_PIN 5 

#define RST_PIN 22 

 
 

MFRC522 rfid(SS_PIN, RST_PIN); 

 
 
 

// ===================================================== 

// BUZZER 

// ===================================================== 

 
 

#define BUZZER_PIN 14 

 
 
 

// ===================================================== 

// BLE 

// ===================================================== 

 
 

BLEAdvertising* advertising; 

 
 
 

// ===================================================== 

// ENVIA "ABRIR" PARA O SPIKE 

// ===================================================== 

 
 

void enviarAbrir() { 

 
 

  uint8_t dados[] = { 

    0x97, 0x03,       // LEGO 

    0x01,             // Canal 1 

    0x00,             // Objeto único 

    0xA5,             // String 

    'A', 

    'B', 

    'R', 

    'I', 

    'R' 

  }; 

 
 
 

  String manufacturerData = ""; 

 
 
 

  for (size_t i = 0; i < sizeof(dados); i++) { 

 
 

    manufacturerData += (char)dados[i]; 

 
 

  } 

 
 
 

  BLEAdvertisementData advertisementData; 

 
 
 

  advertisementData.setManufacturerData( 

    manufacturerData 

  ); 

 
 
 

  advertising->stop(); 

 
 
 

  advertising->setAdvertisementData( 

    advertisementData 

  ); 

 
 
 

  advertising->setAdvertisementType( 

    ADV_TYPE_NONCONN_IND 

  ); 

 
 
 

  advertising->setMinInterval(160); 

  advertising->setMaxInterval(160); 

 
 
 

  advertising->start(); 

 
 
 

  Serial.println("BLE -> ABRIR"); 

 
 
 

  // ------------------------------------------------- 

  // Mantém o anúncio ativo por um pequeno período 

  // para dar tempo do SPIKE receber. 

  // ------------------------------------------------- 

 
 

  delay(1000); 

 
 
 

  advertising->stop(); 

 
 
 

  Serial.println("BLE encerrado."); 

} 

 
 
 

// ===================================================== 

// BUZZER 

// ===================================================== 

 
 

void tocarBuzzer() { 

 
 

  digitalWrite(BUZZER_PIN, HIGH); 

  delay(120); 

 
 

  digitalWrite(BUZZER_PIN, LOW); 

  delay(80); 

 
 

  digitalWrite(BUZZER_PIN, HIGH); 

  delay(120); 

 
 

  digitalWrite(BUZZER_PIN, LOW); 

  delay(80); 

 
 

  digitalWrite(BUZZER_PIN, HIGH); 

  delay(300); 

 
 

  digitalWrite(BUZZER_PIN, LOW); 

} 

 
 
 

// ===================================================== 

// SETUP 

// ===================================================== 

 
 

void setup() { 

 
 

  Serial.begin(115200); 

 
 
 

  pinMode(BUZZER_PIN, OUTPUT); 

 
 

  digitalWrite( 

    BUZZER_PIN, 

    LOW 

  ); 

 
 
 

  delay(1000); 

 
 
 

  Serial.println(); 

  Serial.println("=============================="); 

  Serial.println("     PORTA STAR TREK"); 

  Serial.println("=============================="); 

 
 
 

  // ----------------------------------------------- 

  // SPI 

  // ----------------------------------------------- 

 
 

  SPI.begin(); 

 
 
 

  // ----------------------------------------------- 

  // RC522 

  // ----------------------------------------------- 

 
 

  rfid.PCD_Init(); 

 
 

  delay(100); 

 
 
 

  Serial.println("RFID inicializado."); 

 
 
 

  // ----------------------------------------------- 

  // BLE 

  // ----------------------------------------------- 

 
 

  BLEDevice::init( 

    "ESP32_PYBRICKS" 

  ); 

 
 
 

  advertising = 

    BLEDevice::getAdvertising(); 

 
 
 

  Serial.println("BLE inicializado."); 

 
 

  Serial.println(); 

  Serial.println("Sistema pronto."); 

  Serial.println("Aproxime a tag..."); 

  Serial.println(); 

} 

 
 
 

// ===================================================== 

// LOOP 

// ===================================================== 

 
 

void loop() { 

 
 
 

  // ----------------------------------------------- 

  // Verifica se existe uma nova tag 

  // ----------------------------------------------- 

 
 

  if (!rfid.PICC_IsNewCardPresent()) { 

 
 

    return; 

 
 

  } 

 
 
 

  // ----------------------------------------------- 

  // Tenta ler a tag 

  // ----------------------------------------------- 

 
 

  if (!rfid.PICC_ReadCardSerial()) { 

 
 

    return; 

 
 

  } 

 
 
 

  // ================================================= 

  // RFID IDENTIFICADO 

  // ================================================= 

 
 

  Serial.println("=============================="); 

 
 

  Serial.println( 

    "RFID IDENTIFICADO!" 

  ); 

 
 

  Serial.println( 

    "PORTA ABRINDO" 

  ); 

 
 
 

  // ----------------------------------------------- 

  // Mostra UID 

  // ----------------------------------------------- 

 
 

  Serial.print("UID: "); 

 
 
 

  for ( 

    byte i = 0; 

    i < rfid.uid.size; 

    i++ 

  ) { 

 
 

    if ( 

      rfid.uid.uidByte[i] < 0x10 

    ) { 

 
 

      Serial.print("0"); 

 
 

    } 

 
 
 

    Serial.print( 

      rfid.uid.uidByte[i], 

      HEX 

    ); 

 
 
 

    Serial.print(" "); 

 
 

  } 

 
 
 

  Serial.println(); 

 
 
 

  // ================================================= 

  // BUZZER 

  // ================================================= 

 
 

  tocarBuzzer(); 

 
 
 

  // ================================================= 

  // BLE → SPIKE 

  // ================================================= 

 
 

  enviarAbrir(); 

 
 
 

  Serial.println("=============================="); 

  Serial.println(); 

 
 
 

  // ----------------------------------------------- 

  // Finaliza comunicação com a tag 

  // ----------------------------------------------- 

 
 

  rfid.PICC_HaltA(); 

 
 

  rfid.PCD_StopCrypto1(); 

 
 
 

  // ----------------------------------------------- 

  // Pequeno bloqueio para impedir que a mesma tag 

  // seja imediatamente processada novamente. 

  // ----------------------------------------------- 

 
 

  delay(1500); 

} 

 
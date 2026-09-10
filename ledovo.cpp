#include <Wire.h>
#include <VL53L0X.h>
#include <FastLED.h>

#define LED_PIN     6          
#define NUM_LEDS    300        
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
VL53L0X sensor;

// Limites: de 2 metros até 5 centímetros
const int DIST_MAX = 2000; // 2.00 metros
const int DIST_MIN = 50;   // 5 cm

int ledsPintadosAtualmente = 0;
unsigned long ultimoTempoLed = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(50); // Proteção do Step-Down garantida

  // Fita inteira Verde no início
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();

  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println(F("Falha ao iniciar o sensor VL53L0X!"));
    while (1);
  }
  
  sensor.setMeasurementTimingBudget(20000); 
  sensor.startContinuous();
}

void loop() {
  uint16_t distancia = sensor.readRangeContinuousMillimeters();
  
  bool detectouObjeto = (!sensor.timeoutOccurred() && distancia <= DIST_MAX && distancia >= DIST_MIN);

  // === 1. SE DETECTAR OBJETO (VERMELHO QUASE NO LIMITE: DE 3ms a 0ms) ===
  if (detectouObjeto) {
    int ledAlvo = map(distancia, DIST_MAX, DIST_MIN, 0, NUM_LEDS - 1);

    // Ajuste fino do "por favorzinho": Reduzido para 3ms de longe!
    int atrasoDinamicoIda = map(distancia, DIST_MAX, DIST_MIN, 3, 0);
    atrasoDinamicoIda = constrain(atrasoDinamicoIda, 0, 3);

    if (ledAlvo > ledsPintadosAtualmente) {
      if (millis() - ultimoTempoLed >= (unsigned long)atrasoDinamicoIda) {
        leds[ledsPintadosAtualmente] = CRGB::Red; 
        ledsPintadosAtualmente++;                
        FastLED.show();                          
        ultimoTempoLed = millis();               
      }
    }
  } 
  // === 2. SE FICAR DISTANTE (VERDE VARRENDO NO LIMITE DO PROCESSADOR) ===
  else {
    if (ledsPintadosAtualmente > 0) {
      
      // Mantido em 0ms para o verde limpar tudo na velocidade máxima
      if (millis() - ultimoTempoLed >= 0) {
        ledsPintadosAtualmente--; 
        leds[ledsPintadosAtualmente] = CRGB::Green; 
        FastLED.show();                          
        ultimoTempoLed = millis();               
      }
    }
  }
}
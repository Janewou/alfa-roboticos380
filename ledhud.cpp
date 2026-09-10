#include <FastLED.h>

#define NUM_LEDS 300
#define DATA_PIN 6
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

int currentState = 0;
unsigned long previousMillis = 0;
bool animationInitialized = false;
int scanPosition = 0;
bool flashState = false;

void clearLeds() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void setState(int newState) {
  currentState = newState;
  animationInitialized = false;
  scanPosition = 0;
  flashState = false;
  previousMillis = millis();

  // CANCELAMENTO/OFF: apaga fisicamente os LEDs imediatamente.
  if (currentState == 0) {
    clearLeds();
  }
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
  clearLeds();
}

void loop() {
  while (Serial.available() > 0) {
    char cmd = Serial.read();

    // Aceita somente comandos 0-4; \n e \r são ignorados naturalmente.
    if (cmd >= '0' && cmd <= '4') {
      setState(cmd - '0');
    }
  }

  unsigned long now = millis();

  switch (currentState) {
    case 1: runAudio1(now); break;
    case 2: runAudio2(now); break;
    case 3: runAudio3(now); break;
    case 4: runAudio4(now); break;
    default: break;
  }
}

void runAudio1(unsigned long now) {
  if (now - previousMillis >= 350) {
    previousMillis = now;
    flashState = !flashState;
    fill_solid(leds, NUM_LEDS, flashState ? CRGB(255,0,0) : CRGB::Black);
    FastLED.show();
  }
}

// ÁUDIO 2: SCAN AZUL -> VERDE ESTÁVEL
void runAudio2(unsigned long now) {
  if (!animationInitialized) {
    animationInitialized = true;
    scanPosition = 0;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
  }

  if (scanPosition < NUM_LEDS) {
    if (now - previousMillis >= 8) {
      previousMillis = now;
      fadeToBlackBy(leds, NUM_LEDS, 70);

      for (int i = 0; i < 12; i++) {
        int p = scanPosition - i;
        if (p >= 0 && p < NUM_LEDS) {
          uint8_t brightness = 255 - (i * 18);
          leds[p] = CRGB(0, 0, brightness);
        }
      }

      scanPosition++;
      FastLED.show();
    }
  } else {
    // SEM LARANJA: permanece verde até receber outro comando.
    fill_solid(leds, NUM_LEDS, CRGB(125,255,90));
    FastLED.show();
  }
}

// ÁUDIO 3: SCAN AZUL -> VERDE ESTÁVEL
void runAudio3(unsigned long now) {
  if (!animationInitialized) {
    animationInitialized = true;
    scanPosition = 0;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
  }

  if (scanPosition < NUM_LEDS) {
    if (now - previousMillis >= 8) {
      previousMillis = now;
      fadeToBlackBy(leds, NUM_LEDS, 70);

      for (int i = 0; i < 12; i++) {
        int p = scanPosition - i;
        if (p >= 0 && p < NUM_LEDS) {
          uint8_t brightness = 255 - (i * 18);
          leds[p] = CRGB(0, 0, brightness);
        }
      }

      scanPosition++;
      FastLED.show();
    }
  } else {
    fill_solid(leds, NUM_LEDS, CRGB(125,255,90));
    FastLED.show();
  }
}

// ÁUDIO 4: ALERTA LARANJA
void runAudio4(unsigned long now) {
  if (now - previousMillis >= 300) {
    previousMillis = now;
    flashState = !flashState;
    fill_solid(leds, NUM_LEDS,
      flashState ? CRGB(255,75,0) : CRGB::Black);
    FastLED.show();
  }
}


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

#define FX_NONE      0
#define FX_ARMA      1
#define FX_HIPERSONO 2
#define FX_VIDEO     3

#define ARMA_MS       13368UL
#define HIPERSONO_MS  2299UL
#define FX_FRAME_MS   16UL

#define ARMA_T_CHARGE 6700UL
#define ARMA_T_PEAK   7200UL
#define ARMA_T_DECAY  8700UL

int currentEffect = FX_NONE;
unsigned long effectStart = 0;
unsigned long lastFxFrame = 0;

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

  if (currentState == 0 && currentEffect == FX_NONE) {
    clearLeds();
  }
}

void restoreBaseState() {
  clearLeds();
  previousMillis = millis();
  flashState = false;

  switch (currentState) {
    case 2:
    case 3:
      if (scanPosition >= NUM_LEDS) {
        animationInitialized = true;
      } else {
        animationInitialized = false;
        scanPosition = 0;
      }
      break;
    default:
      break;
  }
}

void startEffect(int fx) {
  currentEffect = fx;
  effectStart = millis();
  lastFxFrame = 0;
}

void stopEffect() {
  if (currentEffect == FX_NONE) return;
  currentEffect = FX_NONE;
  restoreBaseState();
}

void runEffect(unsigned long now);
void runArma(unsigned long t);
void runHipersono(unsigned long t);
void runVideoAlert(unsigned long t);
void runAudio1(unsigned long now);
void runAudio2(unsigned long now);
void runAudio3(unsigned long now);
void runAudio4(unsigned long now);

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

    if (cmd >= '0' && cmd <= '4') {
      setState(cmd - '0');
    }
    else if (cmd == 'A' || cmd == 'a') {
      startEffect(FX_ARMA);
    }
    else if (cmd == 'H' || cmd == 'h') {
      startEffect(FX_HIPERSONO);
    }
    else if (cmd == 'V' || cmd == 'v') {
      setState(0);
      startEffect(FX_VIDEO);
    }
    else if (cmd == 'X' || cmd == 'x') {
      stopEffect();
    }
  }

  unsigned long now = millis();

  if (currentEffect != FX_NONE) {
    runEffect(now);
    return;
  }

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

    fill_solid(
      leds,
      NUM_LEDS,
      flashState ? CRGB(255, 0, 0) : CRGB::Black
    );

    FastLED.show();
  }
}

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
    fill_solid(leds, NUM_LEDS, CRGB(125, 255, 90));
    FastLED.show();
  }
}

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
    fill_solid(leds, NUM_LEDS, CRGB(125, 255, 90));
    FastLED.show();
  }
}

void runAudio4(unsigned long now) {
  if (now - previousMillis >= 300) {
    previousMillis = now;
    flashState = !flashState;

    fill_solid(
      leds,
      NUM_LEDS,
      flashState ? CRGB(255, 75, 0) : CRGB::Black
    );

    FastLED.show();
  }
}

void runEffect(unsigned long now) {
  unsigned long t = now - effectStart;

  if (
    (currentEffect == FX_ARMA && t >= ARMA_MS) ||
    (currentEffect == FX_HIPERSONO && t >= HIPERSONO_MS)
  ) {
    stopEffect();
    return;
  }

  if (now - lastFxFrame < FX_FRAME_MS) return;

  lastFxFrame = now;

  if (currentEffect == FX_ARMA) {
    runArma(t);
  }

  if (currentEffect == FX_HIPERSONO) {
    runHipersono(t);
  }

  if (currentEffect == FX_VIDEO) {
    runVideoAlert(t);
  }

  FastLED.show();
}

// ARMA: carga vermelha, disparo magenta e flashes quentes.
void runArma(unsigned long t) {

  if (t < ARMA_T_CHARGE) {

    float p = (float)t / (float)ARMA_T_CHARGE;
    float e = p * (0.58f + 0.42f * p);

    int lit = (int)(e * NUM_LEDS);

    if (lit > NUM_LEDS) lit = NUM_LEDS;

    uint8_t base = 55 + (uint8_t)(p * 100.0f);
    uint8_t waveAmp = 45 + (uint8_t)(p * 95.0f);

    for (int i = 0; i < NUM_LEDS; i++) {

      if (i < lit) {

        uint8_t wave = sin8((uint8_t)(i * 6 - (t >> 1)));
        uint8_t v = qadd8(base, scale8(wave, waveAmp));

        leds[i] = CRGB(255, 0, 0);
        leds[i].nscale8_video(v);

      } else {

        leds[i] = CRGB(12, 0, 0);
      }
    }

    for (int j = 0; j < 8; j++) {

      int pos = lit - 1 - j;

      if (pos >= 0 && pos < NUM_LEDS) {

        uint8_t v = 255 - j * 25;

        leds[pos] = (j < 2)
          ? CRGB(255, 95, 0)
          : CRGB(255, 0, 0);

        leds[pos].nscale8_video(qsub8(v, random8(0, 35)));
      }
    }
  }

  else if (t < ARMA_T_PEAK) {

    uint8_t pulse = beatsin8(24, 150, 255);

    CRGB c(255, 0, 190);
    c.nscale8_video(pulse);

    fill_solid(leds, NUM_LEDS, c);

    for (int i = 0; i < NUM_LEDS; i += 9) {
      if (random8() < 90) {
        leds[i] = CRGB(255, 35, 0);
      }
    }
  }

  else if (t < ARMA_T_DECAY) {

    unsigned long shotT = t - ARMA_T_PEAK;

    uint8_t flash = (shotT / 75UL) % 2 ? 255 : 115;

    fill_solid(leds, NUM_LEDS, CRGB(255, 0, 190));

    for (int i = 0; i < NUM_LEDS; i++) {

      uint8_t pattern = sin8((uint8_t)(i * 5 + shotT / 3));

      if (pattern > 150) {

        uint8_t choice = (uint8_t)((i / 7 + shotT / 90UL) % 3);

        CRGB hot =
          (choice == 0) ? CRGB(255, 0, 0) :
          (choice == 1) ? CRGB(255, 75, 0) :
                          CRGB(255, 210, 0);

        hot.nscale8_video(flash);
        leds[i] = hot;

      } else {

        leds[i].nscale8_video(flash);
      }
    }

    if ((shotT / 110UL) % 2 == 0) {

      int center = (int)((shotT * 2UL) % NUM_LEDS);

      for (int j = -5; j <= 5; j++) {

        int pos = (center + j + NUM_LEDS) % NUM_LEDS;

        leds[pos] = (j % 2 == 0)
          ? CRGB(255, 220, 0)
          : CRGB(255, 55, 0);
      }
    }
  }

  else {

    float r = (float)(t - ARMA_T_DECAY) /
              (float)(ARMA_MS - ARMA_T_DECAY);

    int remaining = (int)((1.0f - r) * NUM_LEDS);

    uint8_t breathe = beatsin8(30, 8, 35);
    uint8_t amp = qadd8(
      breathe,
      (uint8_t)((1.0f - r) * 95.0f)
    );

    fill_solid(leds, NUM_LEDS, CRGB::Black);

    for (int i = 0; i < remaining; i++) {

      uint8_t flicker = random8(150, 256);

      CRGB ember = (random8() < 45)
        ? CRGB(255, 75, 0)
        : CRGB(255, 0, 0);

      ember.nscale8_video(scale8(amp, flicker));
      leds[i] = ember;
    }

    for (int j = 0; j < 5; j++) {

      int pos = remaining - 1 - j;

      if (pos >= 0 && pos < NUM_LEDS) {

        leds[pos] = (j < 2)
          ? CRGB(255, 190, 0)
          : CRGB(255, 45, 0);

        leds[pos].nscale8_video(qadd8(amp, 35 - j * 6));
      }
    }
  }
}

// HIPERSONO: sequência criogênica azul e ciano.
void runHipersono(unsigned long t) {

  const uint8_t deepBlue = 18;

  uint8_t breath = sin8((uint8_t)(t / 10UL));
  uint8_t glow = 35 + scale8(breath, 95);

  if (t < 450UL) {

    uint8_t rise = (uint8_t)(t * 150UL / 450UL);

    for (int i = 0; i < NUM_LEDS; i++) {

      uint8_t wave = sin8((uint8_t)(i * 3 + t / 8UL));

      leds[i] = CRGB(
        0,
        (uint8_t)(rise / 3),
        qadd8(deepBlue, scale8(wave, rise))
      );
    }
  }

  else if (t < 1550UL) {

    uint16_t head =
      (uint16_t)((t - 450UL) * (NUM_LEDS + 48UL) / 1100UL);

    for (int i = 0; i < NUM_LEDS; i++) {

      uint8_t pulse = sin8((uint8_t)(i * 2 + t / 7UL));
      uint8_t base = 24 + scale8(pulse, glow);

      int distance = abs(i - (int)head);

      uint8_t wave = distance < 28
        ? (uint8_t)(255 - distance * 9)
        : 0;

      leds[i] = CRGB(
        0,
        qadd8(base / 2, scale8(wave, 95)),
        qadd8(55, scale8(wave, 190))
      );
    }
  }

  else {

    uint8_t fade =
      (uint8_t)(255UL - ((t - 1550UL) * 205UL / 749UL));

    for (int i = 0; i < NUM_LEDS; i++) {

      uint8_t shimmer = sin8((uint8_t)(i * 5 + t / 13UL));

      CRGB c(
        0,
        18 + scale8(shimmer, 22),
        70 + scale8(shimmer, 45)
      );

      c.nscale8_video(fade);
      leds[i] = c;
    }
  }
}

// VÍDEO: sequência contínua de alertas vermelhos.
void runVideoAlert(unsigned long t) {

  const unsigned long cycle = 7200UL;
  unsigned long p = t % cycle;

  if (p < 1200UL) {

    bool on =
      ((p % 600UL) < 150UL) ||
      ((p % 600UL) >= 240UL && (p % 600UL) < 330UL);

    fill_solid(
      leds,
      NUM_LEDS,
      on ? CRGB(255, 0, 0) : CRGB(8, 0, 0)
    );
  }

  else if (p < 2400UL) {

    uint8_t phase = (uint8_t)((p - 1200UL) / 100UL);

    for (int i = 0; i < NUM_LEDS; i++) {

      bool block = (((i / 15) + phase) % 2) == 0;

      leds[i] = block
        ? CRGB(210, 0, 0)
        : CRGB(12, 0, 0);
    }
  }

  else if (p < 3600UL) {

    int head =
      (int)((p - 2400UL) * (NUM_LEDS + 36UL) / 1200UL);

    fill_solid(leds, NUM_LEDS, CRGB(3, 0, 0));

    for (int j = 0; j < 30; j++) {

      int pos = head - j;

      if (pos >= 0 && pos < NUM_LEDS) {

        uint8_t v = 255 - (uint8_t)(j * 8);
        leds[pos] = CRGB(v, 0, 0);
      }
    }
  }

  else if (p < 4650UL) {

    uint8_t phase = (uint8_t)((p - 3600UL) / 75UL);

    bool on = (phase % 4) != 1;

    fill_solid(
      leds,
      NUM_LEDS,
      on ? CRGB(255, 8, 0) : CRGB::Black
    );

    if (on) {
      for (int i = 0; i < NUM_LEDS; i += 10) {
        leds[i] = CRGB(255, 90, 35);
      }
    }
  }

  else if (p < 5850UL) {

    uint8_t pulse = sin8((uint8_t)((p - 4650UL) / 3UL));

    CRGB c(65 + scale8(pulse, 190), 0, 0);

    fill_solid(leds, NUM_LEDS, c);

    for (int i = 0; i < NUM_LEDS; i += 25) {
      leds[i] = CRGB(255, 35, 12);
    }
  }

  else {

    uint8_t phase = (uint8_t)((p - 5850UL) / 45UL);

    for (int i = 0; i < NUM_LEDS; i++) {

      bool lit = ((i + phase * 7) % 36) < 18;

      leds[i] = lit
        ? CRGB(255, 0, 0)
        : CRGB(2, 0, 0);
    }
  }
}

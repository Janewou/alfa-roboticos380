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

/* ═══════════════════════════════════════════════════════════
   EFEITOS SECUNDÁRIOS (gestos ROCK e PEACE)
   Rodam POR CIMA do estado atual (0-4). Quando terminam, a fita
   volta sozinha ao estado que estava rodando.

   Comandos seriais:
     '0'..'4'  estados principais (como antes)
     'A'       ARMA       -> carga ciano, 13,37 s (mesma duração do áudio)
     'H'       HIPERSONO  -> amarelo piscando, 2,30 s
     'X'       encerra o efeito atual (o navegador envia no fim do áudio
                            ou quando outro efeito o interrompe)
   ═══════════════════════════════════════════════════════════ */
#define FX_NONE      0
#define FX_ARMA      1
#define FX_HIPERSONO 2

#define ARMA_MS       13368UL   // duração de arma.mp3
#define HIPERSONO_MS  2299UL    // duração de hipersono.mp3
#define FX_FRAME_MS   16UL      // ~60 quadros/s

// Marcos da ARMA (medidos no envelope de energia do áudio)
#define ARMA_T_CHARGE 6700UL    // a carga termina, a energia dispara
#define ARMA_T_PEAK   7200UL    // pico do disparo
#define ARMA_T_DECAY  8700UL    // fim do som audível; começa a descarga lenta

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

  // CANCELAMENTO/OFF: apaga fisicamente os LEDs imediatamente.
  // (Se um efeito está rodando, ele continua; o OFF vale quando o efeito acabar.)
  if (currentState == 0 && currentEffect == FX_NONE) {
    clearLeds();
  }
}

// Reconstrói a fita conforme o estado principal depois que um efeito termina.
void restoreBaseState() {
  clearLeds();
  previousMillis = millis();
  flashState = false;

  switch (currentState) {
    case 2:
    case 3:
      // Se o scan já tinha terminado, volta direto ao verde estável;
      // se não, reinicia o scan.
      if (scanPosition >= NUM_LEDS) {
        animationInitialized = true;
      } else {
        animationInitialized = false;
        scanPosition = 0;
      }
      break;
    default:
      break;  // 1 e 4 voltam a piscar sozinhos; 0 fica apagado
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

    // Estados principais 0-4; \n e \r são ignorados naturalmente.
    if (cmd >= '0' && cmd <= '4') {
      setState(cmd - '0');
    }
    // Efeitos secundários
    else if (cmd == 'A' || cmd == 'a') startEffect(FX_ARMA);
    else if (cmd == 'H' || cmd == 'h') startEffect(FX_HIPERSONO);
    else if (cmd == 'X' || cmd == 'x') stopEffect();
  }

  unsigned long now = millis();

  // Efeito ativo tem prioridade sobre o estado principal.
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

/* ═══════════════════════════════════════════════════════════
   EFEITOS
   ═══════════════════════════════════════════════════════════ */

void runEffect(unsigned long now) {
  unsigned long t = now - effectStart;

  // Segurança: encerra sozinho na duração do áudio, mesmo que o 'X' se perca.
  if ((currentEffect == FX_ARMA && t >= ARMA_MS) ||
      (currentEffect == FX_HIPERSONO && t >= HIPERSONO_MS)) {
    stopEffect();
    return;
  }

  if (now - lastFxFrame < FX_FRAME_MS) return;
  lastFxFrame = now;

  if (currentEffect == FX_ARMA)      runArma(t);
  if (currentEffect == FX_HIPERSONO) runHipersono(t);
  FastLED.show();
}

// ARMA: a fita CARREGA em ciano junto com o som, dispara no pico e descarrega.
//   0     – 6,7 s  carga: barra ciano enche com ondas de energia (cresce e acelera)
//   6,7   – 7,2 s  tudo acende e vai a branco-ciano (explosão de energia)
//   7,2   – 8,7 s  decaimento com cintilação (acompanha o fim do som)
//   8,7   – 13,37 s  brasas ciano drenando até apagar no fim do áudio
void runArma(unsigned long t) {
  if (t < ARMA_T_CHARGE) {
    float p = (float)t / (float)ARMA_T_CHARGE;     // 0..1
    float e = p * (0.6f + 0.4f * p);               // enche quase linear, acelera no fim
    int lit = (int)(e * NUM_LEDS);
    if (lit > NUM_LEDS) lit = NUM_LEDS;

    uint8_t base  = 40 + (uint8_t)(p * 90.0f);
    uint8_t swing = 50 + (uint8_t)(p * 110.0f);

    for (int i = 0; i < NUM_LEDS; i++) {
      if (i < lit) {
        // ondas de energia viajando da base para a ponta
        uint8_t w = sin8((uint8_t)((i * 7) - (int)(t >> 1)));
        uint8_t v = qadd8(base, scale8(w, swing));
        leds[i] = CRGB(0, 255, 255);
        leds[i].nscale8_video(v);
      } else {
        leds[i] = CRGB(0, 5, 5);                   // fantasma da fita ainda vazia
      }
    }

    // cabeça da carga: branco -> ciano, cintilando
    for (int j = 0; j < 6; j++) {
      int pos = lit - 1 - j;
      if (pos >= 0 && pos < NUM_LEDS) {
        uint8_t k = 255 - j * 38;
        leds[pos] = CRGB(k, 255, 255);
        leds[pos].nscale8_video(qsub8(255, random8(0, 45)));
      }
    }
  }
  else if (t < ARMA_T_PEAK) {
    float q = (float)(t - ARMA_T_CHARGE) / (float)(ARMA_T_PEAK - ARMA_T_CHARGE);
    uint8_t wh = (uint8_t)(q * 255.0f);
    uint8_t v  = 170 + (uint8_t)(q * 85.0f);
    CRGB c(wh, 255, 255);
    c.nscale8_video(qsub8(v, random8(0, 50)));
    fill_solid(leds, NUM_LEDS, c);
  }
  else if (t < ARMA_T_DECAY) {
    float d   = (float)(t - ARMA_T_PEAK) / (float)(ARMA_T_DECAY - ARMA_T_PEAK);
    float inv = 1.0f - d;
    uint8_t wh = (d < 0.4f) ? (uint8_t)((1.0f - d / 0.4f) * 255.0f) : 0;
    uint8_t v  = (uint8_t)(255.0f * inv * inv);
    v = qsub8(v, scale8(random8(0, 60), (uint8_t)(inv * 255.0f)));
    CRGB c(wh, 255, 255);
    c.nscale8_video(v);
    fill_solid(leds, NUM_LEDS, c);
  }
  else {
    float r = (float)(t - ARMA_T_DECAY) / (float)(ARMA_MS - ARMA_T_DECAY);
    int remaining = (int)((1.0f - r) * NUM_LEDS);
    unsigned long fadeIn = (t - ARMA_T_DECAY) / 25UL;
    uint8_t amp = (fadeIn > 26UL) ? 26 : (uint8_t)fadeIn;
    uint8_t v = qadd8(amp, scale8(beatsin8(36, 0, 20), amp > 0 ? 255 : 0));

    fill_solid(leds, NUM_LEDS, CRGB::Black);
    for (int i = 0; i < remaining; i++) {
      leds[i] = CRGB(0, 255, 255);
      leds[i].nscale8_video(v);
    }
    // ponta quente onde a energia está escoando
    for (int j = 0; j < 4; j++) {
      int pos = remaining - 1 - j;
      if (pos >= 0 && pos < NUM_LEDS) {
        leds[pos] = CRGB(0, 255, 255);
        leds[pos].nscale8_video(qadd8(v, 70 - j * 16));
      }
    }
  }
}

// HIPERSONO: fita AMARELA piscando durante o som (2,30 s).
// O brilho acompanha o áudio: discreto no silêncio inicial, máximo no pico
// (~1,2 s) e baixando até o fim.
void runHipersono(unsigned long t) {
  uint8_t env;
  if (t < 400UL)        env = 60;
  else if (t < 1200UL)  env = 90 + (uint8_t)((t - 400UL) * 165UL / 800UL);
  else if (t < 1600UL)  env = 255;
  else                  env = 255 - (uint8_t)((t - 1600UL) * 215UL / 699UL);

  bool on = ((t / 120UL) % 2UL) == 0;             // pisca a cada 120 ms
  if (on) {
    CRGB c(255, 190, 0);                          // amarelo
    c.nscale8_video(env);
    fill_solid(leds, NUM_LEDS, c);
  } else {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  }
}

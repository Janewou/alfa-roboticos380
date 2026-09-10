#include <Adafruit_NeoPixel.h> 

  

  

// ===================================================== 

// PINAGEM 

// ===================================================== 

  

#define SENSOR_PIN  0 

#define LED_PIN     1 

#define BUZZER_PIN  2 

  

#define NUMPIXELS   8 

  

  

// ===================================================== 

// ANEL WS2812B 

// ===================================================== 

  

Adafruit_NeoPixel ring(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800); 

  

  

// ===================================================== 

// CONTROLE DO SENSOR 

// ===================================================== 

  

bool ultimoEstado; 

  

  

// ===================================================== 

// SETUP 

// ===================================================== 

  

void setup() { 

  

  // TCRT5000 

  pinMode(SENSOR_PIN, INPUT); 

  

  // Buzzer 

  pinMode(BUZZER_PIN, OUTPUT); 

  noTone(BUZZER_PIN); 

  

  // Anel 

  ring.begin(); 

  ring.setBrightness(60); 

  ring.clear(); 

  ring.show(); 

  

  // Estado inicial do sensor 

  ultimoEstado = digitalRead(SENSOR_PIN); 

} 

  

  

// ===================================================== 

// LOOP PRINCIPAL 

// ===================================================== 

  

void loop() { 

  

  // Novo acionamento 

  if (sensorDisparou()) { 

  

    // Alarme 

    tocarAlarme(); 

  

    // Começa sequência 

    executarSequencia(); 

  } 

  

  delay(10); 

} 

  

  

// ===================================================== 

// DETECÇÃO DO TCRT5000 

// ===================================================== 

// 

// LOW  = sem detecção 

// HIGH = mão detectada 

// 

// Só dispara na transição: 

// LOW → HIGH 

// ===================================================== 

  

bool sensorDisparou() { 

  

  bool estadoAtual = digitalRead(SENSOR_PIN); 

  

  bool disparou = false; 

  

  if (!ultimoEstado && estadoAtual) { 

    disparou = true; 

  } 

  

  ultimoEstado = estadoAtual; 

  

  return disparou; 

} 

  

  

// ===================================================== 

// ALARME 

// ===================================================== 

  

void tocarAlarme() { 

  

  // Beep 1 

  tone(BUZZER_PIN, 1800); 

  delay(150); 

  noTone(BUZZER_PIN); 

  

  delay(80); 

  

  // Beep 2 

  tone(BUZZER_PIN, 2200); 

  delay(150); 

  noTone(BUZZER_PIN); 

  

  delay(80); 

  

  // Beep 3 

  tone(BUZZER_PIN, 2600); 

  delay(250); 

  noTone(BUZZER_PIN); 

  

  delay(100); 

} 

  

  

// ===================================================== 

// NOVO ACIONAMENTO 

// ===================================================== 

// 

// Esta função é chamada sempre que o TCRT5000 

// detectar um novo acionamento durante a animação. 

// 

// Ela: 

// 1. apaga imediatamente os LEDs 

// 2. toca o alarme 

// 3. sinaliza que devemos reiniciar 

// ===================================================== 

  

bool verificarReinicio() { 

  

  if (sensorDisparou()) { 

  

    // Mata imediatamente a animação 

    ring.clear(); 

    ring.show(); 

  

    // Toca o alarme NOVAMENTE 

    tocarAlarme(); 

  

    return true; 

  } 

  

  return false; 

} 

  

  

// ===================================================== 

// EXECUTA TODA A SEQUÊNCIA 

// ===================================================== 

  

void executarSequencia() { 

  

  bool reiniciar; 

  

  do { 

  

    reiniciar = false; 

  

  

    // ------------------------------------------------- 

    // 1. VERMELHO 

    // ------------------------------------------------- 

  

    if (giraCor(255, 0, 0, 6)) { 

      reiniciar = true; 

      continue; 

    } 

  

  

    // ------------------------------------------------- 

    // 2. AMARELO 

    // ------------------------------------------------- 

    // 

    // 255,220,0 = amarelo mais puro 

    // 

  

    if (giraCor(255, 85, 0, 4)) { 

      reiniciar = true; 

      continue; 

    } 

  

  

    // ------------------------------------------------- 

    // 3. VERDE 

    // ------------------------------------------------- 

  

    for (int repeticao = 0; repeticao < 3; repeticao++) { 

  

      if (encheVerde()) { 

        reiniciar = true; 

        break; 

      } 

  

      if (piscaVerde(3)) { 

        reiniciar = true; 

        break; 

      } 

    } 

  

    if (reiniciar) { 

      continue; 

    } 

  

  

    // ------------------------------------------------- 

    // FINAL 

    // ------------------------------------------------- 

  

    ring.clear(); 

    ring.show(); 

  

  } while (reiniciar); 

} 

  

  

// ===================================================== 

// GIRO DE UMA COR 

// ===================================================== 

// 

// Retorna: 

// true  = novo acionamento 

// false = terminou normalmente 

// ===================================================== 

  

bool giraCor(byte r, byte g, byte b, int voltas) { 

  

  for (int v = 0; v < voltas; v++) { 

  

    for (int i = 0; i < NUMPIXELS; i++) { 

  

      // Novo acionamento 

      if (verificarReinicio()) { 

        return true; 

      } 

  

      ring.clear(); 

  

      ring.setPixelColor( 

        i, 

        ring.Color(r, g, b) 

      ); 

  

      ring.show(); 

  

  

      // Aguarda monitorando o sensor 

      if (esperarComSensor(120)) { 

  

        ring.clear(); 

        ring.show(); 

  

        tocarAlarme(); 

  

        return true; 

      } 

    } 

  } 

  

  return false; 

} 

  

  

// ===================================================== 

// ENCHE VERDE 

// ===================================================== 

  

bool encheVerde() { 

  

  ring.clear(); 

  

  for (int i = 0; i < NUMPIXELS; i++) { 

  

    // Novo acionamento 

    if (verificarReinicio()) { 

      return true; 

    } 

  

    ring.setPixelColor( 

      i, 

      ring.Color(0, 255, 0) 

    ); 

  

    ring.show(); 

  

  

    if (esperarComSensor(250)) { 

  

      ring.clear(); 

      ring.show(); 

  

      tocarAlarme(); 

  

      return true; 

    } 

  } 

  

  return false; 

} 

  

  

// ===================================================== 

// PISCA VERDE 

// ===================================================== 

  

bool piscaVerde(int vezes) { 

  

  for (int i = 0; i < vezes; i++) { 

  

    // ------------------------------------------------- 

    // LIGA 

    // ------------------------------------------------- 

  

    if (verificarReinicio()) { 

      return true; 

    } 

  

    // Liga todos os LEDs manualmente 

    for (int j = 0; j < NUMPIXELS; j++) { 

  

      ring.setPixelColor( 

        j, 

        ring.Color(0, 255, 0) 

      ); 

    } 

  

    ring.show(); 

  

  

    if (esperarComSensor(250)) { 

  

      ring.clear(); 

      ring.show(); 

  

      tocarAlarme(); 

  

      return true; 

    } 

  

  

    // ------------------------------------------------- 

    // DESLIGA 

    // ------------------------------------------------- 

  

    if (verificarReinicio()) { 

      return true; 

    } 

  

    ring.clear(); 

    ring.show(); 

  

  

    if (esperarComSensor(250)) { 

  

      ring.clear(); 

      ring.show(); 

  

      tocarAlarme(); 

  

      return true; 

    } 

  } 

  

  

  // --------------------------------------------------- 

  // MANTÉM VERDE 

  // --------------------------------------------------- 

  

  for (int j = 0; j < NUMPIXELS; j++) { 

  

    ring.setPixelColor( 

      j, 

      ring.Color(0, 255, 0) 

    ); 

  } 

  

  ring.show(); 

  

  

  if (esperarComSensor(500)) { 

  

    ring.clear(); 

    ring.show(); 

  

    tocarAlarme(); 

  

    return true; 

  } 

  

  return false; 

} 

  

  

// ===================================================== 

// ESPERA MONITORANDO O SENSOR 

// ===================================================== 

// 

// Durante a espera, o sensor continua sendo monitorado. 

// 

// Retorna: 

// true  = novo acionamento 

// false = tempo terminou 

// ===================================================== 

  

bool esperarComSensor(unsigned long tempo) { 

  

  unsigned long inicio = millis(); 

  

  while (millis() - inicio < tempo) { 

  

    if (sensorDisparou()) { 

      return true; 

    } 

  

    delay(5); 

  } 

  

  return false; 

} 
# 👽 Alfa Robóticos — OBR Artística 2026

> **Building Better Worlds**

> **Observação:** Este README foi preparado como documentação técnica do projeto apresentado na OBR Artística 2026.

![Banner](banner.png)

---

# Sobre o Projeto

Este repositório documenta o desenvolvimento da apresentação artística da equipe **Alfa Robóticos**, inspirada na franquia **Alien**.

Nosso objetivo foi unir robótica, programação, eletrônica, cenografia e atuação para criar uma experiência imersiva baseada na atmosfera da nave **USCSS Nostromo**.

Durante meses de desenvolvimento, a equipe trabalhou **três dias por semana**, permanecendo frequentemente **até às 18h**, realizando programação, soldagem, modelagem 3D, montagem eletrônica, pintura, ensaios e integração entre todos os sistemas.

---

# Tecnologias

## Hardware

- Arduino Nano
- LEGO Mindstorms EV3
- micro:bit
- Sensor VL53L0X (Time of Flight)
- Sensor Ultrassônico LEGO
- Sensor TCRT5000
- WS2812 LED Strip
- WS2812 LED Ring
- Impressão 3D

## Software

- C++
- MicroPython
- HTML
- CSS
- JavaScript
- OpenCV
- MediaPipe
- Git
- GitHub

---

# Sistemas Desenvolvidos

## HUD

Desenvolvida em HTML, CSS e JavaScript.

A HUD utiliza OpenCV e MediaPipe para reconhecimento de gestos em tempo real através da câmera, permitindo controlar eventos da apresentação sem controles físicos.

## Iluminação Inteligente

### WS2812 LED Strip

Foi utilizada uma fita WS2812 instalada no cockpit da Nostromo e no ovo alienígena.

Cada LED possui controle individual, permitindo animações RGB sincronizadas com a narrativa.

### VL53L0X (Time of Flight)

O ovo alienígena utiliza um sensor VL53L0X.

Quando um personagem se aproxima, o sensor mede continuamente a distância utilizando tecnologia Time of Flight (laser infravermelho).

O Arduino Nano interpreta essa distância e altera em tempo real os efeitos da fita WS2812, aumentando brilho, alterando velocidade e mudando as cores conforme a aproximação.

Fluxo:

VL53L0X → Arduino Nano → WS2812 LED Strip

### WS2812 LED Ring

Foi utilizado um LED Ring WS2812 nos óculos de um personagem.

O anel realiza animações circulares RGB sincronizadas com momentos específicos do roteiro.

---

# Comunicação

Os robôs comunicam-se utilizando rádio através do micro:bit, sincronizando diferentes eventos da apresentação.

---

# Robôs

A apresentação utiliza robôs LEGO EV3 integrados com sensores e sistemas automatizados.

Também foi desenvolvida uma porta automatizada integrada ao roteiro.

---

# Áudio

O projeto possui trilhas sonoras, narração, efeitos industriais e alarmes sincronizados com cada cena.

---

# Lore

Foi desenvolvido um universo narrativo próprio inspirado na franquia Alien, incluindo roteiro, arquivos de lore e falas.

---

# Desenvolvimento

Ao longo de vários meses:

- Programação
- Soldagem
- Pintura
- Impressão 3D
- Ensaios
- Testes
- Correção de bugs
- Versionamento utilizando Git e GitHub

---

# Equipe

## João Gabriel V. Camargo

- Soldagem
- LEDs WS2812 do painel
- LEDs WS2812 do ovo alienígena
- Sistema de áudio
- Versionamento no GitHub
- Desenvolvimento do roteiro
- Atuação como David

## Matheus A. da Silva

- HUD (HTML, CSS e JavaScript)
- Arquivos de lore
- Desenvolvimento dos robôs
- Porta automatizada
- Narração
- Integração entre hardware e software

## Victor Muniz Aguiar

- Organização geral
- Operação da apresentação
- Óculos com WS2812 LED Ring
- Colagens artísticas
- Direção artística
- Cenografia

## Pietro Borso Felisberto

- Proton Gun
- Operação
- Pinturas
- Acabamentos
- Acrílicos
- Modelagem e impressão 3D

---

# Estrutura

```
Arduino/
EV3/
HUD/
Lore/
Audio/
3D/
Documentacao/
README.md
LICENSE
```

---

# Licença

MIT License

Copyright (c) 2026 Alfa Robóticos

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction...

from pybricks.hubs import PrimeHub 

from pybricks.pupdevices import Motor 

from pybricks.parameters import Port 

from pybricks.robotics import DriveBase 

from pybricks.tools import wait 

  

  

# ========================================== 

# CONFIGURAÇÕES 

# ========================================== 

  

DISTANCIA_CM = 10 

  

SENTIDO = "FRENTE" 

  

VELOCIDADE = 300 

  

DIAMETRO_RODA_MM = 56 

  

DISTANCIA_ENTRE_RODAS_MM = 100 

  

  

# ========================================== 

# HUB 

# ========================================== 

  

hub = PrimeHub(observe_channels=[1]) 

  

  

# ========================================== 

# MOTORES 

# ========================================== 

  

motorA = Motor(Port.A) 

motorB = Motor(Port.B) 

  

  

# ========================================== 

# BASE MOTRIZ 

# ========================================== 

  

base = DriveBase( 

    motorA, 

    motorB, 

    DIAMETRO_RODA_MM, 

    DISTANCIA_ENTRE_RODAS_MM 

) 

  

  

# ========================================== 

# X = PROGRAMA RODANDO 

# ========================================== 

  

hub.display.icon([ 

    [100, 0, 0, 0, 100], 

    [0, 100, 0, 100, 0], 

    [0, 0, 100, 0, 0], 

    [0, 100, 0, 100, 0], 

    [100, 0, 0, 0, 100] 

]) 

  

  

print("==============================") 

print("SPIKE - BASE MOTRIZ") 

print("==============================") 

print("Aguardando RFID...") 

print("Distancia:", DISTANCIA_CM, "cm") 

print("Sentido:", SENTIDO) 

  

  

# ========================================== 

# LOOP 

# ========================================== 

  

while True: 

  

    mensagem = hub.ble.observe(1) 

  

    if mensagem == "ABRIR": 

  

        print("RFID RECEBIDO!") 

        print("Abrindo...") 

  

        distancia = DISTANCIA_CM * 10 

  

        if SENTIDO == "TRAS": 

            distancia = -distancia 

  

        base.settings( 

            straight_speed=VELOCIDADE 

        ) 

  

        base.straight(distancia) 

  

        base.stop() 

  

        print("Movimento concluido.") 

  

        # Espera o comando atual desaparecer 

        while hub.ble.observe(1) is not None: 

            wait(100) 

  

        print("Aguardando novo RFID...") 

  

    wait(50) 
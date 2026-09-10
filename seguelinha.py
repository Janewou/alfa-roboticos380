import hub
import motor
import distance_sensor
import runloop

# ---  AJUSTE NO OLHÔMETRO AQUI  ---
# Primeira distância (antes de girar)
DISTANCIA_EM_METROS = 3.1

# Nova variável para a segunda distância (depois do giro)
SEGUNDA_DISTANCIA_EM_METROS = 3.42

# --- CONFIGURAÇÃO DOS GANHOS DO PID ---
Kp = 1.8
Ki = 0.02
Kd = 0.4

# --- VARIÁVEIS DE CONTROLE INTERNAS ---
velocidade_base = 800
angulo_alvo = 0

# Conversão automática de metros para graus (roda de 88mm)
graus_alvo_distancia = int((DISTANCIA_EM_METROS * 1000) / (88 * 3.14159) * 360)
segundos_graus_alvo = int((SEGUNDA_DISTANCIA_EM_METROS * 1000) / (88 * 3.14159) * 360)

erro_anterior = 0
integral = 0

async def main():
    global erro_anterior, integral

    print("Iniciando programa...")
    print("Aguardando 1ª aproximacao da mao no sensor ultrassonico (Porta D)...")

    # --- 1. AGUARDAR PRIMEIRA APROXIMAÇÃO DA MÃO ---
    while True:
        distancia = distance_sensor.distance(hub.port.D)
        if distancia is not None and 0 < distancia < 150:
            print("1ª Mao detectada! Iniciando sequencia...")
            break
        await runloop.sleep_ms(50)

    # Liga o motor acessório na Porta B
    motor.run(hub.port.B, 100)

    # Zera o giroscópio e os contadores das rodas
    hub.motion_sensor.reset_yaw(0)
    motor.reset_relative_position(hub.port.E, 0)
    motor.reset_relative_position(hub.port.F, 0)
    await runloop.sleep_ms(500)

    # --- 2. ANDAR RETO (PRIMEIRO PERCURSO) ---
    print("Andando em linha reta (Trecho 1)...")
    while abs(motor.relative_position(hub.port.F)) < graus_alvo_distancia:
        valores_giro = hub.motion_sensor.tilt_angles()
        angulo_atual = valores_giro[0] / 10

        erro = angulo_alvo - angulo_atual
        proporcional = erro
        integral += erro
        derivativo = erro - erro_anterior

        correcao = (Kp * proporcional) + (Ki * integral) + (Kd * derivativo)
        correcao = max(min(correcao, 250), -250)

        v_E = int(velocidade_base - correcao)
        v_F = int(velocidade_base + correcao)

        v_E = max(min(v_E, 1000), -1000)
        v_F = max(min(v_F, 1000), -1000)

        motor.run(hub.port.E, -v_E)
        motor.run(hub.port.F, v_F)

        erro_anterior = erro
        await runloop.sleep_ms(20)

    # PARADA MECÂNICA CRÍTICA
    motor.stop(hub.port.E, stop=motor.HOLD)
    motor.stop(hub.port.F, stop=motor.HOLD)
    await runloop.sleep_ms(700)

    # REZERA O GIROSCÓPIO para a curva
    hub.motion_sensor.reset_yaw(0)
    await runloop.sleep_ms(300)

    # --- 3. FAZER O GIRO DE 90 GRAUS PARA A ESQUERDA ---
    print("Iniciando curva controlada de 90 graus para a esquerda...")
    while True:
        valores_giro = hub.motion_sensor.tilt_angles()
        angulo_atual = valores_giro[0] / 10

        if abs(angulo_atual) >= 90:
            break

        motor.run(hub.port.E, 100)
        motor.run(hub.port.F, 100)
        await runloop.sleep_ms(10)

    # PARA TOTALMENTE APÓS O GIRO
    motor.stop(hub.port.E, stop=motor.HOLD)
    motor.stop(hub.port.F, stop=motor.HOLD)
    print("Giro concluido. Aguardando 2ª aproximacao da mao...")
    await runloop.sleep_ms(500)

    # --- 4. AGUARDAR SEGUNDA APROXIMAÇÃO DA MÃO ---
    while True:
        distancia = distance_sensor.distance(hub.port.D)
        if distancia is not None and 0 < distancia < 150:
            print("2ª Mao detectada! Iniciando segundo percurso...")
            break
        await runloop.sleep_ms(50)

    # Rezera os encoders e o giroscópio para iniciar o novo trecho reto do zero
    hub.motion_sensor.reset_yaw(0)
    motor.reset_relative_position(hub.port.E, 0)
    motor.reset_relative_position(hub.port.F, 0)
    erro_anterior = 0
    integral = 0
    await runloop.sleep_ms(500)

    # --- 5. ANDAR RETO (SEGUNDO PERCURSO DE 2 METROS) ---
    print("Andando em linha reta (Trecho 2)...")
    while abs(motor.relative_position(hub.port.F)) < segundos_graus_alvo:
        valores_giro = hub.motion_sensor.tilt_angles()
        angulo_atual = valores_giro[0] / 10

        erro = angulo_alvo - angulo_atual
        proporcional = erro
        integral += erro
        derivativo = erro - erro_anterior

        correcao = (Kp * proporcional) + (Ki * integral) + (Kd * derivativo)
        correcao = max(min(correcao, 250), -250)

        v_E = int(velocidade_base - correcao)
        v_F = int(velocidade_base + correcao)

        v_E = max(min(v_E, 1000), -1000)
        v_F = max(min(v_F, 1000), -1000)

        motor.run(hub.port.E, -v_E)
        motor.run(hub.port.F, v_F)

        erro_anterior = erro
        await runloop.sleep_ms(20)

    # --- 6. PARAR TUDO DEFINITIVAMENTE ---
    motor.stop(hub.port.E, stop=motor.HOLD)
    motor.stop(hub.port.F, stop=motor.HOLD)
    motor.stop(hub.port.B)
    print("Sequencia total finalizada com sucesso!")

# Executa o loop assíncrono
runloop.run(main())

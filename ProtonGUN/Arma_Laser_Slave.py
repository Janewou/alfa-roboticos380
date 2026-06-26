from microbit import *
import radio

radio.on()
radio.config(group=42)

# ALTERAR AQUI
MY_ID = "MB4"

# -----------------------------
# FRAMES DO >
# -----------------------------

frames = [
    Image("90000:"
          "09000:"
          "00900:"
          "09000:"
          "90000"),

    Image("09000:"
          "00900:"
          "00090:"
          "00900:"
          "09000"),

    Image("00900:"
          "00090:"
          "00009:"
          "00090:"
          "00900"),
]

# -----------------------------
# EFEITO FINAL
# -----------------------------

final_frames = [
    Image("00000:"
          "00000:"
          "00900:"
          "00000:"
          "00000"),

    Image("00000:"
          "00900:"
          "09090:"
          "00900:"
          "00000"),

    Image("00000:"
          "09990:"
          "09990:"
          "09990:"
          "00000"),

    Image("99999:"
          "90009:"
          "90009:"
          "90009:"
          "99999"),
]

# -----------------------------
# LOOP PRINCIPAL
# -----------------------------

while True:

    msg = radio.receive()

    if msg:

        # LIMPA TODOS
        if msg == "CLEAR":
            display.clear()

        # LIMPA APENAS MB4
        elif msg == "CLEAR4" and MY_ID == "MB4":
            display.clear()

        # COMANDOS DESTE MICROBIT
        elif msg.startswith(MY_ID):

            parts = msg.split(":")

            if len(parts) > 1:

                cmd = parts[1]

                # LIMPAR DISPLAY
                if cmd == "CLEAR":
                    display.clear()

                # MOSTRAR FRAME
                else:
                    display.show(frames[int(cmd)])

        # EFEITO FINAL SOMENTE MB4
        elif msg.startswith("FINAL") and MY_ID == "MB4":

            parts = msg.split(":")
            idx = int(parts[1])

            display.show(final_frames[idx])

    sleep(35)
from microbit import *
import radio

radio.on()
radio.config(group=42)

shake_count = 0
running = False

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
          "00100:"
          "00000:"
          "00000"),

    Image("00000:"
          "01110:"
          "01010:"
          "01110:"
          "00000"),

    Image("11111:"
          "10001:"
          "10001:"
          "10001:"
          "11111")
]

# -----------------------------
# RESET
# -----------------------------

def reset_all():
    display.clear()
    radio.send("CLEAR")
    global running
    running = False

# -----------------------------
# EFEITO FINAL
# -----------------------------

def final_effect():

    for i in range(5):

        for idx in range(len(final_frames)):

            radio.send("FINAL:" + str(idx))
            sleep(80)

        radio.send("CLEAR4")
        sleep(80)

# -----------------------------
# ANIMAÇÃO
# -----------------------------

def run_animation():

    global running
    running = True

    speeds = [240, 160, 90]

    for speed in speeds:

        # =========================
        # MB1
        # =========================

        display.show(frames[0])
        sleep(speed)

        display.show(frames[1])
        sleep(speed)

        display.show(frames[2])
        sleep(speed)

        display.clear()

        sleep(220)

        # =========================
        # MB2
        # =========================

        radio.send("MB2:0")
        sleep(speed)

        radio.send("MB2:1")
        sleep(speed)

        radio.send("MB2:2")
        sleep(speed)

        radio.send("MB2:CLEAR")

        sleep(220)

        # =========================
        # MB3
        # =========================

        radio.send("MB3:0")
        sleep(speed)

        radio.send("MB3:1")
        sleep(speed)

        radio.send("MB3:2")
        sleep(speed)

        radio.send("MB3:CLEAR")

        sleep(220)

        # =========================
        # MB4
        # =========================

        radio.send("MB4:0")
        sleep(speed)

        radio.send("MB4:1")
        sleep(speed)

        radio.send("MB4:2")
        sleep(speed)

        radio.send("MB4:CLEAR")

        sleep(220)

    final_effect()

    running = False

# -----------------------------
# LOOP PRINCIPAL
# -----------------------------

while True:

    if accelerometer.was_gesture("shake"):

        shake_count += 1

        display.show(str(shake_count))
        sleep(250)
        display.clear()

        if shake_count >= 3 and not running:

            shake_count = 0
            run_animation()

    if button_a.was_pressed() and not running:

        shake_count = 0
        run_animation()

    if button_b.was_pressed():

        shake_count = 0
        reset_all()

    sleep(50)
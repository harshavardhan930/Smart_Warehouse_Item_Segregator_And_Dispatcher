import socket
import tkinter as tk

HOST = "127.0.1.1"   # or your Raspberry Pi IP
PORT = 5000

s = socket.socket()
connected = False

try:
    s.connect((HOST, PORT))
    connected = True
    print("Connected to Pi!")
except Exception as e:
    print("Could not connect:", e)


SAFE1 = 90
SAFE2 = 30
SAFE3 = 140
SAFE4 = 140


def send(event=None):
    if not connected:
        return
    data = f"{s1.get()},{s2.get()},{s3.get()},{s4.get()}\n"
    try:
        s.send(data.encode())
    except:
        pass


def adjust(slider, delta):
    """increase/decrease selected slider"""
    value = slider.get() + delta
    value = max(0, min(180, value))
    slider.set(value)
    send()


root = tk.Tk()
root.title("Servo Calibrato Keyboard Enabled")
root.geometry("400x350")

lbl = tk.Label(root, text="TAB to change slider  |  Arrows to adjust")
lbl.pack(pady=5)

s1 = tk.Scale(root, from_=0, to=180, orient="horizontal", length=300, label="Servo 1")
s2 = tk.Scale(root, from_=0, to=180, orient="horizontal", length=300, label="Servo 2")
s3 = tk.Scale(root, from_=0, to=180, orient="horizontal", length=300, label="Servo 3")
s4 = tk.Scale(root, from_=0, to=180, orient="horizontal", length=300, label="Servo 4")

sliders = [s1, s2, s3, s4]
for sld in sliders:
    sld.pack(pady=10)
    sld.bind("<B1-Motion>", send)
    sld.bind("<ButtonRelease-1>", send)

# ---- Set SAFE INITIAL VALUES ----
s1.set(SAFE1)
s2.set(SAFE2)
s3.set(SAFE3)
s4.set(SAFE4)


# --- Keyboard navigation + control ---
current_index = 0
sliders[current_index].focus_set()


def focus_next(event=None):
    global current_index
    current_index = (current_index + 1) % len(sliders)
    sliders[current_index].focus_set()
    return "break"


def focus_prev(event=None):
    global current_index
    current_index = (current_index - 1) % len(sliders)
    sliders[current_index].focus_set()
    return "break"


def key_handler(event):
    slider = sliders[current_index]

    if event.keysym in ("Right", "Up"):
        adjust(slider, +1)
    elif event.keysym in ("Left", "Down"):
        adjust(slider, -1)
    elif event.keysym == "Next":        # Page Down
        adjust(slider, -5)
    elif event.keysym == "Prior":       # Page Up
        adjust(slider, +5)
    elif event.keysym == "Home":
        slider.set(0)
        send()
    elif event.keysym == "End":
        slider.set(180)
        send()


root.bind("<Tab>", focus_next)
root.bind("<ISO_Left_Tab>", focus_prev)  # Shift+Tab on Linux
root.bind("<Shift-Tab>", focus_prev)
root.bind("<Key>", key_handler)


# ---- Send initial safe pose to Pi ----
if connected:
    send()

root.mainloop()

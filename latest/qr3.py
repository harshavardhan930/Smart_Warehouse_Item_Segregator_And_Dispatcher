import cv2
import os
from picamera2 import Picamera2
import time

fifo_path = "/tmp/qrpipe"

# Create FIFO if not exists
if not os.path.exists(fifo_path):
    os.mkfifo(fifo_path)

# Open FIFO safely (read-write + non-blocking)
fifo = os.open(fifo_path, os.O_RDWR | os.O_NONBLOCK)

# Initialize Pi Camera
picam2 = Picamera2()

config = picam2.create_preview_configuration(
    main={"size": (640, 480), "format": "BGR888"}
)
picam2.configure(config)
picam2.start()

time.sleep(2)  # camera warm-up

detector = cv2.QRCodeDetector()

while True:
    # Capture frame from Pi camera
    img = picam2.capture_array()

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    retval, decoded_info, points, _ = detector.detectAndDecodeMulti(gray)

    found = False

    # Draw QR box
    if points is not None:
        for i in range(len(points)):
            pts = points[i].astype(int)
            for j in range(4):
                cv2.line(
                    img,
                    tuple(pts[j]),
                    tuple(pts[(j + 1) % 4]),
                    (0, 255, 0),
                    2
                )

    # Send QR text
    if decoded_info:
        for text in decoded_info:
            if text:
                print(text)
                os.write(fifo, (text + "\n").encode())
                found = True

    # Send 0 if no QR found
    if not found:
        print("0")
        os.write(fifo, b"0\n")

    cv2.imshow("QR Scanner", img)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

picam2.stop()
cv2.destroyAllWindows()
os.close(fifo)

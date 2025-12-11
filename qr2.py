import cv2
import os


fifo_path = "/tmp/qrpipe"

# Create FIFO if not exists
if not os.path.exists(fifo_path):
    os.mkfifo(fifo_path)

# Open FIFO safely (read-write mode avoids "no reader" error)
fifo = os.open(fifo_path, os.O_RDWR | os.O_NONBLOCK)

url = "http://192.168.1.136:8080/video"
cap = cv2.VideoCapture(url)
detector = cv2.QRCodeDetector()

while True:
    ret, img = cap.read()
    if not ret:
        continue

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    retval, decoded_info, points, _ = detector.detectAndDecodeMulti(gray)

    # Draw box
    if points is not None:
        for i in range(len(points)):
            pts = points[i].astype(int)
            for j in range(4):
                cv2.line(img, tuple(pts[j]), tuple(pts[(j+1) % 4]), (0, 255, 0), 2)

    found = False

    # Send QR text
    if decoded_info:
        for text in decoded_info:
            if text:
                print(text)
                os.write(fifo, (text + "\n").encode())
                found = True

    # Send 0 if no QR
    if not found:
        print("0")
        os.write(fifo, b"0\n")

    cv2.imshow("QR Scanner", img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
os.close(fifo)

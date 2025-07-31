import serial
import time

ser = serial.Serial('COM3', 115200, timeout=10)

# Wait for the "LEN:<number>" line from Arduino
while True:
    line = ser.readline().decode(errors='ignore').strip()
    if line.startswith("LEN:"):
        length_str = line[4:]
        try:
            img_length = int(length_str)
            print(f"Image length received: {img_length} bytes")
            break
        except ValueError:
            print("Invalid length received, waiting again...")

# Generate unique filename with timestamp
filename = f"photo_{int(time.time())}.jpg"

with open(filename, "wb") as f:
    bytes_read = 0
    while bytes_read < img_length:
        to_read = min(1024, img_length - bytes_read)
        data = ser.read(to_read)
        if not data:
            print("No more data received, ending early.")
            break
        f.write(data)
        bytes_read += len(data)

print(f"Saved image as {filename}")

ser.close()

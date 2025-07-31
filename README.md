# Arducam-Button
Arduino program that, with a push of a button, allows you to take pictures that get saved to your PC with the use of Arducam OV5642 5MP mini.

BEFORE USING THIS:
I used the Arduino IDE and VS Code for the Python script. I also had to download the Arducam example sketch for the camera, which includes a file called memorysaver. In the memorysaver file, I had to uncomment a line of code, which was (#define OV5642_MINI_5MP_PLUS), for it to work.


HOW TO USE:
1. Make sure you have the USB that connects to your Arduino in COM3 then upload the code to the Arduino.
2. Briefly open the serial monitor (make sure the serial is at 115200) to see if it connected to the camera. In the serial, it should say something like OV5642 detected. Once it connects, close out of the Arduino serial.
3. Next, open the Python script and run it.
4. Once you run the Python script, the program waits for you to take the photo.
5. Hold down on the button, and in the terminal, it will say what the image is saved as, and that's how you know it's done running.
6. Now you can check the photo you took.

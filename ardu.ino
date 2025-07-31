#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"

// Ensure OV5642 is enabled in memorysaver.h
#if !(defined OV5642_MINI_5MP_PLUS)
#error Please enable OV5642_MINI_5MP_PLUS in memorysaver.h
#endif

const int CS = 10;
const int buttonPin = 5;

ArduCAM myCAM(OV5642, CS);

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
 uint8_t vid, pid;
uint8_t temp;
#if defined(__SAM3X8E__)
  Wire1.begin();
  Serial.begin(115200);
#else
  Wire.begin();
  Serial.begin(115200);
#endif
Serial.println(F("ACK CMD ArduCAM Start! END"));
// set the CS as an output:
pinMode(CS, OUTPUT);
digitalWrite(CS, HIGH);
// initialize SPI:
SPI.begin();
 //Reset the CPLD
myCAM.write_reg(0x07, 0x80);
delay(100);
myCAM.write_reg(0x07, 0x00);
delay(100); 
while(1){
  //Check if the ArduCAM SPI bus is OK
  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);
  if (temp != 0x55){
    Serial.println(F("ACK CMD SPI interface Error! END"));
    delay(1000);continue;
  }else{
    Serial.println(F("ACK CMD SPI interface OK. END"));break;
  }
}
  while(1){
    //Check if the camera module type is OV5642
    myCAM.wrSensorReg16_8(0xff, 0x01);
    myCAM.rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
    if((vid != 0x56) || (pid != 0x42)){
      Serial.println(F("ACK CMD Can't find OV5642 module! END"));
      delay(1000);continue;
    }
    else{
      Serial.println(F("ACK CMD OV5642 detected. END"));break;
    } 
  }
//Change to JPEG capture mode and initialize the OV5642 module
myCAM.set_format(JPEG);
myCAM.InitCAM();

  myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);   //VSYNC is active HIGH
  myCAM.OV5642_set_JPEG_size(OV5642_320x240);
delay(1000);
myCAM.clear_fifo_flag();
myCAM.write_reg(ARDUCHIP_FRAMES,0x00);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) { // Button is pressed
    Serial.println("Button pressed! Taking picture...");
    delay(200); // Debounce

    myCAM.clear_fifo_flag();
    myCAM.start_capture();

    while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)); // Wait for capture

    Serial.println("Image capture done.");

    uint32_t len = myCAM.read_fifo_length();
    if (len == 0 || len >= 0x7FFFFF) {
      Serial.println("Invalid image size.");
      return;
    }
Serial.print("LEN:");
Serial.println(len);


    myCAM.CS_LOW();
    myCAM.set_fifo_burst();

    for (uint32_t i = 0; i < len; i++) {
      uint8_t data = SPI.transfer(0x00);
      Serial.write(data); // Send image data over Serial
    }

    myCAM.CS_HIGH();
    //Serial.println("\nDone sending image.");

    delay(1000); // Wait before allowing next press
    Serial.println("Waiting for button press...");
  }
}

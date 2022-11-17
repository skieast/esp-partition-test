//USB-otg must be enabled. ARDUINO_HW_CDC_ON_BOOT=1 ARDUINO_USB_MODE=1 ARDUINO_USB_CDC_ON_BOOT=1
#include <Arduino.h>
#include "USB.h"
#include "SPIFFS.h"

#define USBSerial Serial
//USBCDC USBSerial;

void init_spiff() {
    
  if(!SPIFFS.begin(false)){
    USBSerial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  File file = SPIFFS.open("/readme.txt");
  if(!file){
    USBSerial.println("Failed to open file for reading");
    return;
  }
  
  USBSerial.println("File Content:");
  while(file.available()){
    USBSerial.write(file.read());
  }
  file.close();
}

void setup() {
  USBSerial.begin();
  while(!USBSerial) {
    //wait for serial to connect
  }
  printf("Setup: Initialize SPIFFS\n");
  init_spiff();
  // put your setup code here, to run once:
}

void loop() {
  // printf("Loop: Hello world!\n");
  // put your main code here, to run repeatedly:
}

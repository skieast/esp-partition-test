//USB-otg must be enabled. ARDUINO_HW_CDC_ON_BOOT=1 ARDUINO_USB_MODE=1 ARDUINO_USB_CDC_ON_BOOT=1
#include <Arduino.h>
#include "USB.h"
#include "SPIFFS.h"
#include  "/home/bsegal/esp/esp-idf/components/spi_flash/include/esp_partition.h"


#define USBSerial Serial
//USBCDC USBSerial;

// Get the string name of type enum values used in this example
static const char* get_type_str(esp_partition_type_t type)
{
    switch(type) {
        case ESP_PARTITION_TYPE_APP:
            return "ESP_PARTITION_TYPE_APP";
        case ESP_PARTITION_TYPE_DATA:
            return "ESP_PARTITION_TYPE_DATA";
        default:
            return "UNKNOWN_PARTITION_TYPE"; // type not used in this example
    }
}

// Get the string name of subtype enum values used in this example
static const char* get_subtype_str(esp_partition_subtype_t subtype)
{
    switch(subtype) {
        case ESP_PARTITION_SUBTYPE_DATA_NVS:
            return "ESP_PARTITION_SUBTYPE_DATA_NVS";
        case ESP_PARTITION_SUBTYPE_DATA_PHY:
            return "ESP_PARTITION_SUBTYPE_DATA_PHY";
        case ESP_PARTITION_SUBTYPE_APP_FACTORY:
            return "ESP_PARTITION_SUBTYPE_APP_FACTORY";
        case ESP_PARTITION_SUBTYPE_DATA_FAT:
            return "ESP_PARTITION_SUBTYPE_DATA_FAT";
        case ESP_PARTITION_SUBTYPE_APP_OTA_0:
        case ESP_PARTITION_SUBTYPE_APP_OTA_1:
        case ESP_PARTITION_SUBTYPE_APP_OTA_2:   // Till 15
            return "ESP_PARTITION_SUBTYPE_APP_OTA";
        case ESP_PARTITION_SUBTYPE_DATA_SPIFFS:
            return "ESP_PARTITION_SUBTYPE_DATA_SPIFFS";
        case ESP_PARTITION_SUBTYPE_DATA_ESPHTTPD:
            return "ESP_PARTITION_SUBTYPE_DATA_ESPHTTPD";
        case ESP_PARTITION_SUBTYPE_APP_TEST:
            return "ESP_PARTITION_SUBTYPE_APP_TEST";
        default:
            return "UNKNOWN_PARTITION_SUBTYPE"; // subtype not used in this example
    }
}

void list_partitions()
{

    esp_partition_iterator_t it;

    USBSerial.println("Iterating through app partitions...");

    it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
    // Loop through all matching partitions, in this case, all with the type 'data' until partition with desired
    // label is found. Verify if its the same instance as the one found before.
    for (; it != NULL; it = esp_partition_next(it)) {
        const esp_partition_t *part = esp_partition_get(it);
        USBSerial.printf( "'%s' \t[%s - %s] at offset 0x%x with size 0x%x (%dK)\n", 
                    part->label, get_type_str(part->type), get_subtype_str(part->subtype), 
                    part->address, part->size,part->size/1024);
    }

    // Release the partition iterator to release memory allocated for it
    esp_partition_iterator_release(it);

}


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
  // Print chip information
  USBSerial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
	USBSerial.printf("This chip has %d cores\n", ESP.getChipCores());
  USBSerial.printf("%dMB\n", ESP.getFlashChipSize() / (1024 * 1024));
  list_partitions();
  // put your setup code here, to run once:
}

void loop() {
  // printf("Loop: Hello world!\n");
  // put your main code here, to run repeatedly:
}

/*
#AUTHOR Дмитрий Витальевич Мельник
#MAIL i.x.c.o.n@yandex.ru
#LICENSE Использование  только  для не коммерческих проектов, без упоминания автора. При использовании автор не несет ни какой отвественности, и не дает ни какой гарантии работы. Не ведет поддержку или исправление ошибок. 
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 5; //In seconds
BLEScan* pBLEScan;
String str_BLE(256);
byte strBLE[17];
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      str_BLE = advertisedDevice.toString().c_str();
      Serial.print(str_BLE);
      Serial.println(); 
      int w = 0;
      int i = 0;
      for (i = 0; i < 50; i++){
        if (str_BLE[i] == ':'){
          w++;
          if (w == 2){
            i = i + 2;
            break;
            };            
            };
          };
            for (int a = 0; a < 17; a++){
              strBLE[a] = str_BLE[a+i];
              //Serial.print(str_BLE[a+i]);
            };
            for (i = 0; i<17; i++){
              Serial.print(char(strBLE[i]));
              };
            Serial.println();

                  
        };
      //Serial.print(str_BLE_cat); 
      //Serial.printf(str_BLE);
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      
      };


void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}

/*
#AUTHOR Дмитрий Витальевич Мельник
#MAIL i.x.c.o.n@yandex.ru
#LICENSE Использование  только  для не коммерческих проектов, без упоминания автора. При использовании автор не несет ни какой отвественности, и не дает ни какой гарантии работы. Не ведет поддержку или исправление ошибок. 
*/

#include <Esp.h>
#include <esp32-hal-bt.h>
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"

#define NAMEDEVICE "ESP32"
/*
02 01 06 09 FF 16 0F 01 Префикс
03 57 уровень топлива
24 напряжение питания
1C термиапература
00 версия проткоола
0A 09 разделитель 
69 32 31 30 38 30 30 31 имя
05 77 разделитель
61 05 e4 01 время UTC
00 Версия пакета
*/
uint8_t heart_rate_service_uuid [25] = { 0x16, 0x0F, 0x01, 0x00, 0x00, 0x24, 0x00, 0x00, 0x0A, 0x09, 0x69, 0x32, 0x31, 0x30, 0x38, 0x30, 0x30, 0x31, 0x05, 0x77, 0x01, 0xE4, 0x05, 0x61, 0x00};
// 3 и 4 топливо, 6 температура
uint8_t temp = 0;

uint8_t manufacturer_data [] = { 0x01, 0x06};

esp_ble_adv_data_t advData = {
  .set_scan_rsp = false,
  .include_name = false,//true
  .include_txpower = false,//true
  .min_interval = 0x0006,
  .max_interval = 0x0010,
  .appearance = 0x00,
  .manufacturer_len = 0x1A,
  .p_manufacturer_data = heart_rate_service_uuid,
  .service_data_len = 0,
  .p_service_data = NULL,
  .service_uuid_len = 0,
  .p_service_uuid = NULL,
  //.service_data_len = 0,
  //.p_service_data = NULL,
  .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};
static void checkResult(esp_err_t err, const __FlashStringHelper* s)
{
	if (err != ESP_OK) {
		Serial.printf("[ERR %08x] ", err);
		Serial.println(s);
	}
}

void setup()
{
  Serial.begin(115200);

  if (!btStart()) Serial.println(F("Failed to initialize bluetooth controller."));
  checkResult(esp_bluedroid_init(), F("Failed to initialize bluedroid."));
  checkResult(esp_bluedroid_enable(), F("Failed to enable bluedroid."));
  checkResult(esp_ble_gap_set_device_name(NAMEDEVICE), F("Failed to set device name"));
  checkResult(esp_ble_gap_config_adv_data(&advData), F("Failed to configure advertisement data."));

  esp_ble_adv_params_t advParams = { 0 };
  advParams.adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
  advParams.adv_int_max = 0x0100;
  advParams.adv_int_min = 0x0100;
  advParams.adv_type = ADV_TYPE_NONCONN_IND;
  advParams.channel_map = ADV_CHNL_ALL;
  advParams.own_addr_type = BLE_ADDR_TYPE_PUBLIC;
  
  checkResult(esp_ble_gap_start_advertising(&advParams), F("Failed to start advertising."));

}

void loop()
{
  auto status = esp_bluedroid_get_status();
  Serial.printf("bluedroid status: %02x\n temp: %d\n", status, temp);
  heart_rate_service_uuid [6] = temp++;
  checkResult(esp_ble_gap_config_adv_data(&advData), F("Failed to configure advertisement data."));
  delay(1000);
}

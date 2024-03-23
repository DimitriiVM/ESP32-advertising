#include <Esp.h>
#include <esp32-hal-bt.h>
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"

#define NAMEDEVICE "ESP32"

uint8_t heart_rate_service_uuid[16] = {0x30, 0x31, 0x32, 0x34, 0x35, 0x33, 0x36, 0x37, 0x34, 0x36, 0x37, 0x33, 0x36, 0x34, 0x35, 0x00};

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
  
  esp_ble_adv_data_t advData = { 0 };
  advData.flag = ESP_BLE_ADV_FLAG_NON_LIMIT_DISC;
  advData.include_name = true;
  advData.min_interval = 0x0006;
  advData.max_interval = 0x0010;
  advData.service_uuid_len = sizeof(heart_rate_service_uuid);
  advData.p_service_uuid = heart_rate_service_uuid;
  advData.set_scan_rsp = false;
  advData.include_txpower = true;
  advData.appearance = 0x00;
  advData.manufacturer_len = 0;
  advData.p_manufacturer_data =  NULL;
  advData.service_data_len = 0;
  advData.p_service_data = NULL;
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
  Serial.printf("bluedroid status: %02x\n", status);
  delay(1000);
}

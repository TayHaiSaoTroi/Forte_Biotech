#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "time.h" // New
#include <HTTPUpdate.h>
//#include <WiFiClientSecure.h>
#include <EEPROM.h>
#include <Update.h>
#include <ESPmDNS.h>
//#include <nvs32.h>


#ifndef _DEFINE_H
#define _DEFINE_H
#include <Arduino.h>
#define _EEPROM_SIZE 512
#define cDebug                          (0)
#define cMainDebug                      (0)
#define cSensorDebug                    (0)
#define cButtonDebug                    (0)
#define cDisplayDebug                   (0)

#define DEBUG_COM Serial
#define HEADER_FORMAT(fmt) "<%s>:<%d> " fmt "\r\n", pathToFileName(__FILE__), __LINE__
#define dbg_main(format, ...) (cMainDebug&cDebug) ? DEBUG_COM.printf(HEADER_FORMAT(format), ##__VA_ARGS__) : NULL
#define dbg_sensor(format, ...) (cSensorDebug&cDebug) ? DEBUG_COM.printf(HEADER_FORMAT(format), ##__VA_ARGS__) : NULL
#define dbg_button(format, ...) (cButtonDebug&cDebug) ? DEBUG_COM.printf(HEADER_FORMAT(format), ##__VA_ARGS__) : NULL
#define dbg_display(format, ...) (cDisplayDebug&cDebug) ? DEBUG_COM.printf(HEADER_FORMAT(format), ##__VA_ARGS__) : NULL

#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_MISO   19
#define TFT_CS     22
#define TFT_DC     21
#define TFT_RESET  17

// #define PIN_Sensor1 36
// #define PIN_Sensor2 34
// #define PIN_Sensor3 35
// #define PIN_Sensor4 32
// #define PIN_Sensor5 33

#define ON_LED      25

#define valueMAXsensor  3000
#define NumberButton    3
#define TimePressAnti  50
#define calibTime       5000
#define BUTTON_RED      13
#define BUTTON_BLUE     16
#define BUTTON_WHITE    4  
#define settingTime   5000
#define numberCalibration 300


//static String  ssid = "";
// static String  password = "";
//static const char*id_wifi="";
//static const char*pass="";
static String ip = "";
static String FirmwareVer={"V1.3"};

#define URL_fw_Version "https://raw.githubusercontent.com/1MinhTan27/Update_firmware_esp32.github.io/main/bin_version.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/1MinhTan27/Update_firmware_esp32.github.io/main/light_meter.ino.bin"

#endif

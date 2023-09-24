#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "define.h"
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

extern String  ssid;
extern String  password;

void connectWIFI();
void saveCredentialsToEEPROM();
void loadCredentialsFromEEPROM();
#endif
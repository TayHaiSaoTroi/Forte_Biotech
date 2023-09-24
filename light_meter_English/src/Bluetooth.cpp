#include "Bluetooth.h"

BluetoothSerial SerialBT;


String ssid = "";
String password = "";

void connectWIFI(){
  ssid = ""; // Initialize as empty
  password = ""; // Initialize as empty
  SerialBT.begin();
  while(!SerialBT.hasClient()) { // check if bluetooth connection is established
    delay(10);
  }
  SerialBT.println("Establishing Wifi...");
  delay(3000);
  SerialBT.println("Enter Wifi ID:");
  delay(3000);
  while(ssid.isEmpty()){
    ssid = SerialBT.readString();
    ssid.trim();
  }
  SerialBT.println("Wifi is " + ssid);
  SerialBT.println("Enter Wifi password:");
  while(password.isEmpty()){
    password = SerialBT.readString();
    password.trim();
  }
  SerialBT.println("Password is "+password);
  SerialBT.println("Wifi setup completed!");
  delay(3000);
  SerialBT.end();
}

void saveCredentialsToEEPROM() {
  EEPROM.begin(_EEPROM_SIZE);
  
  // Write SSID length to EEPROM
  unsigned char ssidLength = ssid.length();
  EEPROM.put(10, ssidLength); // change from 0 to 10 to avoid conflict with max-min calibration value

  // Write SSID to EEPROM
  for (unsigned char i = 0; i < ssidLength; i++) {
    EEPROM.write(i + sizeof(ssidLength) + 10, ssid[i]);
  }

  // Write password length to EEPROM
  unsigned char passwordLength = password.length();
  EEPROM.put(sizeof(ssidLength) + ssidLength + 10, passwordLength);

  // Write password to EEPROM
  for (int i = 0; i < passwordLength; i++) {
    EEPROM.write(i + sizeof(ssidLength) + ssidLength + sizeof(passwordLength) + 10, password[i]);
  }

  EEPROM.commit();
  EEPROM.end();
}

void loadCredentialsFromEEPROM() {
  EEPROM.begin(_EEPROM_SIZE);
  
  unsigned char ssidLength;
  EEPROM.get(10, ssidLength);

  ssid = ""; // Clear ssid before loading
  // Read SSID from EEPROM
  for (unsigned char i = 0; i < ssidLength; i++) {
    char c = EEPROM.read(i + sizeof(ssidLength) + 10);
    ssid += c;
  }

  unsigned char passwordLength;
  EEPROM.get(sizeof(ssidLength) + ssidLength + 10, passwordLength);

  password = ""; // Clear password before loading
  // Read password from EEPROM
  for (unsigned char i = 0; i < passwordLength; i++) {
    char c = EEPROM.read(i + sizeof(ssidLength) + ssidLength + sizeof(passwordLength) + 10);
    password += c;
  }

  EEPROM.end();
  // Serial.println("Loaded sucessful!");
  // Serial.println(ssid);
  // Serial.println(password);
}

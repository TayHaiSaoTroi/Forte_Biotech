#include "displayCLD.h"
#include <U8g2lib.h>
#include "displayresources.h"
#include "define.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "sensor.h"
// #include "update_firmware.h"
#include "Bluetooth.h"
#define Forte_Green 0x25F8
#define VIOLET  0xA81F  
String measure_value = "";
String id = "RE23080701";
bool butt = 0; // 0: blue, 1: green


displayCLD::displayCLD(/* args */) {
  this->bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO);
  this->display = new Arduino_ILI9341(this->bus, TFT_RESET);
}

displayCLD::~displayCLD() {
}

void displayCLD::begin() {
  this->display->begin();
  this->display->fillScreen(BLACK);
  this->display->setRotation(1);
  this->display->setUTF8Print(true);  
}

void displayCLD::logoFortebiotech() {
  this->display->fillScreen(BLACK);
  this->display->fillTriangle(80, 60, 132, 30, 132, 90, this->display->color565(16, 55, 50));
  this->display->fillTriangle(130, 100, 78, 70, 78, 130, this->display->color565(16, 55, 50));
  this->display->fillTriangle(88, 140, 132, 110, 132, 170, this->display->color565(16, 45, 20));
  this->display->fillTriangle(142, 30, 252, 10, 142, 68, this->display->color565(10, 30, 35));
  this->display->fillTriangle(142, 140, 142, 170, 192, 130, this->display->color565(16, 65, 30));
  this->display->setFont(u8g2_font_unifont_t_vietnamese1);
  this->display->setTextSize(2);
  this->display->setTextColor(this->display->color565(16, 55, 70));
  this->display->setCursor(150, 90);
  this->display->print("FORTE");
  this->display->setCursor(150, 120);
  this->display->print("BIOTECH");
  this->display->setFont(u8g2_font_helvB08_tf);
  this->display->setTextColor(Forte_Green);
  this->display->setTextSize(1);
  this->display->setCursor(80, 190);
  this->display->print("TEST   PRAWNS   WITH   RAPID");
  this->display->setCursor(100, 220);
  this->display->print("PROFIT   NO   LIMIT");
  this->display->setCursor(5, 234);
  this->display->print(FirmwareVer);
  this->display->setFont(u8g2_font_unifont_t_vietnamese1);
  delay(3000);
  dbg_display("logo thanh cong");
  Serial.println("ForteBiotech");
}

void displayCLD::screen_Start() {
  ip = WiFi.localIP().toString().c_str(); // Taking ip address
  this->display->fillScreen(BLACK);
  this->display->setTextSize(1);
  this->display->fillRect(108, 0, 108, 20, Forte_Green);
  this->display->setCursor(110, 15);
  this->display->setTextColor(BLACK);
  this->display->println("FORTE BIOTECH");
  this->display->drawBitmap(1, 1, logoFBT, 35, 34, Forte_Green);
  this->display->drawBitmap(285, 209, shrimp, 35, 29, Forte_Green);
  this->display->drawRect(0, 0, 320, 240, Forte_Green);
  //this->display->drawRect(108, 0, 108, 20, Forte_Green);
  if(butt == 0){
    this->display->drawCircle(35, 110, 25, Forte_Green);
    this->display->fillCircle(35, 110, 20, Forte_Green);
  }
  else{
    this->display->drawCircle(35, 110, 25, GREEN);
    this->display->fillCircle(35, 110, 20, GREEN);
  }
  this->display->setTextSize(2);
  this->display->setCursor(75, 100);
  if(butt == 0){
    this->display->setTextColor(Forte_Green);
    this->display->println("Press blue");
  }
  else{
    this->display->setTextColor(GREEN);
    this->display->println("Press green");
  }
  this->display->setCursor(95, 140);
  this->display->print("to start");
  this->display->setTextSize(1);
  this->display->setCursor(5, 230);
  this->display->print(ip);
  Serial.println("Nguyen Minh Tan");
  Serial.println(ip);
  // dbg_display("Nhan nut do de bat dau");
  //   while (digitalRead(blueButton)) {} //NOTE: sua lai nut nhan
}  
void displayCLD::Choose_tube() {
  this->display->fillScreen(BLACK);
  this->display->setTextSize(2);
  this->display->drawRect(0, 0, 320, 240, BLUE);
  this->display->setTextColor(WHITE);
  this->display->setCursor(50, 30);
  this->display->print("Select tube ");
  this->display->setTextColor(Forte_Green);
  this->display->setCursor(5, 60);
  this->display->print("+Positive");
  this->display->setCursor(5, 90);
  this->display->print("+Negative");
  this->display->setCursor(5, 120);
  this->display->print("+EHP");
  this->display->setCursor(5, 150);
  this->display->print("+EMS ");
  this->display->setCursor(5, 180);
  this->display->print("+WSSV");
  
 
switch (step) {
    case 1:
      {
        this->display->fillTriangle(200, 50, 232, 40, 232, 60, RED);
        measure_value="Positive";
        break;
      }
    case 2:
      {
        this->display->fillTriangle(200, 80, 232, 70, 232, 90, RED);
        measure_value="Negative";
        break;
      }
    case 3:
      {
        this->display->fillTriangle(200, 110, 232, 100, 232, 120, RED);
        measure_value="EHP";
        break;
      }
    case 4:
      {
        this->display->fillTriangle(200, 140, 232, 130, 232, 150, RED);
        measure_value="EMS";
        break;
      }
    case 5:
      {
        this->display->fillTriangle(200, 170, 232, 160, 232, 180, RED);
        measure_value="WSSV";
        break;
      }
    default:
      break;
  
  }

  this->display->setTextSize(1);
  this->display->setTextColor(RED);
  this->display->setCursor(5,210);
  this->display->print("Press red button to select tube");
  this->display->setCursor(5, 230);
  if(butt == 0){
    this->display->setTextColor(Forte_Green);
    this->display->print("Press blue button to continue");
  }
  else{
    this->display->setTextColor(GREEN);
    this->display->print("Press green button to continue");
  }
}

void displayCLD::waiting_Readsensor() {
  // digitalWrite(ON_LED, HIGH);
  this->display->fillScreen(BLACK);
  this->display->drawBitmap(285, 1, logoFBT, 35, 34, Forte_Green);
  this->display->drawRect(0, 0, 320, 240, Forte_Green);
  this->display->setTextSize(3);
  this->display->setTextColor(RED);
  this->display->setCursor(15, 90);
  this->display->print("In progress");
  this->display->setTextSize(2);
  this->display->setTextColor(Forte_Green);
  this->display->setCursor(15, 150);
  this->display->println("Waiting...");
  _sensor.flagReadSensor = true;
}

void displayCLD::prepare() {
  this->display->fillScreen(BLACK);
  this->display->setTextSize(2);
  this->display->setTextColor(Forte_Green);
  this->display->setCursor(5, 60);
  this->display->println("Put the tube inside and close the lid");
  // this->display->drawRect(0, 0, 320, 240, Forte_Green);
  // this->display->drawRect(185, 0, 135, 35, Forte_Green);
   this->display->drawRect(6, 140, 309, 80, RED);
  this->display->drawRect(5, 139, 309, 82, RED);
  for (int i = 0; i <= 310; i += 10) {
    static int x1 = 0, y1 = 100, x2 = 10, y2 = 110, y3 = 120;
    this->display->drawLine(x1 + i, y1, x2 + i, y2, PINK);
    this->display->drawLine(x1 + i, y3, x2 + i, y2, PINK);
  }
  this->display->drawCircle(50, 180, 22, RED);
  this->display->fillCircle(50, 180, 17, RED);
  this->display->setTextSize(2);
  this->display->setTextColor(RED);
  this->display->setCursor(90, 175);
  this->display->println("Press red");
  this->display->setCursor(90, 205);
  this->display->print("to measure #");
  this->display->print(this->couter);
}

void displayCLD::screen_Result() {
  this->display->fillScreen(BLACK);
  this->display->setTextSize(2);
  this->display->setTextColor(YELLOW);
  this->display->setCursor(5, 35);
  this->display->printf("# %d:", this->couter);
  this->display->setTextSize(7);
  this->display->setTextColor(RED);

  if (_sensor.result_Sensor[0][this->couter - 1] < 10)
    this->display->setCursor(135, 140);
  else if (_sensor.result_Sensor[0][this->couter - 1] < 100)
    this->display->setCursor(110, 140);
  else if (_sensor.result_Sensor[0][this->couter - 1] < 1000)
    this->display->setCursor(85, 140);
  else
    this->display->setCursor(50, 140);

  this->display->println(_sensor.result_Sensor[0][this->couter - 1]);

  this->display->setTextSize(1);
  this->display->setCursor(0, 190);
  if(butt == 0){
    this->display->setTextColor(Forte_Green);
    this->display->println("Blue: Redo");
  }
  else{
    this->display->setTextColor(GREEN);
    this->display->println("Green: Redo");
  }
  this->display->setTextColor(RED);
  this->display->println("RED: Continue");
  this->display->setTextColor(WHITE);
  this->display->println("WHITE: Check the previous result");
}

String encMeasureValue(const String&  str){
  if (str == "Chứng dương") {
    return "Chung%20duong";
  } else if (str == "Chứng âm") {
    return "Chung%20am";
  } else if (str == "Đốm trắng") {
    return "Dom%20trang";
}
    else return str;
}

void displayCLD::log_data(){
  // Google script ID and required credentials
String GOOGLE_SCRIPT_ID = "AKfycbxHUNzXFcPF5KwK4p21t--g7yr8ovq4n9cuTCzQMSBdgZo7T_OBbaPqh37zClL70hEP";    // change Gscript ID
String encodedMeasureValue = encMeasureValue(measure_value);
  String lan_1 = String(_sensor.result_Sensor[0][0]); 
  String lan_2 = String(_sensor.result_Sensor[0][1]);
  String lan_3 = String(_sensor.result_Sensor[0][2]);
  String trung_binh = String(_sensor.AverageResult[0]);
  //String urlFinal = "https://script.google.com/macros/s/AKfycbymqZg24T27Y9TKGP5zZMGKup7B0EBYgC42xWBCJEKOw06VFG8l1S8Zt0EBr9QzNOJ6Tw/exec?date=2023&lan_1=100&lan_2=200&lan_3=300&trung_binh=250";
  String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"&lan_1=" + lan_1
  + "&lan_2=" + lan_2+"&lan_3=" + lan_3+"&trung_binh=" + trung_binh+"&measure_value="+encodedMeasureValue+"&id="+id;
  HTTPClient http;
  http.begin(urlFinal.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
   int httpCode = http.GET(); 
   if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
   http.end();
  }

void displayCLD::screen_Average_Result() {
  this->display->fillScreen(BLACK);

  this->display->setTextSize(2);
  this->display->drawRect(0,0,320,240,BLUE);
  this->display->setCursor(5,30);
  this->display->print("Tube:");
  this->display->setCursor(125,30);
  this->display->print(measure_value);
  this->display->drawLine(0,45,320,45,BLUE);
  this->display->setCursor(115,80);
  this->display->print("Result");
  this->display->setTextColor(RED);  // ong 1
  this->display->setCursor(5, 110);
  this->display->printf("1st:  %d \n\r", _sensor.result_Sensor[0][0]);
  this->display->setCursor(5, 140);
  this->display->printf("2nd:  %d \n\r", _sensor.result_Sensor[0][1]);
  this->display->setCursor(5, 170);
  this->display->printf("3rd:  %d \n\r", _sensor.result_Sensor[0][2]);
  this->display->setCursor(5, 200);
  this->display->setTextColor(YELLOW);  // ong 1 
  this->display->printf("Average: %d", _sensor.AverageResult[0]);
  this->display->setTextSize(1);
  this->display->setCursor(5, 230);
  if(butt == 0){
    this->display->setTextColor(Forte_Green);  // ong 1
    this->display->print("Blue: Startover ");
  }
  else{
    this->display->setTextColor(GREEN);  // ong 1
    this->display->print("Green: Startover ");
  }
}

void displayCLD::screen_Calib() {
  // dbg_display("nhan do calib, nhan xanh ket thuc, nhan trang de format");
  this->display->fillScreen(BLACK);
  this->display->setTextSize(2);
  this->display->setTextColor(WHITE);
  this->display->setCursor(30, 35);
  this->display->println("Calibration mode");
  this->display->setCursor(3, 80);
  this->display->print("Type of tube:");
  this->display->setTextColor(ORANGE);
  if (_sensor.typecalib == 0)
    this->display->print("Highest");
  else
    this->display->print("Lowest");

  this->display->setCursor(0, 190);
  this->display->setTextSize(1);
  if(butt == 0){
    this->display->setTextColor(Forte_Green);
    this->display->println("Blue: Exit");
  }
  else{
    this->display->setTextColor(GREEN);
    this->display->println("Green: Exit");
  }
  this->display->setTextColor(RED);
  this->display->println("Red: Calibrate");
  this->display->setTextColor(WHITE);
  this->display->println("White: Delete calibration");
}

void displayCLD::waiting_Calib() {
 this->display->fillScreen(BLACK);
  this->display->drawBitmap(285, 1, logoFBT, 35, 34, Forte_Green);
  this->display->drawRect(0, 0, 320, 240, Forte_Green);
  this->display->setTextSize(2);
  this->display->setTextColor(RED);
  this->display->setCursor(15, 90);
  this->display->print("Calibrating");
  this->display->setTextSize(2);
  this->display->setTextColor(Forte_Green);
  this->display->setCursor(15, 150);
  this->display->println("Waiting...");
}

void displayCLD::screen_Calib_Complete() {
  this->display->fillScreen(BLACK);
  this->display->setTextSize(2);
  this->display->setTextColor(ORANGE);
  this->display->setCursor(35, 35);
  this->display->println("calibration mode");
  this->display->setTextSize(3);
  this->display->setCursor(45, 130);
  this->display->setTextColor(Forte_Green);
  this->display->println("Successful!");
  delay(3000);
}
void displayCLD::setting()
{
  this->display->fillScreen(BLACK);
  this->display->drawRect(0,0,320,240,Forte_Green);
  this->display->setTextSize(2);
  this->display->drawRect(5,10,310,100,RED);
  this->display->setCursor(10,70);
  this->display->setTextColor(RED);
  this->display->print("WiFi Setup");
//  this->display->drawBitmap(210-61, 140-45, WiFi_symbol, 60, 44, BLACK);
  this->display->drawRect(5,120,310,100,WHITE);
  this->display->setCursor(10,180);
  this->display->setTextColor(WHITE);
  this->display->print("Update firmware");
}
// void displayCLD::set_id_wifi()
// { 
//    this->display->fillScreen(WHITE);
//    this->display->setTextSize(1);
//   this->display->setTextColor(BLACK);
//   this->display->setCursor(8,35);
//   this->display->print("Please enter your wifi id...");
//      // this->display->fillRect(0,135,320,105,WHITE);
//      this->display->drawRect(2,2,316,56,BLACK);
//      //this->display->fillRect(5,5,310,50,WHITE);
//       this->display->setTextSize (3);
//       this->display->setTextColor(WHITE);
//       for( j=0; j<5;j++)
//       {
//       for ( i=0; i<10; i++)
//       {
//       this->display->fillRoundRect((i*32), j*35+101-35, 31, 34, 3,BLACK);
//       this->display->setCursor (i*32+5,j*35+132-35 );
//       this->display->print(symbol[j][i]);
//       }
//       }
//       this->display->fillRoundRect(1,171, 31, 34, 3,BLACK);
//       this->display->setTextColor(WHITE);
//       this->display->setTextSize (1);
//       this->display->setCursor (2,190 );
//       this->display->print("Caps");
//       this->display->fillRoundRect(289,171, 31, 34, 3,BLACK);
//       this->display->setTextColor(WHITE);
//       this->display->setTextSize (2);
//       this->display->setCursor (295,200);
//       this->display->print("!");
//       this->display->fillRoundRect(289,136, 31, 34, 3,BLACK);
//       this->display->setTextColor(WHITE);
//       this->display->setTextSize (1);
//       this->display->setCursor (290,155 );
//       this->display->print("Ent");
//       this->display->fillRoundRect(1,206, 223, 34, 3,BLACK);
//       this->display->setTextColor(WHITE);
//       this->display->setTextSize (2);
//       this->display->setCursor (55,230 );
//       this->display->print("Space");
//       this->display->fillRoundRect(225,206, 95, 34, 3,BLACK);
//       this->display->setTextColor(WHITE);
//       this->display->setTextSize (2);
//       this->display->setCursor (230,230 );
//       this->display->print("Clear"); 
//       this->display->setCursor(10,35);
// }

/*
void displayCLD::update_firmware()
{
  int incomingByte;
   delay(1000);
   this->display->fillScreen(BLACK);
   this->display->drawRect(0,0,320,240,Forte_Green);
   this->display->setTextSize(2);
   this->display->setCursor(10,70);
   this->display->print("Cập nhật phần mềm");
    Serial.print(" Active Firmware Version:");
    Serial.println(FirmwareVer);
    if (Serial.available() > 0) {
        incomingByte = Serial.read();
        if (incomingByte == 'U') {
            Serial.println("Firmware Update In Progress..");
            this->display->setCursor(10,120);
            this->display->setTextSize(2);
            this->display->print("Đang cập nhật");
            if (FirmwareVersionCheck()) {
                firmwareUpdate();
                this->display->drawRect(10,80,300,60,BLACK);
                this->display->setCursor(10,120);
                this->display->print("Cập nhật thành công");
            }
        }
    }
}
*/

void displayCLD::set_connect_bluetooth()
{
  this->display->fillScreen(BLACK);
  this->display->drawRect(0,0,320,240,BLUE);
  this->display->setTextSize(2);
  this->display->setTextColor(ORANGE);
  this->display->setCursor(80,30);
  this->display->print("Wifi Setup");
  this->display->drawRect(10,50,300,50,WHITE);
    this->display->setTextColor(WHITE);
    this->display->setTextSize(1);
    this->display->setCursor(20,90);
    this->display->print("ID: "+ssid);
    this->display->drawRect(10,120,300,50,WHITE);
    this->display->setCursor(20,160);
    this->display->print("Password: "+password);
    connectWIFI(); // Obtain Wifi ID and password from user via bluetooth
    saveCredentialsToEEPROM(); // save Wifi ID and password in EEPROM
    loadCredentialsFromEEPROM();
    //Update newScreen
    this->display->fillScreen(BLACK);
    this->display->setTextSize(2);
    this->display->setCursor(30,120);
    this->display->setTextColor(GREEN);
    this->display->print("Setup successful!");
    delay(5000);
    this->display->fillScreen(BLACK);
    this->display->drawRect(0,0,320,240,BLUE);
  this->display->setTextSize(2);
  this->display->setTextColor(ORANGE);
  this->display->setCursor(80,30);
  this->display->print("WIFI setup");
  this->display->drawRect(10,50,300,50,WHITE);
    this->display->setTextColor(WHITE);
    this->display->setTextSize(1);
    this->display->setCursor(20,90);
    this->display->print("ID: "+ssid);
    this->display->drawRect(10,120,300,50,WHITE);
    this->display->setCursor(20,160);
    this->display->print("Password: "+password);
}
void displayCLD::loop() {
  if (this->changeScreen) {
    switch (this->type_infor) {
      case escreenStart:
        {
          dbg_display("escreenStart");
          this->screen_Start();
          _sensor.clear();
          break;
        }
      case ewaitingReadsensor:
        {
          dbg_display("ewaitingReadsensor");
          waiting_Readsensor();
          break;
        }

      case eprepare:
        {
          dbg_display("eprepare");
          this->prepare();
          break;
        }

      case escreenResult:
        {
          dbg_display("escreenResult lan %d", this->couter);
          this->screen_Result();
          break;
        }

      case escreenAverageResult:
        {
          dbg_display("escreenAverageResult");
          _sensor.Average_Result();
          this->screen_Average_Result();
         // this->log_data();
          break;
        }

      case ecalibSensor:
        {
          dbg_display("ecalibSensor");
          this->screen_Calib();
          break;
        }
      case echoosetube:
        {
          this->Choose_tube();
          break;
        }
      case e_setting:
      {
        this->setting();
        break;
      }
     
    case eset_id_wifi:
    {
      //this->set_id_wifi();
      this->set_connect_bluetooth();
      break;
    }
    /*
    case e_update:
    {
      this->update_firmware();
      break;
    }
    */
    case logdata:
    {
      this->log_data();
      break;
    }
      default:
        break;
    }
    this->changeScreen = false;
  }
}

displayCLD _displayCLD;

#ifndef _DISPLAY_LCD_H
#define _DISPLAY_LCD_H
#include "Arduino.h"
#include "U8g2lib.h"
#include "Arduino_GFX_Library.h"


typedef enum
{
    escreenStart,
    ewaitingReadsensor,
    echoosetube,
    eprepare,
    escreenResult,
    escreenAverageResult,
    ecalibSensor,
    e_setting,
    logdata,
    eset_id_wifi
    // e_update
} e_statuslcd;

class displayCLD
{
private:
    /* data */
    Arduino_ESP32SPI *bus;
    Arduino_GFX *display; //   1
    

public:
    displayCLD(/* args */);
    ~displayCLD();

    void begin();
    void logoFortebiotech();
    void screen_Start();
    void Choose_tube();
    void screen_Complete();
    void screen_Result();
    void screen_Average_Result();
    void waiting_Readsensor();
    void prepare();
    void screen_Calib();
    void waiting_Calib();
    void screen_Calib_Complete();
    void log_data();
    void setting();
   // void set_id_wifi();
    //void update_firmware();
   void set_connect_bluetooth();
   
    void loop();

    e_statuslcd type_infor = escreenStart;
    uint8_t couter = 0;
    uint8_t instantStatus[2];
    bool changeScreen = true;
    uint8_t step=1;
    int i=0,j=0;
    uint8_t row=0;
    uint8_t col=0;
    int x=0;
    int y=0;
    int incomingByte;
    

    String symbol[5][10] = {
      {"0","1","2","3","4","5","6","7","8","9"},
  { "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P" },
  { "A", "S", "D", "F", "G", "H", "J", "K", "L","ENT"},
  {  "Caps","Z", "X", "C", "V", "B", "N", "M", "." ,"!"},
  {"Space","Clear"}
};
    // void screen2();
    // void screen3();
};

extern displayCLD _displayCLD;

#endif

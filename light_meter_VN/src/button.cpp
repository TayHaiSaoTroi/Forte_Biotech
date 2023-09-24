#include "button.h"
#include "Ticker.h"
#include "displayCLD.h"
#include "sensor.h"

typedef void (*hanler)();
void buttonRedHandler();
void buttonBlueHandler();
void buttonWhiteHandler();
static void tickerHandler(uint8_t index);
static void tickerHandler1(uint8_t index);

static uint8_t buttons[NumberButton];
static hanler Hanler[NumberButton];
static bool buttonPressed[NumberButton];
static Ticker buttonTicker[NumberButton];
static unsigned long timeAtPress[NumberButton];

buttonManager::buttonManager(/* args */) {
  buttons[0] = BUTTON_RED;
  Hanler[0] = &buttonRedHandler;
  buttons[1] = BUTTON_BLUE;
  Hanler[1] = &buttonBlueHandler;
  buttons[2] = BUTTON_WHITE;
  Hanler[2] = &buttonWhiteHandler;

  for (int i = 0; i < NumberButton; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttons[i]), Hanler[i], CHANGE);
  }
}

buttonManager::~buttonManager() {
}

void buttonProcess(e_statusbutton index) {

  if (!digitalRead(buttons[index]) && (buttonPressed[index] == false)) {
    timeAtPress[index] = millis();
    if (index == B_WHITE) {
      buttonTicker[index].attach_ms(calibTime, &tickerHandler, (uint8_t)index);
    }
    if (index == B_RED) {
      buttonTicker[index].attach_ms(calibTime, &tickerHandler1, (uint8_t)index);
    }
    buttonPressed[index] = true;
  }

  else if ((buttonPressed[index] == true) && ((unsigned long)(millis() - timeAtPress[index]) > TimePressAnti) && ((unsigned long)(millis() - timeAtPress[index]) < calibTime)) {
    buttonPressed[index] = false;
    switch (index) {
      case B_RED:
        {
          if (_sensor.flagback == true) {
            _displayCLD.couter++;
            if (_displayCLD.couter == _displayCLD.instantStatus[0]) {
              _sensor.flagback = false;
              _displayCLD.type_infor = (e_statuslcd)_displayCLD.instantStatus[1];
            } else {
              _displayCLD.type_infor = escreenResult;
            }
            _displayCLD.changeScreen = true;
          }

          else if (_displayCLD.type_infor == ecalibSensor) {
            dbg_button("nut RED - bat dau calib");
            _sensor.flagCalibSensor = true;
          }

          else if (_displayCLD.type_infor == eprepare) {
            _displayCLD.type_infor = ewaitingReadsensor;
            _displayCLD.changeScreen = true;
            dbg_button("nut RED - bat dau do");
          }

          else if (_displayCLD.type_infor == escreenResult && _displayCLD.couter < 3) {
            _displayCLD.couter++;
            _displayCLD.type_infor = eprepare;
            _displayCLD.changeScreen = true;
            dbg_button("nut RED - tiep tuc do");
          }

          else if (_displayCLD.type_infor == escreenResult && _displayCLD.couter >= 3) {
            _displayCLD.couter = 0;
            _displayCLD.type_infor = escreenAverageResult;
            _displayCLD.changeScreen = true;
            dbg_button("nut RED - ket qua tb 3 lan do");
          }
          else if(_displayCLD.type_infor == echoosetube)
          {
            
            if(_displayCLD.step==1)
            {
              _displayCLD.type_infor=echoosetube;
              _displayCLD.changeScreen = true;
              //_displayCLD.step=2;
            }
            else if(_displayCLD.step==2)
            {
              _displayCLD.type_infor=echoosetube;
              _displayCLD.changeScreen = true;
              //_displayCLD.step=3;
            }
            else if(_displayCLD.step==3)
            {
              _displayCLD.type_infor=echoosetube;
              _displayCLD.changeScreen = true;
              //_displayCLD.step=4;
            }
            else if(_displayCLD.step==4)
            {
              _displayCLD.type_infor=echoosetube;
              _displayCLD.changeScreen = true;
             // _displayCLD.step=5;
            }
            else if(_displayCLD.step ==5)
            {
              _displayCLD.type_infor=echoosetube;
              _displayCLD.changeScreen = true;
              _displayCLD.step=0;
            }
            // if(_displayCLD.step>5)
            // {
            //   _displayCLD.step=1;
            //   _displayCLD.type_infor=echoosetube;
            //   _displayCLD.changeScreen = true;
            // break;
            // }

            _displayCLD.step++;
          }  
          else if(_displayCLD.type_infor== e_setting)
          {
            _displayCLD.type_infor=eset_id_wifi;
            _displayCLD.changeScreen=true;
          }
           else if(_displayCLD.type_infor ==escreenAverageResult)
          {
            _displayCLD.type_infor=logdata;
            _displayCLD.changeScreen=true;
          }
          break;
         
        }

      case B_BLUE:
        {
          if (_displayCLD.type_infor == escreenAverageResult) {
            _displayCLD.type_infor = escreenStart;
            _displayCLD.changeScreen = true;
          }

          else if (_displayCLD.type_infor == ecalibSensor) {
            _displayCLD.type_infor = escreenStart;
            _displayCLD.changeScreen = true;
            dbg_button("nut BLUE - huy calib");
          }

          else if (_displayCLD.type_infor == escreenResult) {
            _displayCLD.type_infor = eprepare;
            _displayCLD.changeScreen = true;
            dbg_button("nut BLUE - do lai");
          }

          else if (_displayCLD.type_infor == escreenStart) {
            //_displayCLD.couter = 1;
            _displayCLD.type_infor = echoosetube;
            _displayCLD.changeScreen = true;
            dbg_button("nut BLUE - chuan bi");
          } else if (_displayCLD.type_infor == echoosetube) {
            _displayCLD.couter = 1;
            _displayCLD.type_infor = eprepare;
            _displayCLD.changeScreen = true;
          }
          break;
        }

      case B_WHITE:
        {
          buttonTicker[index].detach();
          if (_displayCLD.type_infor == ecalibSensor) {
            _sensor.flagformatCalib = true;
            dbg_button("nut WHITE - format calib");
          }

          else if (_displayCLD.type_infor == escreenResult || _displayCLD.type_infor == eprepare)  //|| _displayCLD.type_infor == escreenAverageResult
          {
            if (_sensor.flagback == false && _displayCLD.couter > 1) {
              _sensor.flagback = true;
              _displayCLD.instantStatus[0] = _displayCLD.couter;
              _displayCLD.instantStatus[1] = _displayCLD.type_infor;
            }
            if (_displayCLD.couter > 1) {
              _displayCLD.couter--;
              _displayCLD.type_infor = escreenResult;
              _displayCLD.changeScreen = true;
            }
          }
          /*
          else if(_displayCLD.type_infor ==e_setting)
          {
            _displayCLD.type_infor=e_update;
            _displayCLD.changeScreen=true;
          }
          */

          break;
        }

      default:
        break;
    }
  } else {
    buttonPressed[index] = false;
  }
}

static void tickerHandler(uint8_t index) {
  buttonTicker[index].detach();

  if (!digitalRead(buttons[index])) {
    buttonPressed[index] = false;
    _displayCLD.type_infor = ecalibSensor;
    _displayCLD.changeScreen = true;
    dbg_button("nut WHITE - calib");
  }
}
static void tickerHandler1(uint8_t index) {
  buttonTicker[index].detach();
if (!digitalRead(buttons[index])) {
    buttonPressed[index] = false;
    _displayCLD.type_infor = e_setting;
    _displayCLD.changeScreen = true;
    dbg_button("nut Red - setting");
  }
}

void IRAM_ATTR buttonRedHandler() {
  buttonProcess(B_RED);
}

void IRAM_ATTR buttonBlueHandler() {
  buttonProcess(B_BLUE);
}

void IRAM_ATTR buttonWhiteHandler() {
  buttonProcess(B_WHITE);
}

buttonManager _buttonManager;

#include <U8g2lib.h>
#include <Arduino_GFX_Library.h>
Arduino_DataBus *bus = new Arduino_HWSPI(A3 /* DC */, A5 /* CS */);
Arduino_GFX *mylcd = new Arduino_ILI9341(bus, A4 /* RST */);
#define Forte_Green 0x07E0
// temp probe

#include <OneWire.h>
#include <DallasTemperature.h>
#define PIN_ONE_WIRE_BUS A1
OneWire oneWire(PIN_ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// all switches n peltier (mosfet) n buzzer
//#define temperarture A1
#define HEAT A2
#define blueButton 9
#define redButton 6
#define whiteButton 8
#define BUZZER 7
#define LAMP 1
#define LYSIS 2
#define NORMAL 0
#define GOD 100
#define FREQ_test 3000
#define FREQ_start 3000
#define FREQ_preheat 3000
#define FREQ_end 3000
#define FREQ_warn 800
float
ctemp = 0,
heatTime = 0;
unsigned int
heatTemp = 0,
flow = 20,
pset = 1,
timeron = 0,
end_ = 0;
unsigned int timer_value = 10;
unsigned long
timestart = 0,
timedur = 0;
boolean testGUIMODE = false;

void setup() {
  // switches
  pinMode(blueButton, INPUT_PULLUP);
  pinMode(redButton, INPUT_PULLUP);
  pinMode(whiteButton, INPUT_PULLUP);
  // buzzer
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  // mosfets
  pinMode(HEAT, OUTPUT);
  mylcd->begin();
  mylcd->setRotation(1);
  mylcd->setUTF8Print(true);
  mylcd->setFont(u8g2_font_unifont_t_vietnamese1);
  mylcd->fillScreen(BLACK);
  mylcd->setTextSize(3);
  mylcd->setTextColor(0x25F8);
  mylcd->setCursor(5, 100);
  mylcd->println("FORTE BIOTECH");
  mylcd->setTextSize(1);
  mylcd->setCursor(80, 130);
  mylcd->print("Test prawns with RAPID");
  mylcd->setCursor(100, 150);
  mylcd->print("Profit no Limit");
  mylcd->setCursor(5, 240);
  mylcd->print("V3.0");
  //Khoi tao gia tri chinh
  heatTemp = 60;
  timer_value = 10;
  delay(4000);
}

void next_step() { // press red button to continue
  mylcd->fillScreen(BLACK);
  mylcd->setTextSize(2);
  mylcd->setTextColor(RED);
  mylcd->setCursor(50, 100);
  mylcd->print("Press red button ");
  mylcd->setCursor(50, 140);
  mylcd->print("to continue");
}
void preheat() {
  mylcd->fillScreen(BLACK);
  float current_temp = 0;
  bool hit = false;
  if (testGUIMODE == true) {
    ctemp += 4;
    delay(200);
  } else {
    sensors.requestTemperatures();  // Send the command to get temperature readings
    ctemp = sensors.getTempCByIndex(0);
    //    get_temp();
    if (current_temp == 0) {
      current_temp = ctemp;
    }
    mylcd->fillRect(90, 100, 300, 50, BLACK);
    mylcd->setTextSize(4);
    mylcd->setCursor(90, 150);
    mylcd->print(String(ctemp));
  }
  mylcd->setTextSize(2);
  mylcd->setTextColor(WHITE);
  mylcd->setCursor(5, 30);
  mylcd->print("Heating");
  mylcd->setTextColor(Forte_Green);
  mylcd->setTextSize(2);
  mylcd->setCursor(150, 30);
  mylcd->print(heatTemp);
  while (not hit) {
    sensors.requestTemperatures(); // Send the command to get temperature readings
    ctemp = sensors.getTempCByIndex(0);
    if (ctemp != current_temp ) {
      current_temp = ctemp;
      mylcd->fillRect(90, 100, 300, 50, BLACK);
      mylcd->setTextSize(4);
      mylcd->setCursor(90, 150);
      mylcd->print(String(ctemp));
      ////////////////////////////////////////////////////////////////
      mylcd->setCursor(200, 150);
      mylcd->print("℃");
      ////////////////////////////////////////////////////////////////
    }
    if ((ctemp > heatTemp) && (ctemp < heatTemp + 0.5)) {
      hit = true;
    }
    if (ctemp > heatTemp + 0.2) {
      digitalWrite(HEAT, LOW);
    }
    else if (ctemp < heatTemp) {
      digitalWrite(HEAT, HIGH);
    }
    delay(500);
  }
  digitalWrite(HEAT, LOW);
  for (unsigned int i = 0; i < 25; i++) {
    if ((digitalRead(blueButton) || !digitalRead(redButton) || !digitalRead(whiteButton))) {
      tone(BUZZER, FREQ_end, 1000);
      delay(500);
      noTone(BUZZER);
      delay(200);
    }
    else {
      break;
    }
  }
}
//duy tri nhiet do
void heat_maintain(float heatTemp) {
  sensors.requestTemperatures(); // Send the command to get temperature readings
  ctemp = sensors.getTempCByIndex(0);
  if (ctemp < heatTemp) {
    digitalWrite(HEAT, HIGH);
  }
  else {
    digitalWrite(HEAT, LOW);
  }
}
//set nhiet do
void heat_set() {
  mylcd->fillScreen(BLACK);
  mylcd->setTextSize(2);
  mylcd->setTextColor(WHITE);
  mylcd->setCursor(5, 30);
  mylcd->print("Select temperature:");
  mylcd->fillRect(70, 100, 300, 50, BLACK);
  mylcd->setTextSize(4);
  mylcd->setTextColor(WHITE);
  mylcd->setCursor(85, 150);
  mylcd->print(String(heatTemp));
  mylcd->setCursor(193, 150);
  mylcd->print("C");
  mylcd->setTextSize(2);
  mylcd->setCursor(180, 125);
  mylcd->print("o");
  mylcd->setTextSize(1);
  mylcd->setCursor(5, 200);
  mylcd->setTextColor(Forte_Green);
  mylcd->print("GREEN: Dec");
  mylcd->setCursor(120, 200);
  mylcd->setTextColor(RED);
  mylcd->println("RED: Inc");
  mylcd->setCursor(210, 200);
  mylcd->setTextColor(WHITE);
  mylcd->println("WHITE: Select");
  while (digitalRead(whiteButton)) {
    if (!digitalRead(redButton)) {
      if (heatTemp < 85) {
        heatTemp += 1;
        mylcd->fillRect(70, 100, 110, 50, BLACK);
        mylcd->setTextSize(4);
        mylcd->setTextColor(WHITE);
        mylcd->setCursor(85, 150);
        mylcd->print(String(heatTemp));
        delay(300);
        if (heatTemp >= 85) {
          heatTemp = 85;
        }
      }
      else {
        tone(BUZZER, FREQ_preheat, 1000);
        delay(200);
        noTone(BUZZER);
      }
    }
    if (!digitalRead(blueButton)) {
      if (heatTemp > 55) {
        heatTemp -= 1;
        mylcd->fillRect(70, 100, 110, 50, BLACK);
        mylcd->setTextSize(4);
        mylcd->setTextColor(WHITE);
        mylcd->setCursor(85, 150);
        mylcd->print(String(heatTemp));
        delay(300);
      }
      else {
        tone(BUZZER, FREQ_preheat, 1000);
        delay(100);
        noTone(BUZZER);
      }
    }
  }
}
int minuteLeft;
void timer_start(unsigned int dur) {
  mylcd->fillScreen(BLACK);
  timeron = 1;
  minuteLeft = dur;
  timestart = millis();
}
void timer_set() {
  mylcd->fillScreen(BLACK);
  mylcd->setTextSize(2);
  mylcd->setTextColor(WHITE);
  mylcd->setCursor(5, 30);
  mylcd->print("Select time:");
  mylcd->fillRect(90, 100, 300, 50, BLACK);
  mylcd->setTextSize(4);
  mylcd->setTextColor(WHITE);
  mylcd->setCursor(90, 150);
  mylcd->print(String(timer_value));
  mylcd->setCursor(170, 150);
  mylcd->print("min");
  mylcd->setTextSize(1);
  mylcd->setCursor(5, 200);
  mylcd->setTextColor(Forte_Green);
  mylcd->print("GREEN: Dec");
  mylcd->setCursor(120, 200);
  mylcd->setTextColor(RED);
  mylcd->println("RED: Inc");
  mylcd->setCursor(210, 200);
  mylcd->setTextColor(WHITE);
  mylcd->println("WHITE: Select");
  while (digitalRead(whiteButton)) {
    if (!digitalRead(redButton)) {
      if (timer_value < 60) {
        timer_value += 1;
        mylcd->fillRect(90, 100, 60, 50, BLACK);
        mylcd->setTextSize(4);
        mylcd->setTextColor(WHITE);
        mylcd->setCursor(90, 150);
        mylcd->print(String(timer_value));
        delay(300);
        if (timer_value >= 60) {
          timer_value = 60;
        }
      }
      else {
        tone(BUZZER, FREQ_preheat, 1000);
        delay(100);
        noTone(BUZZER);
      }
    }
    if (!digitalRead(blueButton)) {
      if (timer_value > 1) {
        timer_value -= 1;
        mylcd->fillRect(90, 100, 60, 50, BLACK);
        mylcd->setTextSize(4);
        mylcd->setTextColor(WHITE);
        mylcd->setCursor(90, 150);
        mylcd->print(String(timer_value));
        delay(300);
      }
      else {
        tone(BUZZER, FREQ_preheat, 1000);
        delay(100);
        noTone(BUZZER);
      }
    }
  }
  timer_start(timer_value);
  flow += 1;
}
int secondLeft = 0;
long tsec0 = 0;
long  tleft;
int mode = 0 ;

void timer() {
  mylcd->setTextColor(Forte_Green);
  mylcd->setTextSize(5);
  mylcd->setCursor(140, 150);
  mylcd->print(":");
  while (timeron == 1) {
    heat_maintain(heatTemp);
    if (secondLeft == 0) {
      minuteLeft -= 1;
      secondLeft = 59;
      int min0 = minuteLeft % 10;
      int min1 = minuteLeft / 10;
      mylcd->fillRect(100, 100, 40, 50, BLACK);
      mylcd->setCursor(100, 150);
      mylcd->print(min0);
      if (min1 <= 9) {
        mylcd->fillRect(50, 100, 40, 50, BLACK);
        mylcd->setCursor(50, 150);
        mylcd->print(min1);
      }
      if (min0 == 9) {
        mylcd->fillRect(50, 100, 40, 50, BLACK);
        mylcd->setCursor(50, 150);
        mylcd->print(min1);
      }
    }
    else {
      secondLeft -= 1;
    }
    int sec0 = secondLeft % 10;
    int sec1 = secondLeft / 10;
    mylcd->fillRect(230, 100, 40, 50, BLACK);
    mylcd->setCursor(230, 150);
    mylcd->print(sec0);
    if (sec0 == 9) {
      mylcd->fillRect(180, 100, 40, 50, BLACK);
      mylcd->setCursor(180, 150);
      mylcd->print(sec1);
    }
    /////////////////////////////////////////////
    delay(465);
    if (minuteLeft == 0 && secondLeft == 0) {
      timeron = 0;
    }
  }
  mylcd->fillScreen(BLACK);
  mylcd->setTextSize(4);
  mylcd->setCursor(20, 150);
  mylcd->print("COMPLETED");
  digitalWrite(HEAT, LOW);
  for (unsigned int i = 0; i < 20; i++) {
    if ((digitalRead(blueButton) || !digitalRead(redButton) || !digitalRead(whiteButton))) {
      tone(BUZZER, FREQ_end, 1000);
      delay(200);
      noTone(BUZZER);
      delay(200);
    }
    else {
      break;
    }
  }
  mylcd->fillScreen(BLACK);
  secondLeft = 0;

  switch (mode) {
    case 0:
      if (flow == 2 || flow == 5 || flow == 8 || flow == 9) next_step();
      flow += 1;
      break;
    case 1:
      flow += 1;
      break;
    case 2:
      flow = 20;
      break;
  }
}
bool sw_mode;
bool sw1_mode;
bool mode1 = false ;
void select_modestep() {
  sw1_mode = false;
  mylcd->setTextSize(2);
  mylcd->setTextColor(WHITE);
  mylcd->setCursor(10, 30);
  mylcd->print("Select step");
  mylcd->setCursor(20, 100);
  mylcd->print("WHITE:");
  mylcd->setCursor(130, 100);
  mylcd->print("80oC-10min");
  mylcd->setTextColor(RED);
  mylcd->setCursor(20, 160);
  mylcd->print("RED:");
  mylcd->setCursor(130, 160);
  mylcd->print("67oC-60min");
  mylcd->setCursor(20, 220);
  mylcd->setTextColor(Forte_Green);
  mylcd->print("GREEN:");
  mylcd->setCursor(130, 220);
  mylcd->print("67oC-5min");
  while (!sw1_mode) {
    //mylcd->fillScreen(BLACK);
    if (!digitalRead(whiteButton) == HIGH) {
      flow = 0;
      sw1_mode = true;
    }
    if (!digitalRead(redButton) == HIGH) {
      flow = 3;
      sw1_mode = true;
    }
    if (!digitalRead(blueButton) == HIGH) {
      flow = 6;
      sw1_mode = true;
    }
    delay(200);
  }
}

void select_mode() {
  sw_mode = false;
  while (!sw_mode) {
    mylcd->setTextSize(2);
    mylcd->setTextColor(WHITE);
    mylcd->setCursor(5, 30);
    mylcd->print("Select mode");
    mylcd->setTextSize(2);
    mylcd->setTextColor(WHITE);
    mylcd->setCursor(5, 100);
    mylcd->print("WHITE:");
    mylcd->setTextSize(2);
    mylcd->setCursor(120, 100);
    mylcd->print("Select step");
    mylcd->setTextSize(2);
    mylcd->setTextColor(RED);
    mylcd->setCursor(5, 160);
    mylcd->print("RED:");
    mylcd->setCursor(120, 160);
    mylcd->print("Custom");
    mylcd->setTextSize(2);
    mylcd->setTextColor(Forte_Green);
    mylcd->setCursor(5, 220);
    mylcd->print("GREEN:");
    mylcd->setCursor(120, 220);
    mylcd->print("Auto");
    if (!digitalRead(blueButton)) {
      mode = 1;
      sw_mode = true;
      flow = 0;
      delay(200);
    }
    if (!digitalRead(redButton)) {
      mode = 0;
      sw_mode = true;
      flow = 0;
      delay(200);
    }
    if (!digitalRead(whiteButton)) {
      mode = 2;
      sw_mode = true;
      flow = 15;
      delay(200);
    }
  }
}

void loop() {
  if (flow == 20) { // Đã bấm bất kì nút nào để quay lại
    mylcd->fillScreen(BLACK);
    select_mode();
  }
  //MANUAL
  if (mode == 0) {
    //code manual
    if (flow == 0) {
      heat_set();
      timer_set();
      preheat();
      mylcd->fillScreen(BLACK);
    }
    else if (flow == 1) {
      mylcd->setTextSize(2);
      mylcd->setCursor(5, 30);
      mylcd->setTextColor(WHITE);
      mylcd->print("Maintain temp:");
      mylcd->setTextColor(RED);
      mylcd->setCursor(250, 30);
      mylcd->print(heatTemp);
      timer();
    }
    else if (flow == 2) {
      flow = 20;
    }
  }
  //AUTO
  else if (mode == 1) {
    // code auto
    //check for timer

    heat_maintain(heatTemp);
    if (flow == 2) { // Step 1: 80C - 10p
      mylcd->setTextSize(2);
      mylcd->setCursor(5, 30);
      mylcd->setTextColor(WHITE);
      mylcd->print("Maintain temp:");
      mylcd->setTextColor(RED);
      mylcd->setCursor(250, 30);
      mylcd->print(heatTemp);
      timer(); // flow += 1
      next_step();
    }
    if (flow == 5) { // Step 2: 67C - 60p
      mylcd->setTextSize(2);
      mylcd->setCursor(5, 30);
      mylcd->setTextColor(WHITE);
      mylcd->print("Maintain temp:");
      mylcd->setTextColor(RED);
      mylcd->setCursor(250, 30);
      mylcd->print(heatTemp);
      timer(); // flow += 1
    }
    // Step 3: 80C - 10p
    if (flow == 8) {
      mylcd->setTextSize(2);
      mylcd->setCursor(5, 30);
      mylcd->setTextColor(WHITE);
      mylcd->print("Maintain temp:");
      mylcd->setTextColor(RED);
      mylcd->setCursor(250, 30);
      mylcd->print(heatTemp);
      timer(); // flow += 1
    }
    // Step 4: 67 - 5p
    if (flow == 11) {
      mylcd->setTextSize(2);
      mylcd->setCursor(5, 30);
      mylcd->setTextColor(WHITE);
      mylcd->print("Maintain temp:");
      mylcd->setTextColor(RED);
      mylcd->setCursor(250, 30);
      mylcd->print(heatTemp);
      timer(); // flow += 1
    }
    if (flow == 12) {
      mylcd->setTextSize(2);
      mylcd->setCursor(5, 30);
      mylcd->setTextColor(WHITE);
      mylcd->print("Maintain temp:");
      mylcd->setTextColor(RED);
      mylcd->setCursor(250, 30);
      mylcd->print(heatTemp);
      mylcd->setTextColor(Forte_Green);
      mylcd->setTextSize(2);
      mylcd->setCursor(30, 100); //chinh toa do
      mylcd->print("Press any button");
      mylcd->setCursor(30, 140); //chinh toa do
      mylcd->print("to back to Menu");
    }
    // 1st step: Preheat to 80C degree
    if ( flow == 0 && (!digitalRead(blueButton) == HIGH || !digitalRead(redButton) == HIGH || !digitalRead(whiteButton) == HIGH) ) {
      /*
      if (testGUIMODE == true) {
        heatTemp = 35;
      }
      else {
        heatTemp = 80;
      }
      */
      heatTemp = 80;
      preheat();
      next_step();
      flow = 1;
    }
    //After preheat 80C, wait for press to start timer 10min
    if (flow == 1 && ( !digitalRead(redButton) == HIGH )) {
      heat_maintain(heatTemp);
      if (testGUIMODE == true) {
        timer_start(1);
      }
      else {
        timer_start(10);
        ////////////////////////////
        ////////////////////////////
      }
      flow = 2;
    }
    //After timer 10min done --> 2nd step: preheat 67C
    if (flow == 3 && (!digitalRead(redButton) == HIGH)) {
      heat_maintain(heatTemp);
      heatTemp = 67;
      preheat();
      flow = 4;
      next_step();
    }
    // After preheat 67C done, maintain for 60 mins
    if (flow == 4 && (!digitalRead(redButton) == HIGH )) {
      heat_maintain(heatTemp);
      if (testGUIMODE == true) {
        timer_start(1);
      }
      else {
        timer_start(60); // changed from 40 to 60
      }
      flow = 5;
    }
    // After 60 mins done --> 3rd step: preheat to 80C
    if (flow == 6 ) {
      heat_maintain(heatTemp);
      heatTemp = 80;
      preheat();
      flow = 7;
      next_step(); // press red button to continue
    }
    // After preheat 80C done, maintain for 10 mins
    if (flow == 7 && (!digitalRead(redButton) == HIGH)) {
      heat_maintain(heatTemp);
      if (testGUIMODE == true) {
        timer_start(1);
      }
      else{
        timer_start(10);
      }     
      flow = 8;
    }
    // After 10 mins done --> 4th step: Preheat to 67C
    if (flow == 9) {
      heat_maintain(heatTemp);
      heatTemp = 67;
      preheat();
      flow = 10;
      next_step(); // press red button to continue
    }
    // After preheat 67C done, maintain for 5 mins
    if (flow == 10 && (!digitalRead(redButton) == HIGH )) {
      heat_maintain(heatTemp);
      if (testGUIMODE == true) {
        timer_start(1);
      }
      else {
        timer_start(5);
      }
      flow = 11;
    }
    // Finish, now press any button to back to menu
    if (flow == 12) {
      heat_maintain(heatTemp);
      if (!digitalRead(redButton) == HIGH || !digitalRead(blueButton) == HIGH || !digitalRead(whiteButton) == HIGH) {
        flow = 20;
      } 
    }
  }
  //STEP
  else if (mode == 2) {
    if (flow == 15) {
      mylcd->fillScreen(BLACK);
      select_modestep();
    }
    if (flow != 0) {
      heat_maintain(heatTemp);
    }
    if (flow == 2 || flow == 5 || flow == 8 || flow == 9) {
      mylcd->setTextSize(2);
      mylcd->setCursor(5, 30);
      mylcd->setTextColor(WHITE);
      mylcd->print("Maintain temp:");
      mylcd->setTextColor(RED);
      mylcd->setCursor(250, 30);
      mylcd->print(heatTemp);
      timer(); // flow += 1
    }
    //Preheat to 60C degree
    if (flow == 0) {
      if (testGUIMODE == true) {
        heatTemp = 35;
      }
      else {
        heatTemp = 80 ;
      }
      preheat();
      next_step();
      flow = 1;
      //next_step();
    }
    //After preheat 60C, wait for press to start timer 30min
    if (flow == 1 && ( !digitalRead(redButton) == HIGH )) {
      heat_maintain(heatTemp);
      if (testGUIMODE == true) {
        timer_start(1);
      }
      else {
        timer_start(10);
      }
      flow = 2;
    }
    //After timer 60min done, preheat 65C
    if (flow == 3) {
      heatTemp = 67;
      preheat();
      next_step();
      flow = 4;
    }

    if (flow == 4 && ( !digitalRead(redButton) == HIGH) ) {
      if (testGUIMODE == true) {
        timer_start(1);
      }
      else {
        timer_start(60); //Change from 40 to 60 mins
      }
      flow = 5;
    }
    if (flow == 6) {
      heatTemp = 67;
      preheat();
      next_step();
      flow = 7;
    }
    if (flow == 7 && (!digitalRead(redButton) == HIGH)) {
      mylcd->fillScreen(BLACK);
      timer_start(5);
      flow = 8;
    }
    if (flow == 9) {
      heat_maintain(heatTemp);
      heatTemp = 67;
      preheat();
      flow = 10;
      next_step();
    }
    if (flow == 10) {
      heat_maintain(heatTemp);
      if (!digitalRead(redButton) == HIGH || !digitalRead(blueButton) == HIGH || !digitalRead(whiteButton) == HIGH) {
        flow = 20;
      }
    }
  }
}

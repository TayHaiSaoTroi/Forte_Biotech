#include "sensor.h"
#include "displayCLD.h"
#include "button.h"
#include "tcs.h"

sensor::sensor(/* args */)
{
    // pinMode(PIN_Sensor1, INPUT);
    // pinMode(PIN_Sensor2, INPUT);
    // pinMode(PIN_Sensor3, INPUT);
    // pinMode(PIN_Sensor4, INPUT);
    // pinMode(PIN_Sensor5, INPUT);
    pinMode(ON_LED, OUTPUT);
    digitalWrite(ON_LED, LOW);
}

sensor::~sensor()
{
}

void sensor::begin()
{
    this->clear();

    EEPROM.begin(_EEPROM_SIZE);
    EEPROM.get(0, this->valueCalibMin);
    EEPROM.get(2, this->valueCalibMax);
    dbg_sensor("calib_Min: %d", this->valueCalibMin);
    dbg_sensor("calib_Max: %d", this->valueCalibMax);

    Wire.begin(32, 33);
    rgb_sensor.begin();
}

void sensor::read_Sensor(uint8_t num)
{
    for (int i = 0; i < numSample; i++)
    {
        this->Data_Sensor1[i] = analogRead(num);
        // this->Data_Sensor1[i] = analogReadMilliVolts(sensor)
    }
}


void sensor::read_Allsensor()
{
    digitalWrite(ON_LED, HIGH);
    delay(620);
    rgb_sensor.getData();
    rgb_sensor.getData();
    for (int i = 0; i < numSample; i++)
    {
        rgb_sensor.getData();
        this->Data_Sensor1[i] = rgb_sensor.lux;
    }
    digitalWrite(ON_LED, LOW);
}

uint16_t sensor::calib_SensorLight()
{
    digitalWrite(ON_LED, HIGH);
    delay(620);

    float sumCalib = 0;
    rgb_sensor.getData();
    rgb_sensor.getData();

    for (int i = 0; i < 5; i++)
    {
        rgb_sensor.getData();
        dbg_sensor("sensor_calib %d: %f", i, rgb_sensor.lux);
        sumCalib += rgb_sensor.lux;
    }
    digitalWrite(ON_LED, LOW);

    float valueCalib = (float)(sumCalib / 5.0) * 1000.0;
    dbg_sensor("TB sensor_calib: %f", valueCalib);
    return (uint16_t)valueCalib;
}

void sensor::format_CalibSensor()
{
    for (int i = 0; i < numSensor; i++)
    {
        EEPROM.put(i * 2, (int16_t)0);
    }
    EEPROM.commit();
}

void sensor::handle_Sensor()
{
    this->sum_Sensor[0] = 0;
    for (int k = 0; k < numSample; k++)
    {
        // dbg_sensor("sensor %f", this->Data_Sensor1[k]);
        this->sum_Sensor[0] += (float)this->Data_Sensor1[k];
        // dbg_sensor("sum sensor%d: %f", k, this->sum_Sensor[0]);
    }

    for (int i = 0; i < numSensor; i++)
    {
        // float value_sensor = round((float)(sum_Sensor[i] / (float)((numSample - 1) * 1.0)));
        // result_Sensor[i][_displayCLD.couter - 1] = round((float)(sum_Sensor[i] / (float)((numSample - 1) * 1.0)));

        uint16_t value_sensor = round((float)(sum_Sensor[i] / (float)((numSample) * 1.0)) * 1000.0);

        if (value_sensor < this->valueCalibMin)
            this->result_Sensor[i][_displayCLD.couter - 1] = 0;
        else if (value_sensor > this->valueCalibMax)
            this->result_Sensor[i][_displayCLD.couter - 1] = valueMAXsensor;
        else
            this->result_Sensor[i][_displayCLD.couter - 1] = map(value_sensor, this->valueCalibMin, this->valueCalibMax, 0, valueMAXsensor);
        // result_Sensor[i][_displayCLD.couter - 1] = map(value_sensor, 0.0 , 50.0, 0.0 , 4096.0);
        dbg_sensor("sensor_value: %d", value_sensor);
        //   dbg_sensor("sensor_map: %f", data);
        dbg_sensor("sensor: %d", this->result_Sensor[i][_displayCLD.couter - 1]);
        // if (result_Sensor[i][_displayCLD.couter - 1] > 0)
        //     result_Sensor[i][_displayCLD.couter - 1] += Calib_Sensor[i];
        // sum_Sensor[i] = 0;
    }
}

void sensor::Average_Result()
{
    for (int i = 0; i < numSensor; i++)
    {
        for (int y = 0; y < 3; y++)
        {
            AverageResult[i] += result_Sensor[i][y];
        }
    }
    for (int i = 0; i < 3; i++)
        AverageResult[i] = AverageResult[i] / 3;
}

void sensor::clear()
{
    for (int i = 0; i < numSensor; i++)
    {
        sum_Sensor[i] = 0;
        AverageResult[i] = 0;
        sensor_kalman[i] = 0;
        result_Sensor[i][0] = 0;
        result_Sensor[i][1] = 0;
        result_Sensor[i][2] = 0;
    }
}
void sensor::loop()
{
    if (flagReadSensor)
    {
        this->read_Allsensor();
        this->handle_Sensor();

        _displayCLD.type_infor = escreenResult;
        _displayCLD.changeScreen = true;
        flagReadSensor = false;
    }

    else if (flagCalibSensor)
    {
        flagCalibSensor = false;

        if (this->typecalib == 0)
        {
            _displayCLD.waiting_Calib();
            this->valueCalibMax = this->calib_SensorLight();
            this->typecalib = 1;
            _displayCLD.type_infor = ecalibSensor;
            _displayCLD.changeScreen = true;
        }
        else if (this->typecalib == 1)
        {
            _displayCLD.waiting_Calib();
            this->valueCalibMin = this->calib_SensorLight();
            _sensor.typecalib = 0;
            EEPROM.put(0, this->valueCalibMin);
            EEPROM.put(2, this->valueCalibMax);
            EEPROM.commit();
            _displayCLD.screen_Calib_Complete();
            ESP.restart();
        }
        else
        {
            this->typecalib = 0;
        }
    }

    else if (flagformatCalib)
    {
        this->format_CalibSensor();
        flagformatCalib = false;
        _displayCLD.type_infor = escreenStart;
        _displayCLD.changeScreen = true;
    }
}

sensor _sensor;

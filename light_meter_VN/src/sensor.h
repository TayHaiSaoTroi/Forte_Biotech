#ifndef _SENNSOR_H
#define _SENNSOR_H

#include "define.h"
#define numSensor 1
#define numSampling 3
#define numSample 3

class sensor
{
private:

public:
    sensor(/* args */);
    ~sensor();

    void begin();
    void read_Sensor(uint8_t num);
    void read_Allsensor();
    void handle_Sensor();
    uint16_t calib_SensorLight();
    void format_CalibSensor();
    void Average_Result();
    void clear();
    void loop();

    float Data_Sensor1[numSample];
    // uint16_t Data_Sensor2[numSample];
    // uint16_t Data_Sensor3[numSample];
    // uint16_t Data_Sensor4[numSample];
    // uint16_t Data_Sensor5[numSample];

    uint8_t typecalib = 0;
    uint16_t Calib_Sensor[2];
    uint16_t valueCalibMax;
    uint16_t valueCalibMin;

    uint16_t result_Sensor[numSensor][numSampling];
    uint16_t AverageResult[numSensor];
    float sum_Sensor[numSensor];
    uint16_t sensor_kalman[numSensor];

    bool flagReadSensor = false;
    bool flagCalibSensor = false;
    bool flagformatCalib = false;
    bool flagback = false;
};

extern sensor _sensor;
#endif

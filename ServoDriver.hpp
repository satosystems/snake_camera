#ifndef ___ServoDriver
#define ___ServoDriver

#include <stdint.h>

// Ada 16ch PWMサーボドライバクラス

enum {
    PCA9685_SUBADR1 = 0x2,
    PCA9685_SUBADR2 = 0x3,
    PCA9685_SUBADR3 = 0x4,
};

const uint8_t PCA9685_MODE1 = 0x0;
const uint8_t PCA9685_PRESCALE = 0xFE;

const uint8_t LED0_ON_L = 0x6;
const uint8_t LED0_ON_H = 0x7;
const uint8_t LED0_OFF_L = 0x8;
const uint8_t LED0_OFF_H = 0x9;

const uint8_t ALLLED_ON_L = 0xFA;
const uint8_t ALLLED_ON_H = 0xFB;
const uint8_t ALLLED_OFF_L = 0xFC;
const uint8_t ALLLED_OFF_H = 0xFD;

const double SERVO_CONTROL_FREQUENCY = 60;
const double SERVO_CENTER_PULSE_WIDTH_US = 1520;
const double SERVO_CENTER_PULSE_WIDTH_US_FUTABA_OLD = 1310;
const double SERVO_RANGE_PULSE_WIDTH_US = 1600;

class ServoDriver {
public:
    ServoDriver();
    void reset(void);
    void setPWMFreq(float frea);
    void setPWM(uint8_t srvNo, uint16_t onTime, uint16_t offTime);
    void setServoPulse(uint8_t ch, double pulseWidth_us);
    uint8_t read8(uint8_t addr);
    void write8(uint8_t addr, uint8_t d);

private:
    uint8_t _i2cAddr;
    int _i2c;
};

#endif


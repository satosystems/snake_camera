// http://junkroom2cyberrobotics.blogspot.jp/2013/06/raspberry-pi-adafruit-i2c-16-channel.html
//
// Ada I2C 16chサーボドライバ

#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "ServoDriver.hpp"

ServoDriver::ServoDriver() {
    const int driverAddress = 0x40;
    char fileName[16];

    for (int i = 0; i <= 1; i++) {
        sprintf(fileName, "/dev/i2c-%d", i);
        if ((_i2c = open(fileName, O_RDWR)) < 0) {
            continue;
        }
        if (ioctl(_i2c, I2C_SLAVE, driverAddress) < 0) {
            fprintf(stderr, "Unable to get bus access to talk to slave\n");
            exit(1);
        } else {
            break;
        }
    }
    if (_i2c < 0) {
        fprintf(stderr, "Unable to found I2C device\n");
        exit(1);
    }
}

void ServoDriver::write8(uint8_t addr, uint8_t d) {
    uint8_t sendData[2];

    sendData[0] = addr;
    sendData[1] = d;
    if (write(_i2c, sendData, 2) != 2) {
        fprintf(stderr, "Faild to send i2c\n");
    }
}

uint8_t ServoDriver::read8(uint8_t addr) {
    uint8_t sendData;
    uint8_t readData;

    sendData = addr;
    if (write(_i2c, &sendData, 1) != 1) {
        fprintf(stderr, "Failed to send i2c @read\n");
    } else if (read(_i2c, &readData, 1) != 1) {
        fprintf(stderr, "Failed to read i2c\n");
    }

    return readData;
}

void ServoDriver::reset(void) {
    write8(PCA9685_MODE1, 0x0);
}

void ServoDriver::setPWMFreq(float freq) {
    float prescaleval = 25000000;

    prescaleval /= 4096;
    prescaleval /= freq;
    prescaleval -= 1;
    fprintf(stderr, "Estimated pre-scale: %f\n", prescaleval);

    uint8_t prescale = floor(prescaleval + 0.5);
    fprintf(stderr, "Final pre-scale: %d\n", prescale);

    uint8_t oldmode = read8(PCA9685_MODE1);
    uint8_t newmode = (oldmode & 0x7F) | 0x10;
    write8(PCA9685_MODE1, newmode);
    write8(PCA9685_PRESCALE, prescale);
    write8(PCA9685_MODE1, oldmode);
    sleep(5);
    write8(PCA9685_MODE1, oldmode | 0xa1);
}

void ServoDriver::setPWM(uint8_t srvNo, uint16_t onTime, uint16_t offTime) {
    uint8_t sendData[5];

    sendData[0] = LED0_ON_L + 4 * srvNo;
    sendData[1] = (uint8_t) (0x00ff & onTime);
    sendData[2] = (uint8_t) ((0xff00 & onTime) >> 8);
    sendData[3] = (uint8_t) (0x00ff & offTime);
    sendData[4] = (uint8_t) ((0xff00 & offTime) >> 8);

    if (write(_i2c, sendData, 5) != 5) {
        fprintf(stderr, "Faild to send i2c @setPWM\n");
    }
}

void ServoDriver::setServoPulse(uint8_t ch, double pulseWidth_us) {
    double pulseLength;
    double pulseWidth;

    // 1秒=1000000usを60Hzで割ってパルス長を算出。
    pulseLength = 1000000 / SERVO_CONTROL_FREQUENCY;
    // 12bit(2^12=4096)分解能相当へ。1分解能当たりの時間算出。
    pulseLength /= 4096;
    // PWMのパルス設定値を算出。
    pulseWidth = pulseWidth_us / pulseLength;

    // PWM値設定。
    //  setPWM(channel, on_timing, off_timing)
    //  channelで指定したチャネルのPWM出力のon(0→1）になるタイミングと
    //  off(1→0)になるタイミングを0～4095で設定する。
    setPWM(ch, 0, pulseWidth);
}


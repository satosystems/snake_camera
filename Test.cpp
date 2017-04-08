// http://junkroom2cyberrobotics.blogspot.jp/2013/06/raspberry-pi-adafruit-i2c-16-channel.html
//
// Ada I2C 16chサーボドライバ

#include <stdio.h>
#include <unistd.h>
#include "ServoDriver.hpp"

static ServoDriver servo;

int main(int argc, char *argv[]) {

    servo.reset();

    usleep(100000);

    // サーボ制御パルス周波数の設定。
    servo.setPWMFreq(SERVO_CONTROL_FREQUENCY);

    // サーボをセンタ位置へ。
    servo.setServoPulse(0, SERVO_CENTER_PULSE_WIDTH_US);
    servo.setServoPulse(1, SERVO_CENTER_PULSE_WIDTH_US);

    sleep(1);

    for (int i = 0; i < 3; i++) {

        servo.setServoPulse(0, (SERVO_CENTER_PULSE_WIDTH_US + SERVO_RANGE_PULSE_WIDTH_US / 3));

        usleep(500000);

        servo.setServoPulse(1, (SERVO_CENTER_PULSE_WIDTH_US - SERVO_RANGE_PULSE_WIDTH_US / 3));

        usleep(500000);

        servo.setServoPulse(0, (SERVO_CENTER_PULSE_WIDTH_US - SERVO_RANGE_PULSE_WIDTH_US / 3));

        usleep(500000);

        servo.setServoPulse(1, SERVO_CENTER_PULSE_WIDTH_US + SERVO_RANGE_PULSE_WIDTH_US / 3);

        usleep(500000);
    }

    servo.setServoPulse(0, SERVO_CENTER_PULSE_WIDTH_US);
    servo.setServoPulse(1, SERVO_CENTER_PULSE_WIDTH_US);

    return 0;
}


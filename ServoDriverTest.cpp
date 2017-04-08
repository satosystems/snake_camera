// http://junkroom2cyberrobotics.blogspot.jp/2013/06/raspberry-pi-adafruit-i2c-16-channel.html
//
// Ada I2C 16chサーボドライバ

#include <stdio.h>
#include <unistd.h>
#include "ServoDriver.hpp"

static ServoDriver servo;

double max(double a, double b) {
    if (a > b) return a;
    else return b;
}

double min(double a, double b) {
    if (a < b) return a;
    else return b;
}

double map(double angle) {
    double r = SERVO_RANGE_PULSE_WIDTH_US / 180;
    return r * angle;
}

/**
 * カメラを左右に向ける。
 *
 * @param [in] angle 角度（-90 で右、90 で左）
 */
void turnHorizontal(double angle) {
    double safeAngle = min(90, max(angle, -90));
    double r = map(safeAngle);
    servo.setServoPulse(0, (SERVO_CENTER_PULSE_WIDTH_US + r));
}

/**
 * カメラを上下に向ける。
 *
 * @param [in] angle 角度（-40 で上、90 で下）
 */
void turnVertical(double angle) {
    double safeAngle = min(90, max(angle, -40));
    double r = map(safeAngle);
    servo.setServoPulse(1, (SERVO_CENTER_PULSE_WIDTH_US + r));
}

int main(int argc, char *argv[]) {

    servo.reset();

    usleep(100000);

    // サーボ制御パルス周波数の設定。
    servo.setPWMFreq(SERVO_CONTROL_FREQUENCY);

    // サーボをセンタ位置へ。
//    servo.setServoPulse(0, SERVO_CENTER_PULSE_WIDTH_US);
//    servo.setServoPulse(1, SERVO_CENTER_PULSE_WIDTH_US);

    sleep(1);

//    servo.setServoPulse(1, (SERVO_CENTER_PULSE_WIDTH_US - SERVO_RANGE_PULSE_WIDTH_US / 3));  // カメラ下向き
//    servo.setServoPulse(0, (SERVO_CENTER_PULSE_WIDTH_US - SERVO_RANGE_PULSE_WIDTH_US / 3));  // カメラ向かって左

/*
    for (int i = 0; i < 3; i++) {
        turnVertical(-90);
        usleep(500000);
        turnVertical(90);
        usleep(500000);
        turnVertical(0);
        usleep(500000);
    }
*/

    turnHorizontal(0);
    turnVertical(0);

/*
    for (int i = -40; i != -90; i--) {
        turnVertical(i);
        printf("%d\n", i);
        usleep(1000000);
    }
*/

    servo.reset();

    if (1) return 0;
    else return 0;

    for (int i = 0; i < 3; i++) {

//        servo.setServoPulse(0, (SERVO_CENTER_PULSE_WIDTH_US + SERVO_RANGE_PULSE_WIDTH_US / 3));

        usleep(500000);

        servo.setServoPulse(1, (SERVO_CENTER_PULSE_WIDTH_US - SERVO_RANGE_PULSE_WIDTH_US / 3));

        usleep(500000);

//        servo.setServoPulse(0, (SERVO_CENTER_PULSE_WIDTH_US - SERVO_RANGE_PULSE_WIDTH_US / 3));

        usleep(500000);

        servo.setServoPulse(1, SERVO_CENTER_PULSE_WIDTH_US + SERVO_RANGE_PULSE_WIDTH_US / 3);

        usleep(500000);
    }

//    servo.setServoPulse(0, SERVO_CENTER_PULSE_WIDTH_US);
    servo.setServoPulse(1, SERVO_CENTER_PULSE_WIDTH_US - SERVO_RANGE_PULSE_WIDTH_US / 3);

    return 0;
}


#include <cv.h>
#include <highgui.h>
#include <unistd.h>
#include "ServoDriver.hpp"

static ServoDriver servo;

static double max(double a, double b) {
    if (a > b) return a;
    else return b;
}

static double min(double a, double b) {
    if (a < b) return a;
    else return b;
}

static double toPulse(double angle) {
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
    double r = toPulse(safeAngle);
    servo.setServoPulse(0, (SERVO_CENTER_PULSE_WIDTH_US + r));
}

/**
 * カメラを上下に向ける。
 *
 * @param [in] angle 角度（-40 で上、90 で下）
 */
void turnVertical(double angle) {
    double safeAngle = min(90, max(angle, -40));
    double r = toPulse(safeAngle);
    servo.setServoPulse(1, (SERVO_CENTER_PULSE_WIDTH_US + r));
}

int main(int argc, char *argv[]) {
    servo.reset();
    usleep(100000);
    // サーボ制御パルス周波数の設定。
    servo.setPWMFreq(SERVO_CONTROL_FREQUENCY);
    usleep(1000000);
    turnHorizontal(0);
    turnVertical(0);
    char cascade[] = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
    double w = 160;
    double h = 120;
    cvNamedWindow("Snake Camera", CV_WINDOW_AUTOSIZE);
    CvCapture *capture = NULL;
    capture = cvCreateCameraCapture(0);
    // キャプチャサイズを設定する．
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, w);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, h);

    IplImage *frame;
    // 正面顔検出器の読み込み
    CvHaarClassifierCascade *cvHCC = (CvHaarClassifierCascade *) cvLoad(cascade);
    // 検出に必要なメモリストレージを用意する
    CvMemStorage *cvMStr = cvCreateMemStorage(0);
    // 検出情報を受け取るためのシーケンスを用意する
    CvSeq *face;
    double angleX = 0;
    double angleY = 0;
    for (;;) {
        frame = cvQueryFrame(capture);
        if (!frame) break;
        // 画像中から検出対象の情報を取得する
        face = cvHaarDetectObjects(frame, cvHCC, cvMStr);
        for (int i = 0; i < face->total; i++) {
            // 検出情報から顔の位置情報を取得
            CvRect *faceRect = (CvRect *) cvGetSeqElem(face, i);
            // 取得した顔の位置情報に基づき、矩形描画を行う
            cvRectangle(frame,
                        cvPoint(faceRect->x, faceRect->y),
                        cvPoint(faceRect->x + faceRect->width, faceRect->y + faceRect->height),
                        CV_RGB(255, 0 ,0),
                        2,
                        CV_AA);

            if (i == 0) {
                // 顔を中央に移動
                double xx = faceRect->x + faceRect->width / 2;
                double yy = faceRect->y + faceRect->height / 2;
                double ax = -(xx - w / 2) / 2;
                double ay = (yy - h / 2) / 2;
                printf("x:%3d y:%3d w:%3d h:%3d xx:%3.2lf yy:%3.2lf ax:%3.2lf ay:%3.2lf\n", faceRect->x, faceRect->y, faceRect->width, faceRect->height, xx, yy, ax, ay);

                if (ax > 45 || ax < -45) angleX += ax / 2;
                else if (ax > 30 || ax < -30) angleX += ax / 3;
                else if (ax > 20 || ax < -20) angleX += ax / 4;
                else if (ax > 10 || ax < -10) angleX += ax / 5;
                else if (ax > 5 || ax < -5) angleX += ax / 10;
                if (ay > 45 || ay < -45) angleY += ay / 2;
                else if (ay > 30 || ay < -30) angleY += ay / 3;
                else if (ay > 20 || ay < -20) angleY += ay / 4;
                else if (ay > 10 || ay < -10) angleY += ay / 5;
                else if (ay > 5 || ay < -5) angleY += ay / 10;

                turnHorizontal(angleX);
                turnVertical(angleY);
            }
        }
        cvShowImage("Snake Camera", frame);
        char c = cvWaitKey(33);
        if (c == 27) break;
    }
    // 用意したメモリストレージを解放
    cvReleaseMemStorage(&cvMStr);
    // カスケード識別器の解放
    cvReleaseHaarClassifierCascade(&cvHCC);
    cvReleaseCapture(&capture);
    cvDestroyWindow("Snake Camera");
}


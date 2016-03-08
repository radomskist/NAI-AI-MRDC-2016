/*#include <iostream>
#include <string>
#include <vector>

#include <chrono>
#include <thread>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

// #define DISABLE_RED
// #define DISABLE_YELLOW
// #define DISABLE_GREEN
// #define DISABLE_WHITE

const int MIN_RADIUS = 75;

const int CRED_LOW = 0;
const int CRED_HIGH = 1;
const int CYELLOW = 2;
const int CGREEN = 3;
const int CWHITE = 4;

const pair <int, int> fail(0, 0);
const pair <int, int> redLow(0, 10);
const pair <int, int> redHigh(161, 179);
const pair <int, int> yellow(21, 33);
const pair <int, int> green(34, 80);
const pair <int, int> white(0, 255);

vector<pair<int, int>> colours;

string getColor(int c)
{
    if (c == CRED_LOW) {
        return "RED";
    }
    else if (c == CRED_HIGH) {
        return "RED";
    }
    else if (c == CYELLOW) {
        return "YELLOW";
    }
    else if (c == CGREEN) {
        return "GREEN";
    }
    else if (c == CWHITE) {
        return "WHITE";
    }
    else {
        cout << "LOL" << endl;
        return "FAILED";
    }
}

pair<int, int> getColorHuePair(int c) {
    if (c == CRED_LOW) {
        return redLow;
    }
    else if (c == CRED_HIGH) {
        return redHigh;
    }
    else if (c == CYELLOW) {
        return yellow;
    }
    else if (c == CGREEN) {
        return green;
    }
    else if (c == CWHITE) {
        return white;
    }
    else {
        cout << "FAILED" << endl;
        return fail;
    }
}

int main()
{
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Cannot open the web cam" << endl;
        exit(-1);
    }

    int iLowS = 85;
    int iHighS = 255;

    int iLowV = 35;
    int iHighV = 255;

    int iWLS = 0;
    int iWHS = 255;
    int iWLL = 115;
    int iWHL = 255;

    // namedWindow("Control", CV_WINDOW_AUTOSIZE);
    //
    // cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    // cvCreateTrackbar("HighS", "Control", &iHighS, 255);
    //
    // cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    // cvCreateTrackbar("HighV", "Control", &iHighV, 255);

    // namedWindow("ControlWhite", CV_WINDOW_AUTOSIZE);
    //
    // cvCreateTrackbar("LowS", "ControlWhite", &iWLS, 255); //Saturation (0 - 255)
    // cvCreateTrackbar("HighS", "ControlWhite", &iWHS, 255);
    //
    // cvCreateTrackbar("LowL", "ControlWhite", &iWLL, 255); //Value (0 - 255)
    // cvCreateTrackbar("HighL", "ControlWhite", &iWHL, 255);

    while (true) {
        bool fallthrough = false;

        Mat original;
        Mat bgr;
        Mat hsv;
        Mat hls;
        Mat hue;

        Mat red_hue;
        Mat yellow_hue;
        Mat green_hue;
        Mat white_hue;

        bool succ = cap.read(original);
        if (!succ) {
            cout << "failed to read frame from video stream" << endl;
            break;
        }
        bgr = original.clone();
        medianBlur(bgr, bgr, 3);
        
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        cvtColor(bgr, hls, COLOR_BGR2HLS);

#ifndef DISABLE_RED
        Mat lower_red_hue_range;
        Mat upper_red_hue_range;
        inRange(hsv, Scalar(getColorHuePair(CRED_LOW).first, iLowS, iLowV), Scalar(getColorHuePair(CRED_LOW).second, iHighS, iHighV), lower_red_hue_range);
        inRange(hsv, Scalar(getColorHuePair(CRED_HIGH).first, iLowS, iLowV), Scalar(getColorHuePair(CRED_HIGH).second, iHighS, iHighV), upper_red_hue_range);
        addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue);
        GaussianBlur(red_hue, red_hue, Size(9, 9), 2, 2);
        vector<Vec3f> red_circles;
        HoughCircles(red_hue, red_circles, CV_HOUGH_GRADIENT, 1, red_hue.rows / 8, 100, 40, 0, 0);

        for (size_t current_circle = 0; current_circle < red_circles.size(); ++current_circle) {
            Point center(round(red_circles[current_circle][0]), round(red_circles[current_circle][1]));
            int radius = round(red_circles[current_circle][2]);

            if (radius > MIN_RADIUS) {
                fallthrough = true;
                circle(original, center, radius, Scalar(0, 255, 0), 5);
                cout << "RED" << radius << endl;
            }
        }
        // namedWindow("Red Threshold", WINDOW_AUTOSIZE);
        // imshow("Red Threshold", red_hue);
#endif
#ifndef DISABLE_YELLOW
        if (!fallthrough) {
            inRange(hsv, Scalar(getColorHuePair(CYELLOW).first, iLowS, iLowV), Scalar(getColorHuePair(CYELLOW).second, iHighS, iHighV), yellow_hue);
            GaussianBlur(yellow_hue, yellow_hue, Size(9, 9), 2, 2);
            vector<Vec3f> yellow_circles;
            HoughCircles(yellow_hue, yellow_circles, CV_HOUGH_GRADIENT, 1, yellow_hue.rows / 8, 100, 40, 0, 0);

            for (size_t current_circle = 0; current_circle < yellow_circles.size(); ++current_circle) {
                Point center(round(yellow_circles[current_circle][0]), round(yellow_circles[current_circle][1]));
                int radius = round(yellow_circles[current_circle][2]);

                if (radius > MIN_RADIUS) {
                    fallthrough = true;
                    circle(original, center, radius, Scalar(0, 255, 0), 5);
                    cout << "YELLOW" << radius << endl;
                }
            }
            // namedWindow("Yellow Threshold", WINDOW_AUTOSIZE);
            // imshow("Yellow Threshold", yellow_hue);
        }
#endif
#ifndef DISABLE_GREEN
        if (!fallthrough) {
            inRange(hsv, Scalar(getColorHuePair(CGREEN).first, iLowS, iLowV), Scalar(getColorHuePair(CGREEN).second, iHighS, iHighV), green_hue);
            GaussianBlur(green_hue, green_hue, Size(9, 9), 2, 2);
            vector<Vec3f> green_circles;
            HoughCircles(green_hue, green_circles, CV_HOUGH_GRADIENT, 1, green_hue.rows / 8, 100, 40, 0, 0);

            for (size_t current_circle = 0; current_circle < green_circles.size(); ++current_circle) {
                Point center(round(green_circles[current_circle][0]), round(green_circles[current_circle][1]));
                int radius = round(green_circles[current_circle][2]);

                if (radius > MIN_RADIUS) {
                    fallthrough = true;
                    circle(original, center, radius, Scalar(0, 255, 0), 5);
                    cout << "GREEN" << radius << endl;
                }
            }
            // namedWindow("Green Threshold", WINDOW_AUTOSIZE);
            // imshow("Green Threshold", green_hue);
        }
#endif
#ifndef DISABLE_WHITE
        if (!fallthrough) {
            inRange(hls, Scalar(getColorHuePair(CWHITE).first, iWLL, iWLS), Scalar(getColorHuePair(CWHITE).second, iWHL, iWHS), white_hue);
            GaussianBlur(white_hue, white_hue, Size(9, 9), 2, 2);
            vector<Vec3f> white_circles;
            HoughCircles(white_hue, white_circles, CV_HOUGH_GRADIENT, 1, white_hue.rows / 8, 100, 40, 0, 0);

            for (size_t current_circle = 0; current_circle < white_circles.size(); ++current_circle) {
                Point center(round(white_circles[current_circle][0]), round(white_circles[current_circle][1]));
                int radius = round(white_circles[current_circle][2]);

                if (radius > MIN_RADIUS) {
                    circle(original, center, radius, Scalar(0, 255, 0), 5);
                    cout << "WHITE" << radius << endl;
                }
            }
            // namedWindow("White Threshold", WINDOW_AUTOSIZE);
            // imshow("White Threshold", white_hue);
        }
#endif

        namedWindow("EyeCam Circle Detection", WINDOW_AUTOSIZE);
        imshow("EyeCam Circle Detection", original);

        if (waitKey(1) == 27) {
            cout << "Exiting" << endl;
            break;
        }

        // this_thread::sleep_for(chrono::milliseconds(500));
    }
    return 0;
}*/

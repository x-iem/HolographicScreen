#ifndef TRACKER_H
#define TRACKER_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <utils.h>

class FaceTracker {
    public:
        FaceTracker(unsigned int avg_number);
        ~FaceTracker();
        cv::Mat getFrame();
        enum status {OK=0, CAM_OPEN_FAILED, CLASSIFIER_LOAD_FAILED, EMPTY_FRAME};
        std::vector<float> retrieveFacePos();
        int getWidth();
        int getHeight();
    private:
        enum status internalState = status::OK;
        cv::VideoCapture cap;
        cv::Mat frame;
        int loadClassifiers();
        int openCam();
        void closeCam();
        cv::CascadeClassifier faceCascade;
        MovingAverage averageX;
        MovingAverage averageY;
};
#endif

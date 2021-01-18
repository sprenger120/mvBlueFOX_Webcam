#pragma once
#include <string>
#include <thread>
#include "FileHandleWrapper.hpp"
#include "Undistortion.hpp"
#include <linux/videodev2.h>
#include <chrono>
#include "FaceDetector.hpp"

using namespace std::chrono;

namespace webcam {

class Webcam {
public:
    Webcam(const std::string& device, unsigned int frameWidth, unsigned int frameHeight);

    Webcam(const Webcam &) = delete;
    Webcam(Webcam &&) = delete;
    Webcam &operator=(const Webcam &) = delete;
    Webcam &operator=(Webcam &&) = delete;

    void publish(int imageWidth , int imageHeight, int channelCount, void * rawData);

private:
    unsigned int _frameWidth;
    unsigned int _frameHeight;
    FileHandleWrapper _dev_fd;
    v4l2_format _videoFormat;
    steady_clock::time_point _lastFrame;
    Undistortion _undistortion;
    FaceDetector _faceDetector;

    double faceX = 0;
    double faceY = 0;
    double faceSize = 250;
    cv::Point lastFaceDetectionResult;
    bool firstDetection{false};
    double avg(double value, double newSample);

    static constexpr int detectionWaitFrames{3};
    int frameCounter{0};

    //std::thread _writeThread;
    //bool _threadShouldRun{true};
    //void threadMain();
};

}

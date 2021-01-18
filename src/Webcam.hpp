#pragma once
#include <string>
#include <thread>
#include "FileHandleWrapper.hpp"
#include <linux/videodev2.h>
#include <chrono>

using namespace std::chrono;

namespace webcam {

class Webcam {
public:
    Webcam(const std::string& device, unsigned int frameWidth, unsigned int frameHeight);
    ~Webcam();

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

    //std::thread _writeThread;
    //bool _threadShouldRun{true};
    //void threadMain();
};

}

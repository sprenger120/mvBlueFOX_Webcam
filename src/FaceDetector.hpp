#pragma once
#include <opencv2/opencv.hpp>

namespace webcam{


class FaceDetector {
public:
    FaceDetector();

    cv::Point detectFace(cv::Mat&);
private:
    cv::CascadeClassifier _face_cascade;
};


}
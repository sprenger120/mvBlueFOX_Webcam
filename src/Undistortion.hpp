#pragma once
#include <opencv2/opencv.hpp>
#include <array>

namespace webcam {

class Undistortion {
public:
    Undistortion();

    void undistortImage(cv::Mat& _src, cv::Mat& _dst) const;

    static constexpr int cameraWidth = 752;
    static constexpr int cameraHeight = 480;
private:
    std::array<double,9> _rawInstrinsicMatrix;
    std::array<double,9> _rawRectificationMatrix;
    std::array<double,12> _rawProjectionMatrix;
    // plumb_bob distortion
    std::array<double,5> _rawDistortionMatrix;

    cv::Mat _intrinsicMatrix;
    cv::Mat _rectificationMatrix;
    cv::Mat _projectionMatrix;
    cv::Mat _distortionMatrix;

    cv::Mat _undistortionMap1;
    cv::Mat _undistortionMap2;

};

}
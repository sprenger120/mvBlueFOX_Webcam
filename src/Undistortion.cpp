#include "Undistortion.hpp"

namespace webcam {

// Values obtained from running an non-opensourced calibration
// script for the lens I'm currently using
// If these parameter do not match your lense by chance (which they probably wont)
// just comment out any undistortion stuff
Undistortion::Undistortion() :
    _rawInstrinsicMatrix{{
                             425.399269, 0.000000, 381.599384,
                             0.000000, 425.446103, 231.300097,
                             0.000000, 0.000000, 1.000000
                         }},
    _rawRectificationMatrix{{
                                1, 0, 0,
                                0, 1, 0,
                                0, 0, 1
                            }},
    _rawProjectionMatrix{{
                             325.399269, 0.000000, 381.599384, 0.000000,
                             0.000000, 325.446103, 231.300097, 0.000000,
                             0.000000, 0.000000, 1.000000, 0.000000
                         }},
    _rawDistortionMatrix{{
                             -0.269296, 0.055, -0.000566, 0.001512, 0.000000
                         }},

    _intrinsicMatrix(3, 3, CV_64F, reinterpret_cast<void *>(_rawInstrinsicMatrix.data())),
    _rectificationMatrix(3, 3, CV_64F, reinterpret_cast<void *>(_rawRectificationMatrix.data())),
    _projectionMatrix(3, 4, CV_64F, reinterpret_cast<void *>(_rawProjectionMatrix.data())),
    _distortionMatrix(1, 5, CV_64F, reinterpret_cast<void *>(_rawDistortionMatrix.data())) {
    assert(_rawInstrinsicMatrix.size() == 3 * 3);
    assert(_rawRectificationMatrix.size() == 3 * 3);
    assert(_rawProjectionMatrix.size() == 3 * 4);
    assert(_rawDistortionMatrix.size() == 1 * 5);

    cv::initUndistortRectifyMap(_intrinsicMatrix, _distortionMatrix, _rectificationMatrix, _projectionMatrix,
                                cv::Size(cameraWidth, cameraHeight), CV_32FC1, _undistortionMap1, _undistortionMap2);

}
void Undistortion::undistortImage(cv::Mat &_src, cv::Mat &_dst) const {
    cv::UMat src = _src.getUMat(cv::ACCESS_READ);
    cv::UMat dst = _dst.getUMat(cv::ACCESS_WRITE);

    cv::remap(src, dst, _undistortionMap1, _undistortionMap2, cv::INTER_LINEAR);
}
}
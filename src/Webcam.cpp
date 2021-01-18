#include "Webcam.hpp"
#include <stdexcept>
#include <unistd.h>
#include <sys/ioctl.h>
#include <array>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <libyuv.h>
#include <sstream>

namespace webcam {

Webcam::Webcam(const std::string &device, unsigned int frameWidth, unsigned int frameHeight)
    : _frameWidth(frameWidth), _frameHeight(frameHeight), _dev_fd(device, O_RDWR), _lastFrame(steady_clock::now()) {
    if (frameHeight == 0 || frameWidth == 0)
    {
        throw std::runtime_error("Illegal frame heigh/width");
    }


    /*v4l2_capability vid_caps;
    if (ioctl(_dev_fd.get(), VIDIOC_QUERYCAP, &vid_caps) < 0) {
        throw std::runtime_error("cannot query video device");
    }*/

    _videoFormat.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    if (ioctl(_dev_fd.get(), VIDIOC_G_FMT, &_videoFormat) < 0)
    {
        throw std::runtime_error("cannot setup video device");
    }
    _videoFormat.fmt.pix.width = frameWidth;
    _videoFormat.fmt.pix.height = frameHeight;
    _videoFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    _videoFormat.fmt.pix.sizeimage = 2 * frameWidth * frameHeight;
    _videoFormat.fmt.pix.field = V4L2_FIELD_NONE;
    if (ioctl(_dev_fd.get(), VIDIOC_S_FMT, &_videoFormat) < 0)
    {
        throw std::runtime_error("cannot setup video device");
    }
}

void Webcam::publish(int imageWidth, int imageHeight, int channelCount, void *rawData) {
    if (imageWidth != Undistortion::cameraWidth || imageHeight != Undistortion::cameraHeight
        || (!(channelCount == 1 || channelCount == 3)) || rawData == nullptr)
    {
        throw std::invalid_argument("illegal input");
    }

    // incoming picture to RGBA
    cv::Mat bgraImgDistorted;
    if (channelCount == 1)
    {
        cv::Mat grayImage(imageHeight, imageWidth, CV_8UC1, rawData);
        cv::cvtColor(grayImage, bgraImgDistorted, cv::COLOR_GRAY2BGRA);
    }
    else
    {
        cv::Mat bgrImg(imageHeight, imageWidth, CV_8UC3, rawData);
        cv::cvtColor(bgrImg, bgraImgDistorted, cv::COLOR_RGB2BGRA);
    }

    // undistort
    cv::Mat bgraImg(bgraImgDistorted.size(), bgraImgDistorted.type());
    _undistortion.undistortImage(bgraImgDistorted, bgraImg);

    //cv::imshow("aa", bgraImg);
    //cv::waitKey(0);

    // Resize to fit video target
    cv::Mat bgraImg_OutputSize;
    cv::resize(bgraImg, bgraImg_OutputSize, cv::Size(_frameWidth, _frameHeight));

    auto deltaT{steady_clock::now() - _lastFrame};
    auto millis{duration_cast<milliseconds>(deltaT).count()};
    _lastFrame = steady_clock::now();
    double fps = 1.0 / (static_cast<double>(millis) / 1000.0);

    std::stringstream ss;
    ss << "FPS: " << std::fixed << std::setprecision(1) << fps;
    cv::putText(bgraImg_OutputSize,
                ss.str(),
                cv::Point(30, 100),
                cv::HersheyFonts::FONT_HERSHEY_PLAIN,
                1,
                cv::Scalar(255, 0, 255, 255));

    // convert RGBA to ARGB as I didn't find a RGBA to YUV422 function and openCV doesn't
    // have an ARGB encoding except when converting from beyer which we don't
    //cv::Mat argbImg_OutputSize(bgraImg_OutputSize.size(), bgraImg_OutputSize.type());
    /*libyuv::BGRAToARGB(bgraImg_OutputSize.data, bgraImg_OutputSize.cols * 4, argbImg_OutputSize.data,
                       argbImg_OutputSize.cols * 4, argbImg_OutputSize.cols, argbImg_OutputSize.rows);*/

    // even though the method is called ARGB 2 yuv2 it requires the source image to be in BGRA format
    // or else everything has a blue tinge
    cv::Mat yuv422Output(bgraImg_OutputSize.size(), CV_8UC2);
    libyuv::ARGBToYUY2(bgraImg_OutputSize.data, bgraImg_OutputSize.cols * 4,
                       yuv422Output.data, yuv422Output.cols * 2, bgraImg_OutputSize.cols, bgraImg_OutputSize.rows);

    // write to socket
    size_t finalSize = yuv422Output.elemSize() * yuv422Output.cols * yuv422Output.rows;
    if (finalSize != _videoFormat.fmt.pix.sizeimage)
    {
        throw std::runtime_error("yuv array incorrect size");
    }
    write(_dev_fd.get(), reinterpret_cast<void *>(yuv422Output.data), finalSize);
}

}
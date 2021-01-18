#include "FaceDetector.hpp"
#include <exception>

using namespace cv;

namespace webcam {

FaceDetector::FaceDetector() {
    cv::String face_cascade_name = cv::samples::findFile( "haarcascades/haarcascade_frontalface_alt2.xml" );

    if( !_face_cascade.load( face_cascade_name ) )
    {
        throw std::runtime_error("unable to load");
    }
}

Point FaceDetector::detectFace(cv::Mat & bgra) {
    cv::Mat frame_gray;
    cvtColor( bgra, frame_gray, COLOR_BGRA2GRAY);
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    std::vector<Rect> faces;
    _face_cascade.detectMultiScale( frame_gray, faces );
    for(auto & face : faces)
    {
        Point center( face.x + face.width/2, face.y + face.height/2 );

        return center;
    }

    return Point(0,0);
}

}
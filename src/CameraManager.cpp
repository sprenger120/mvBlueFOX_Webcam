#include "CameraManager.hpp"
#include <stdexcept>
#include <iostream>

namespace webcam::Driver {

CameraManager::CameraManager() {

    const unsigned int devCnt = _devMgr.deviceCount();
    if (devCnt == 0)
    {
        throw std::runtime_error("No compliant device found!");
    }
    std::cout<<"Found "<<devCnt<<" devices"<<std::endl;

    // display every device detected that matches
    for (unsigned int i = 0; i < devCnt; i++)
    {
        Device *dev = _devMgr[i];
        _cameras.emplace_back(new Camera(dev));
    }

}

}
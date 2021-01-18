#pragma once
#include <mvIMPACT_CPP/mvIMPACT_acquire.h>
#include <memory>
#include "Camera.hpp"

using mvIMPACT::acquire::DeviceManager;

namespace webcam::Driver {

class CameraManager {
public:
    CameraManager();

    CameraManager(const CameraManager &) = delete;
    CameraManager(CameraManager &&) = delete;
    CameraManager &operator=(const CameraManager &) = delete;
    CameraManager &operator=(CameraManager &&) = delete;

private:
    DeviceManager _devMgr;
    std::vector<std::unique_ptr<Camera>> _cameras;

};

}
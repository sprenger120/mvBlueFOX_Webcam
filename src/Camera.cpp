#include "Camera.hpp"
#include "CameraHelper.hpp"
#include <stdexcept>
#include <iostream>

namespace webcam::Driver {

Camera::Camera(Device *dev) : _dev(dev), _webcam("/dev/video0", 640, 480) {
    if (dev == nullptr)
    {
        throw std::invalid_argument("nullptr");
    }

    std::cout << _dev->serial.read() << " (" << _dev->product.read() << ", " << _dev->family.read();
    if (_dev->interfaceLayout.isValid())
    {
        // if this device offers the 'GenICam' interface switch it on, as this will
        // allow are better control over GenICam compliant devices
        CameraHelper::conditionalSetProperty(_dev->interfaceLayout, dilGenICam, true);
        std::cout << ", interface layout: " << _dev->interfaceLayout.readS();
    }
    if (_dev->acquisitionStartStopBehaviour.isValid())
    {
        // if this device offers a user defined acquisition start/stop behaviour
        // enable it as this allows finer control about the streaming behaviour
        CameraHelper::conditionalSetProperty(_dev->acquisitionStartStopBehaviour, assbUser, true);
        std::cout << ", acquisition start/stop behaviour: " << _dev->acquisitionStartStopBehaviour.readS();
    }
    if (_dev->interfaceLayout.isValid() && !_dev->interfaceLayout.isWriteable())
    {
        if (_dev->isInUse())
        {
            std::cout << ")" << std::endl;
            throw std::runtime_error("Camera already in use");
        }
    }
    std::cout << ")" << std::endl;
    std::cout << "Opening device...";

    try
    {
        _dev->open();
    }
    catch (const ImpactAcquireException &e)
    {
        std::cout << "error" << std::endl;
        // this e.g. might happen if the same device is already opened in another process...
        std::cout << "An error occurred while opening the device " << _dev->serial.read()
                  << "(error code: " << e.getErrorCodeAsString() << ").";
        throw std::runtime_error("Error opening device");
    }
    std::cout << "ok" << std::endl;

    try
    {
        _functionInterface = std::make_unique<FunctionInterface>(_dev);
    }
    catch (const mvIMPACT::acquire::ImpactAcquireException &e)
    {
        // this e.g. might happen if the same device is already opened in another process...
        std::cout << "An error occurred while creating the function interface on device " << _dev->serial.read()
                  << "(error code: " << e.getErrorCode() << "(" << e.getErrorCodeAsString() << ")). " << std::endl;
        throw;
    }

    // Set Properties
    mvIMPACT::acquire::SettingsBlueFOX settings(_dev); // Using the "Base" settings (default)

    settings.cameraSetting.autoControlParameters.exposeUpperLimit_us.write(40000);
    settings.cameraSetting.autoControlParameters.gainUpperLimit_dB.write(12.0);
    settings.cameraSetting.autoGainControl.write(mvIMPACT::acquire::TAutoGainControl::agcOn);
    settings.cameraSetting.autoExposeControl.write(mvIMPACT::acquire::TAutoExposureControl::aecOn);
    settings.cameraSetting.offsetAutoCalibration.write(mvIMPACT::acquire::TAutoOffsetCalibration::aocOn);
    settings.imageProcessing.bayerConversionMode.write(mvIMPACT::acquire::TBayerConversionMode::bcmAdaptiveEdgeSensingPlus);
    settings.imageProcessing.whiteBalance.write(mvIMPACT::acquire::TWhiteBalanceParameter::wbpFluorescent);

    mvIMPACT::acquire::ImageDestination imgDst(_dev);
    imgDst.pixelFormat.write(idpfBGR888Packed);

    _requestProvider = std::make_unique<RequestProvider>(_dev);
    _requestProvider->acquisitionStart(AquisitionCallbackStatic, std::ref(*this));

}
void Camera::aquisitionCallback(std::shared_ptr<Request>& request) {
    if (request->isOK())
    {
        _webcam.publish(request->imageWidth.read(),
                        request->imageHeight.read(),
                        request->imageChannelCount.read(),
                        request->imageData.read());
    }
    else
    {
        std::cout << "Error: " << request->requestResult.readS() << std::endl;
    }
}

void Camera::AquisitionCallbackStatic(std::shared_ptr<Request>& request, Camera &context) {
    context.aquisitionCallback(request);
}

}
#pragma once
#include <mvIMPACT_CPP/mvIMPACT_acquire.h>
#include <memory>
#include "AquireHelper.hpp"
#include "Webcam.hpp"

using mvIMPACT::acquire::Device;
using mvIMPACT::acquire::FunctionInterface;
using mvIMPACT::acquire::helper::RequestProvider;

namespace webcam::Driver {

class Camera {
public:
    Camera(Device *);

//TODO MOVE/COPY CONSTRUCTOR, OPERATOR
private:
    Device *_dev;
    //std::thread _aquisitionThread;
    //bool _threadShouldRun{true};

    void aquisitionCallback(std::shared_ptr<Request>& pRequest);
    static void AquisitionCallbackStatic(std::shared_ptr<Request>& request, Camera &context);

    std::unique_ptr<RequestProvider> _requestProvider;
    std::unique_ptr<FunctionInterface> _functionInterface;
    webcam::Webcam _webcam;
};

}
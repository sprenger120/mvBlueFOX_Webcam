#include "CameraHelper.hpp"

#include <iostream>

//-----------------------------------------------------------------------------
// Stop the acquisition manually if this was requested
void manuallyStopAcquisitionIfNeeded(mvIMPACT::acquire::Device *pDev, const mvIMPACT::acquire::FunctionInterface &fi)
//-----------------------------------------------------------------------------
{
    if (pDev->acquisitionStartStopBehaviour.read() == mvIMPACT::acquire::assbUser)
    {
        const mvIMPACT::acquire::TDMR_ERROR result = static_cast<mvIMPACT::acquire::TDMR_ERROR>( fi.acquisitionStop());
        if (result != mvIMPACT::acquire::DMR_NO_ERROR)
        {
            std::cout << "'FunctionInterface.acquisitionStop' returned with an unexpected result: " << result
                      << "(" << mvIMPACT::acquire::ImpactAcquireException::getErrorCodeAsString(result) << ")"
                      << std::endl;
        }
    }
}

//-----------------------------------------------------------------------------
// Start the acquisition manually if this was requested(this is to prepare the driver for data capture and tell the device to start streaming data)
void manuallyStartAcquisitionIfNeeded(mvIMPACT::acquire::Device *pDev, const mvIMPACT::acquire::FunctionInterface &fi)
//-----------------------------------------------------------------------------
{
    if (pDev->acquisitionStartStopBehaviour.read() == mvIMPACT::acquire::assbUser)
    {
        const mvIMPACT::acquire::TDMR_ERROR result = static_cast<mvIMPACT::acquire::TDMR_ERROR>( fi.acquisitionStart());
        if (result != mvIMPACT::acquire::DMR_NO_ERROR)
        {
            std::cout << "'FunctionInterface.acquisitionStart' returned with an unexpected result: " << result
                      << "(" << mvIMPACT::acquire::ImpactAcquireException::getErrorCodeAsString(result) << ")"
                      << std::endl;
        }
    }
}
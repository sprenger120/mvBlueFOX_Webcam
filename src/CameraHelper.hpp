#pragma once
#include <iostream>
#include <vector>
#include <mvIMPACT_CPP/mvIMPACT_acquire.h>

void manuallyStopAcquisitionIfNeeded(mvIMPACT::acquire::Device *pDev, const mvIMPACT::acquire::FunctionInterface &fi);
void manuallyStartAcquisitionIfNeeded(mvIMPACT::acquire::Device *pDev, const mvIMPACT::acquire::FunctionInterface &fi);

namespace webcam::Driver {

class CameraHelper {
public:

    /// \brief Checks is a certain value for property is supported.
    template<class _Tx, typename _Ty>
    static bool supportsValue(const _Tx &prop, const _Ty &value) {
        if (prop.hasDict())
        {
            typename std::vector<_Ty> sequence;
            prop.getTranslationDictValues(sequence);
            return std::find(sequence.begin(), sequence.end(), value) != sequence.end();
        }

        if (prop.hasMinValue() && (prop.getMinValue() > value))
        {
            return false;
        }

        if (prop.hasMaxValue() && (prop.getMaxValue() < value))
        {
            return false;
        }

        return true;
    }

    /// \brief Sets a property to a certain value if this value is supported.
    template<typename _Ty, typename _Tx>
    static void conditionalSetProperty(const _Ty &prop, const _Tx &value, bool boSilent = false) {
        if (prop.isValid() && prop.isWriteable() && supportsValue(prop, value))
        {
            prop.write(value);
            if (!boSilent)
            {
                std::cout << "Property '" << prop.name() << "' set to '" << prop.readS() << "'." << std::endl;
            }
        }
    }
};

}

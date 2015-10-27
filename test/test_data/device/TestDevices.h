//
// Created by Frank Krick on 9/12/15.
//

#ifndef INSTRUMENT_TOOL_KIT_TESTDEVICES_H
#define INSTRUMENT_TOOL_KIT_TESTDEVICES_H

#include <Typedef.h>
#include <device/Device.h>

using namespace itk;

class TestDevices {
public:
    static Device::Ptr parameterCopyDevice(IndexType numParameters, IndexType sourceParameterId);
    static Device::Ptr inputCopyDevice();
    static Device::Ptr inputAddDevice();
};


#endif //INSTRUMENT_TOOL_KIT_TESTDEVICES_H

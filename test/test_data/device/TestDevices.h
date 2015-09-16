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
    static Device::Ptr createParameterCopyDevice(IndexType numParameters, IndexType sourceParameterId);

};


#endif //INSTRUMENT_TOOL_KIT_TESTDEVICES_H

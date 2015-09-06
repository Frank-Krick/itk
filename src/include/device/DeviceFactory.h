//
// Created by Frank Krick on 9/5/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICEFACTORY_H
#define INSTRUMENT_TOOL_KIT_DEVICEFACTORY_H

#include "Device.h"

namespace itk {

    class DeviceFactory {
    public:
        static Device::Ptr createDevice(DeviceType type);
    };

}

#endif //INSTRUMENT_TOOL_KIT_DEVICEFACTORY_H

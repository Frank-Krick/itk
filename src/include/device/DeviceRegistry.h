//
// Created by Frank Krick on 10/8/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICEREGISTRY_H
#define INSTRUMENT_TOOL_KIT_DEVICEREGISTRY_H

#include <vector>

#include "Device.h"

namespace itk {

    class DeviceRegistry {
    public:
        typedef std::vector<Device::Ptr> DeviceList;

        DeviceRegistry();

        DeviceList registeredDevices();

    private:
        DeviceList _registeredDevices;
    };

}

#endif //INSTRUMENT_TOOL_KIT_DEVICEREGISTRY_H

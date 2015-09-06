//
// Created by Frank Krick on 9/5/15.
//

#include <device/DeviceFactory.h>

#include "DeviceImplementation.h"

namespace itk {

    Device::Ptr DeviceFactory::createDevice(DeviceType type) {
        auto device = new DeviceImplementation();
        device->deviceType(type);
        return Device::Ptr((Device *) device);
    }

}

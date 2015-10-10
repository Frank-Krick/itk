//
// Created by Frank Krick on 9/5/15.
//

#include <device/DeviceFactory.h>

#include "DeviceImplementation.h"

namespace itk {

    Device::Ptr DeviceFactory::createDevice(DeviceType type) {
        return createDevice("", "", type);
    }

    Device::Ptr DeviceFactory::createDevice(std::string name, std::string description, DeviceType type) {
        auto device = new DeviceImplementation();
        device->deviceType(type);
        device->name(name);
        device->description(description);
        return Device::Ptr((Device *) device);
    }

}

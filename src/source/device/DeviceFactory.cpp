//
// Created by Frank Krick on 9/5/15.
//

#include <device/DeviceFactory.h>

#include "DeviceImplementation.h"

namespace itk {

using std::begin;
using std::end;

Device::Ptr DeviceFactory::createDevice(DeviceType type) {
    return createDevice("", "", type);
}

Device::Ptr DeviceFactory::createDevice(std::string name, std::string description, DeviceType type) {
    std::vector<ParameterDescription> noParam = {};
    return DeviceFactory::createDevice(name, description, type, begin(noParam), end(noParam));
}

template<class Iter>
Device::Ptr DeviceFactory::createDevice(std::string name, std::string description, DeviceType type,
                                        Iter deviceParameterBegin, Iter deviceParameterEnd) {

    auto device = new DeviceImplementation();
    device->deviceType(type);
    device->name(name);
    device->description(description);

    for (auto it = deviceParameterBegin; it != deviceParameterEnd; ++it) {
        device->addParameter(*it);
    }

    return Device::Ptr((Device *) device);
}

}

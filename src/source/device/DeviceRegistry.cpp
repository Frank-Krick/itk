//
// Created by Frank Krick on 10/8/15.
//

#include <device/DeviceRegistry.h>
#include <device/DeviceFactory.h>

#include "DeviceImplementation.h"

namespace itk {

    DeviceRegistry::DeviceRegistry() {
        _registeredDevices = {
                DeviceFactory::createDevice("Test device 01", "Description of the test device", DeviceType::AUDIO),
                DeviceFactory::createDevice("Test device 02", "Description of the test device", DeviceType::AUDIO),
                DeviceFactory::createDevice("Test device 03", "Description of the test device", DeviceType::CONTROL),
                DeviceFactory::createDevice("Test device 04", "Description of the test device", DeviceType::AUDIO),
                DeviceFactory::createDevice("Test device 05", "Description of the test device", DeviceType::AUDIO),
                DeviceFactory::createDevice("Test device 06", "Description of the test device", DeviceType::AUDIO),
                DeviceFactory::createDevice("Test device 07", "Description of the test device", DeviceType::AUDIO),
                DeviceFactory::createDevice("Test device 08", "Description of the test device", DeviceType::CONTROL),
                DeviceFactory::createDevice("Test device 09", "Description of the test device", DeviceType::AUDIO),
                DeviceFactory::createDevice("Test device 10", "Description of the test device", DeviceType::AUDIO),
                DeviceFactory::createDevice("Test device 11", "Description of the test device", DeviceType::CONTROL),
                DeviceFactory::createDevice("Test device 12", "Description of the test device", DeviceType::AUDIO)
        };
    }

    DeviceRegistry::DeviceList DeviceRegistry::registeredDevices() {
        return _registeredDevices;
    }

}

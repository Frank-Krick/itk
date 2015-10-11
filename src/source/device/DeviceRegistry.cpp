#include <device/DeviceRegistry.h>
#include <device/DeviceFactory.h>

#include "DeviceImplementation.h"

namespace itk {

DeviceRegistry::DeviceRegistry() {
    using std::begin;
    using std::end;

    std::vector<ParameterDescription> parameters = {
            ParameterDescription(0, "Frequency", "Frequency of the Generator", 1, 22000),
            ParameterDescription(1, "Param 2", "Frequency of the Generator", 0, 1),
            ParameterDescription(2, "Param 3", "Frequency of the Generator", 1, 300)
    };

    _registeredDevices = {
            DeviceFactory::createDevice(
                    "Test device 01",
                    "Description of the test device",
                    DeviceType::AUDIO,
                    begin(parameters),
                    end(parameters)),
            DeviceFactory::createDevice(
                    "Test device 02",
                    "Description of the test device",
                    DeviceType::AUDIO,
                    begin(parameters),
                    end(parameters)),
            DeviceFactory::createDevice(
                    "Test device 03",
                    "Description of the test device",
                    DeviceType::CONTROL,
                    begin(parameters),
                    end(parameters)),
            DeviceFactory::createDevice(
                    "Test device 04",
                    "Description of the test device",
                    DeviceType::AUDIO,
                    begin(parameters),
                    end(parameters)),
            DeviceFactory::createDevice(
                    "Test device 05",
                    "Description of the test device",
                    DeviceType::AUDIO,
                    begin(parameters),
                    end(parameters)),
            DeviceFactory::createDevice(
                    "Test device 06",
                    "Description of the test device",
                    DeviceType::AUDIO,
                    begin(parameters),
                    end(parameters)),
            DeviceFactory::createDevice(
                    "Test device 07",
                    "Description of the test device",
                    DeviceType::AUDIO,
                    begin(parameters),
                    end(parameters)),
            DeviceFactory::createDevice(
                    "Test device 08",
                    "Description of the test device",
                    DeviceType::CONTROL,
                    begin(parameters),
                    end(parameters)),
            DeviceFactory::createDevice(
                    "Test device 09",
                    "Description of the test device",
                    DeviceType::AUDIO,
                    begin(parameters),
                    end(parameters)),
    };
}

DeviceRegistry::DeviceList DeviceRegistry::registeredDevices() {
    return _registeredDevices;
}

}

//
// Created by Frank Krick on 10/10/15.
//

#ifndef INSTRUMENT_TOOL_KIT_PYDEVICEREGISTRY_H
#define INSTRUMENT_TOOL_KIT_PYDEVICEREGISTRY_H

#include <device/DeviceRegistry.h>

namespace itk { namespace wrapper {

class PyDeviceRegistry {
public:

    PyDeviceRegistry() {}

    boost::python::list registeredDevices();

private:
    DeviceRegistry _deviceRegistry;
};

}}
#endif //INSTRUMENT_TOOL_KIT_PYDEVICEREGISTRY_H

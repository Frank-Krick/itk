//
// Created by Frank Krick on 10/10/15.
//

#include "PyDeviceRegistry.h"

#include "PyDevice.h"

namespace itk { namespace wrapper {

boost::python::list PyDeviceRegistry::registeredDevices() {
    auto result = boost::python::list();
    for (auto device : _deviceRegistry.registeredDevices()) {
        result.append(PyDevice(device));
    }
    return result;
}

}}
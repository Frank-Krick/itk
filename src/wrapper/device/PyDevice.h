#ifndef INSTRUMENT_TOOL_KIT_PYDEVICE_H
#define INSTRUMENT_TOOL_KIT_PYDEVICE_H

#include <boost/python/list.hpp>

#include <device/Device.h>

namespace itk { namespace wrapper {

class PyDevice {
public:
    PyDevice(const Device::Ptr &device) : _device(device) {}

    std::string name() { return _device->name(); }
    std::string description() { return _device->description(); }
    DeviceType deviceType() { return _device->deviceType(); }
    boost::python::list availableParameters();

    Device::Ptr device() { return _device; }

private:
    const Device::Ptr _device;
};

}}

#endif //INSTRUMENT_TOOL_KIT_PYDEVICE_H

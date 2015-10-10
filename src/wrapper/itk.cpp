#ifndef INSTRUMENT_TOOL_KIT_ITK_H
#define INSTRUMENT_TOOL_KIT_ITK_H

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <device/Device.h>
#include <device/DeviceRegistry.h>

namespace boost {
    template<class T>
    T* get_pointer(std::shared_ptr<T> p) { return p.get(); }
}

BOOST_PYTHON_MODULE(itk)
{
    using boost::python::class_;
    using boost::python::no_init;
    using boost::noncopyable;
    using boost::python::vector_indexing_suite;
    using itk::Device;
    using itk::DeviceRegistry;

    class_<Device, Device::Ptr, Device*, noncopyable>("Device", no_init)
            .add_property("name", &Device::name)
            .add_property("description", &Device::description)
            .add_property("deviceType", &Device::deviceType);

    boost::python::register_ptr_to_python<Device::Ptr>();

    class_<DeviceRegistry>("DeviceRegistry")
            .def("registeredDevices", &DeviceRegistry::registeredDevices);
}

#endif //INSTRUMENT_TOOL_KIT_ITK_H

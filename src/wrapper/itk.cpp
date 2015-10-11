#ifndef INSTRUMENT_TOOL_KIT_ITK_H
#define INSTRUMENT_TOOL_KIT_ITK_H

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <device/Device.h>
#include <device_graph/DeviceGraph.h>
#include <device_graph/DeviceGraphFactory.h>

#include "device/PyDeviceRegistry.h"

namespace boost {
    template<class T>
    T* get_pointer(std::shared_ptr<T> p) { return p.get(); }
}

BOOST_PYTHON_MODULE(itk)
{
    using boost::python::class_;
    using boost::python::enum_;
    using boost::python::no_init;
    using boost::noncopyable;
    using boost::python::vector_indexing_suite;
    using itk::Device;
    using itk::DeviceType;
    using itk::wrapper::PyDeviceRegistry;
    using itk::DeviceGraph;
    using itk::DeviceGraphFactory;

    class_<Device, Device::Ptr, noncopyable>("Device", no_init)
            .add_property("name", &Device::name)
            .add_property("description", &Device::description)
            .add_property("deviceType", &Device::deviceType);

    class_<PyDeviceRegistry>("DeviceRegistry")
            .def("registeredDevices", &PyDeviceRegistry::registeredDevices);

    enum_<DeviceType>("DeviceType")
            .value("Audio", DeviceType::AUDIO)
            .value("Control", DeviceType::CONTROL);

    void (DeviceGraph::*connect_audio)(typename itk::IndexType, typename itk::IndexType) = &DeviceGraph::connect;
    void (DeviceGraph::*connect_control)(typename itk::IndexType, typename itk::IndexType, typename itk::IndexType) = &DeviceGraph::connect;

    class_<DeviceGraph, DeviceGraph::Ptr, noncopyable>("DeviceGraph", no_init )
            .def("add_device", &DeviceGraph::addDevice)
            .def("create", &DeviceGraphFactory::createDeviceGraph)
            .staticmethod("create")
            .def("connect", connect_audio)
            .def("connect", connect_control);
}

#endif //INSTRUMENT_TOOL_KIT_ITK_H

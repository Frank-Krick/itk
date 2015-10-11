#ifndef INSTRUMENT_TOOL_KIT_ITK_H
#define INSTRUMENT_TOOL_KIT_ITK_H

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <device/Device.h>
#include <device_graph/DeviceGraph.h>
#include <device_graph/DeviceGraphFactory.h>

#include "device/PyDeviceRegistry.h"
#include "device/PyDevice.h"
#include "device_graph/PyDeviceGraph.h"

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
    using itk::ParameterDescription;
    using itk::wrapper::PyDevice;
    using itk::wrapper::PyDeviceGraph;

    class_<ParameterDescription>("ParameterDescription", no_init)
            .def_readonly("name", &ParameterDescription::name)
            .def_readonly("description", &ParameterDescription::description)
            .def_readonly("max", &ParameterDescription::max)
            .def_readonly("min", &ParameterDescription::min)
            .def_readonly("id", &ParameterDescription::id);

    class_<PyDevice>("Device", no_init)
            .add_property("name", &PyDevice::name)
            .add_property("description", &PyDevice::description)
            .add_property("deviceType", &PyDevice::deviceType)
            .add_property("available_parameters", &PyDevice::availableParameters);

    class_<PyDeviceRegistry>("DeviceRegistry")
            .def("registeredDevices", &PyDeviceRegistry::registeredDevices);

    enum_<DeviceType>("DeviceType")
            .value("Audio", DeviceType::AUDIO)
            .value("Control", DeviceType::CONTROL);

    void (PyDeviceGraph::*connect_audio)(
            typename itk::IndexType,
            typename itk::IndexType) = &PyDeviceGraph::connect;

    void (PyDeviceGraph::*connect_control)(
            typename itk::IndexType,
            typename itk::IndexType,
            typename itk::IndexType) = &PyDeviceGraph::connect;

    bool (PyDeviceGraph::*isConnected_audio)(
            typename itk::IndexType,
            typename itk::IndexType) = &PyDeviceGraph::isConnected;

    bool (PyDeviceGraph::*isConnected_control)(
            typename itk::IndexType,
            typename itk::IndexType,
            typename itk::IndexType) = &PyDeviceGraph::isConnected;

    class_<PyDeviceGraph, noncopyable>("DeviceGraph")
            .def("add_device", &PyDeviceGraph::addDevice)
            .def("connect", connect_audio)
            .def("connect", connect_control)
            .def("is_connected", isConnected_audio)
            .def("is_connected", isConnected_control);
}

#endif //INSTRUMENT_TOOL_KIT_ITK_H

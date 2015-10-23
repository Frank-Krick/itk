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

#include <Typedef.h>

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
    using itk::DataBuffer;

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
            .add_property("availableParameters", &PyDevice::availableParameters);

    class_<PyDeviceRegistry>("DeviceRegistry")
            .def_readonly("registeredDevices", &PyDeviceRegistry::registeredDevices);

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

    itk::DataType (PyDeviceGraph::*getParameterValue)(
            typename itk::IndexType,
            typename itk::IndexType) = &PyDeviceGraph::parameterValue;

    void (PyDeviceGraph::*setParameterValue)(
            typename itk::IndexType,
            typename itk::IndexType,
            typename itk::DataType) = &PyDeviceGraph::parameterValue;

    class_<PyDeviceGraph::DeviceDescription>("DeviceDescription", no_init)
            .def_readonly("name", &PyDeviceGraph::DeviceDescription::name)
            .def_readonly("deviceId", &PyDeviceGraph::DeviceDescription::deviceId)
            .def_readonly("device", &PyDeviceGraph::DeviceDescription::device);

    class_<DeviceGraph::ControlConnection>("ControlConnection")
            .def_readonly("source", &DeviceGraph::ControlConnection::source)
            .def_readonly("target", &DeviceGraph::ControlConnection::target)
            .def_readonly("parameter", &DeviceGraph::ControlConnection::parameterId);

    class_<DeviceGraph::AudioConnection>("AudioConnection")
            .def_readonly("source", &DeviceGraph::AudioConnection::source)
            .def_readonly("target", &DeviceGraph::AudioConnection::target);

    class_<PyDeviceGraph, noncopyable>("DeviceGraph")
            .def("add_device", &PyDeviceGraph::addDevice)
            .def("device", &PyDeviceGraph::device)
            .def("connect", connect_audio)
            .def("connect", connect_control)
            .def("is_connected", isConnected_audio)
            .def("is_connected", isConnected_control)
            .def("parameter_value", getParameterValue)
            .def("parameter_value", setParameterValue)
            .add_property("devices", &PyDeviceGraph::devices)
            .add_property("controlConnections", &PyDeviceGraph::controlConnections)
            .add_property("audioConnections", &PyDeviceGraph::audioConnections);

    class_<DataBuffer>("DataBuffer")
            .def(vector_indexing_suite<DataBuffer>());
}

#endif //INSTRUMENT_TOOL_KIT_ITK_H

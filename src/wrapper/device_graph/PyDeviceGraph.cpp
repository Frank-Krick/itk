#include "PyDeviceGraph.h"

#include <algorithm>
#include <iterator>

namespace itk { namespace wrapper {

IndexType PyDeviceGraph::addDevice(PyDevice device) {
    return _deviceGraph->addDevice(device.device());
}

PyDeviceGraph::DeviceDescription PyDeviceGraph::device(IndexType deviceId) {
    auto device = _deviceGraph->device(deviceId);
    return PyDeviceGraph::DeviceDescription(device);
}

void PyDeviceGraph::connect(IndexType sourceId, IndexType targetId) {
    _deviceGraph->connect(sourceId, targetId);
}

void PyDeviceGraph::connect(IndexType sourceId, IndexType targetId, IndexType parameterId) {
    _deviceGraph->connect(sourceId, targetId, parameterId);
}

bool PyDeviceGraph::isConnected(IndexType sourceId, IndexType targetId) {
    return _deviceGraph->isConnected(sourceId, targetId);
}

bool PyDeviceGraph::isConnected(IndexType sourceId, IndexType targetTd, IndexType parameterId) {
    return _deviceGraph->isConnected(sourceId, targetTd, parameterId);
}

template <class T>
boost::python::list to_python_list(T container) {
    boost::python::list result;
    for (auto item : container) {
        result.append(item);
    }
    return result;
}

boost::python::list PyDeviceGraph::devices() {
    using std::transform;
    using std::begin;
    using std::end;
    using std::back_inserter;
    using std::vector;

    vector<PyDeviceGraph::DeviceDescription> converted;
    auto devices = _deviceGraph->devices();
    transform(begin(devices), end(devices), back_inserter(converted),
        [](DeviceGraph::DeviceDescription description) { return PyDeviceGraph::DeviceDescription(description); });
    return to_python_list(converted);
}

boost::python::list PyDeviceGraph::audioConnections() {
    return to_python_list(_deviceGraph->audioConnections());
}

boost::python::list PyDeviceGraph::controlConnections() {
    return to_python_list(_deviceGraph->controlConnections());
}


DataType PyDeviceGraph::parameterValue(IndexType deviceId, IndexType parameterId) {
    return _deviceGraph->parameterValue(deviceId, parameterId);
}

void PyDeviceGraph::parameterValue(IndexType deviceId, IndexType parameterId, DataType value) {
    _deviceGraph->parameterValue(deviceId, parameterId, value);
}

}}

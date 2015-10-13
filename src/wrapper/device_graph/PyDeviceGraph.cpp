#include "PyDeviceGraph.h"

namespace itk { namespace wrapper {

IndexType PyDeviceGraph::addDevice(PyDevice device) {
    return _deviceGraph->addDevice(device.device());
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
    return to_python_list(_deviceGraph->devices());
}


boost::python::list PyDeviceGraph::audioConnections() {
    return to_python_list(_deviceGraph->audioConnections());
}

boost::python::list PyDeviceGraph::controlConnections() {
    return to_python_list(_deviceGraph->controlConnections());
}

}}
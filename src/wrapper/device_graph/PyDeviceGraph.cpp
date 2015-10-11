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

}}
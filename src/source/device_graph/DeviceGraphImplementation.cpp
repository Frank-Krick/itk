//
// Created by Frank Krick on 9/2/15.
//

#include "DeviceGraphImplementation.h"

#include <exception/DeviceTypeMismatch.h>
#include <exception/DeviceMissing.h>
#include <exception/DeviceParameterMissing.h>

namespace itk {

using namespace std;
using namespace boost;

IndexType DeviceWrapper::nextDeviceId = 0;

IndexType DeviceGraphImplementation::addDevice(Device::Ptr device) {
    auto vertex = add_vertex(_graph);
    auto deviceWrapper = DeviceWrapper(vertex, device);
    auto deviceId = deviceWrapper.deviceId;
    _deviceTable[deviceId] = deviceWrapper;
    _deviceIdVertexMap[deviceId] = vertex;
    _parameterTable->registerDeviceParameters(deviceId, *device);
    _graph[vertex].deviceId = deviceId;
    return deviceId;
}

void DeviceGraphImplementation::removeDevice(IndexType deviceId) {
    auto vertex = vertexFromDeviceId(deviceId);
    clear_vertex(vertex, _graph);
    remove_vertex(vertex, _graph);
    _deviceTable.erase(deviceId);
    _deviceIdVertexMap.erase(deviceId);
    _parameterTable->unregisterDeviceParameters(deviceId);
}

void DeviceGraphImplementation::connect(IndexType sourceId, IndexType targetId) {
    auto sourceVertex = vertexFromDeviceId(sourceId);
    auto targetVertex = vertexFromDeviceId(targetId);
    auto sourceDevice = _deviceTable[sourceId];
    auto targetDevice = _deviceTable[targetId];
    if (sourceDevice.device->deviceType() != DeviceType::AUDIO ||
        targetDevice.device->deviceType() != DeviceType::AUDIO) {

        throw DeviceTypeMismatch(
                sourceDevice.device->deviceType(),
                targetDevice.device->deviceType(),
                ConnectionType::CONTROL);

    } else {
        typename Graph::edge_descriptor edge;
        bool isNewEdge;
        tie(edge, isNewEdge) = add_edge(sourceVertex, targetVertex, _graph);
        _graph[edge].type = ConnectionType::AUDIO;
    }
}

void DeviceGraphImplementation::connect(IndexType sourceId,
                                        IndexType targetId,
                                        IndexType parameterId) {

    auto sourceType = _deviceTable.at(sourceId).device->deviceType();
    auto targetType = _deviceTable.at(targetId).device->deviceType();
    if (sourceType == DeviceType::AUDIO)
        throw DeviceTypeMismatch(sourceType, targetType, ConnectionType::CONTROL);

    if (!_parameterTable->hasParameter(targetId, parameterId))
        throw DeviceParameterMissing(targetId, parameterId);

    auto sourceVertex = vertexFromDeviceId(sourceId);
    auto targetVertex = vertexFromDeviceId(targetId);
    typename Graph::edge_descriptor edge;
    bool isNewEdge;
    tie(edge, isNewEdge) = add_edge(sourceVertex, targetVertex, _graph);
    _graph[edge].type = ConnectionType::CONTROL;
    _graph[edge].parameterId = parameterId;
}

void DeviceGraphImplementation::disconnect(IndexType sourceId, IndexType targetId) {}

bool DeviceGraphImplementation::isConnected(IndexType sourceId, IndexType targetId) {
    typename Graph::out_edge_iterator ei, ei_end;
    auto sourceVertex = vertexFromDeviceId(sourceId);
    auto targetVertex = vertexFromDeviceId(targetId);
    for (tie(ei, ei_end) = out_edges(sourceVertex, _graph); ei != ei_end; ei++) {
        if (_graph[*ei].type == ConnectionType::AUDIO &&
            target(*ei, _graph) == targetVertex) return true;
    }
    return false;
}

bool DeviceGraphImplementation::isConnected(IndexType sourceId, IndexType targetId, IndexType parameterId) {
    typename Graph::out_edge_iterator ei, ei_end;
    auto sourceVertex = vertexFromDeviceId(sourceId);
    auto targetVertex = vertexFromDeviceId(targetId);
    for (tie(ei, ei_end) = out_edges(sourceVertex, _graph); ei != ei_end; ei++) {
        if (_graph[*ei].type == ConnectionType::CONTROL &&
            target(*ei, _graph) == targetVertex &&
            _graph[*ei].parameterId == parameterId) return true;
    }
    return false;
}

Instance::Ptr DeviceGraphImplementation::createInstance(unsigned int bufferSize) {
    return _instanceBuilder.createInstance(bufferSize);
}

bool DeviceGraphImplementation::isInstanceUpToDate(Instance &instance) {
    return false;
}

std::string DeviceWrapper::defaultName(IndexType deviceId) {
    std::stringstream stream;
    stream << "device " << deviceId;
    return stream.str();
}

DeviceGraphImplementation::Vertex DeviceGraphImplementation::vertexFromDeviceId(IndexType deviceId) {
    return _deviceIdVertexMap.at(deviceId);
}

DeviceGraphImplementation::~DeviceGraphImplementation() {}

DeviceGraph::DeviceDescriptions DeviceGraphImplementation::devices() {
    DeviceDescriptions devices;
    for (auto device : _deviceTable) {
        auto description = describeDevice(device.second);
        devices.push_back(description);
    }
    return devices;
}

DeviceGraph::DeviceDescription DeviceGraphImplementation::device(IndexType deviceId) {
    auto device = _deviceTable.at(deviceId);
    return describeDevice(device);
}

DeviceGraph::DeviceDescription DeviceGraphImplementation::describeDevice(DeviceWrapper &device) {
    DeviceDescription description;
    description.name = device.name;
    description.deviceId = device.deviceId;
    description.device = device.device;
    return description;
}

DeviceGraph::AudioConnections DeviceGraphImplementation::audioConnections() {
    AudioConnections result;
    typename Graph::edge_iterator ei_begin, ei_end;
    tie(ei_begin, ei_end) = edges(_graph);
    for (auto edge = ei_begin; edge != ei_end; ++edge) {
        if (_graph[*edge].type == ConnectionType::AUDIO) {
            AudioConnection connection;
            auto sourceVertex = source(*edge, _graph);
            auto targetVertex = target(*edge, _graph);
            connection.source = _graph[sourceVertex].deviceId;
            connection.target = _graph[targetVertex].deviceId;
            result.push_back(connection);
        }
    }
    return result;
}

DeviceGraph::ControlConnections DeviceGraphImplementation::controlConnections() {
    ControlConnections result;
    typename Graph::edge_iterator ei_begin, ei_end;
    tie(ei_begin, ei_end) = edges(_graph);
    for (auto edge = ei_begin; edge != ei_end; ++edge) {
        if (_graph[*edge].type == ConnectionType::CONTROL) {
            ControlConnection connection;
            auto sourceVertex = source(*edge, _graph);
            auto targetVertex = target(*edge, _graph);
            connection.source = _graph[sourceVertex].deviceId;
            connection.target = _graph[targetVertex].deviceId;
            connection.parameterId = _graph[*edge].parameterId;
            result.push_back(connection);
        }
    }
    return result;
}

DataType DeviceGraphImplementation::parameterValue(IndexType deviceId, IndexType parameterId) {
    return _parameterTable->parameterValue(deviceId, parameterId);
}

void DeviceGraphImplementation::parameterValue(IndexType deviceId, IndexType parameterId, DataType value) {
    _parameterTable->parameterValue(deviceId, parameterId, value);
}

DeviceGraphImplementation::DeviceGraphImplementation() : _instanceBuilder(*this, _graph, *_parameterTable) {}

DeviceWrapper::DeviceWrapper() {
    vertex = 0;
    device = Device::Ptr();
    deviceId = 0;
    name = defaultName(0);
}

DeviceWrapper::DeviceWrapper(Vertex vertex, Device::Ptr device) {
    this->vertex = vertex;
    this->device = device;
    this->deviceId = nextDeviceId++;
    this->name = defaultName(this->deviceId);
}

DeviceWrapper::DeviceWrapper(const DeviceWrapper &dw) {
    this->vertex = dw.vertex;
    this->device = dw.device;
    this->deviceId = dw.deviceId;
    this->name = dw.name;
}

DeviceWrapper &DeviceWrapper::operator=(DeviceWrapper wrapper) {
    std::swap(vertex, wrapper.vertex);
    std::swap(device, wrapper.device);
    std::swap(deviceId, wrapper.deviceId);
    std::swap(name, wrapper.name);
    return *this;
}

} // namespace itk

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
    auto vertex = add_vertex(graph);
    auto deviceWrapper = DeviceWrapper(vertex, device);
    auto deviceId = deviceWrapper.deviceId;
    deviceTable[deviceId] = deviceWrapper;
    deviceIdVertexMap[deviceId] = vertex;
    parameterTable->registerDeviceParameters(deviceId, *device);
    graph[vertex].deviceId = deviceId;
    return deviceId;
}

void DeviceGraphImplementation::removeDevice(IndexType deviceId) {
    auto vertex = vertexFromDeviceId(deviceId);
    clear_vertex(vertex, graph);
    remove_vertex(vertex, graph);
    deviceTable.erase(deviceId);
    deviceIdVertexMap.erase(deviceId);
    parameterTable->unregisterDeviceParameters(deviceId);
}

void DeviceGraphImplementation::connect(IndexType sourceId, IndexType targetId) {
    auto sourceVertex = vertexFromDeviceId(sourceId);
    auto targetVertex = vertexFromDeviceId(targetId);
    auto sourceDevice = deviceTable[sourceId];
    auto targetDevice = deviceTable[targetId];
    if (sourceDevice.device->deviceType() != DeviceType::AUDIO ||
        targetDevice.device->deviceType() != DeviceType::AUDIO) {

        throw DeviceTypeMismatch(
                sourceDevice.device->deviceType(),
                targetDevice.device->deviceType(),
                ConnectionType::CONTROL);

    } else {
        typename Graph::edge_descriptor edge;
        bool isNewEdge;
        tie(edge, isNewEdge) = add_edge(sourceVertex, targetVertex, graph);
        graph[edge].type = ConnectionType::AUDIO;
    }
}

void DeviceGraphImplementation::connect(IndexType sourceId,
                                        IndexType targetId,
                                        IndexType parameterId) {

    auto sourceType = deviceTable.at(sourceId).device->deviceType();
    auto targetType = deviceTable.at(targetId).device->deviceType();
    if (sourceType == DeviceType::AUDIO)
        throw DeviceTypeMismatch(sourceType, targetType, ConnectionType::CONTROL);

    if (!parameterTable->hasParameter(targetId, parameterId))
        throw DeviceParameterMissing(targetId, parameterId);

    auto sourceVertex = vertexFromDeviceId(sourceId);
    auto targetVertex = vertexFromDeviceId(targetId);
    typename Graph::edge_descriptor edge;
    bool isNewEdge;
    tie(edge, isNewEdge) = add_edge(sourceVertex, targetVertex, graph);
    graph[edge].type = ConnectionType::CONTROL;
    graph[edge].parameterId = parameterId;
}

void DeviceGraphImplementation::disconnect(IndexType sourceId, IndexType targetId) {}

bool DeviceGraphImplementation::isConnected(IndexType sourceId, IndexType targetId) {
    typename Graph::out_edge_iterator ei, ei_end;
    auto sourceVertex = vertexFromDeviceId(sourceId);
    auto targetVertex = vertexFromDeviceId(targetId);
    for (tie(ei, ei_end) = out_edges(sourceVertex, graph); ei != ei_end; ei++) {
        if (graph[*ei].type == ConnectionType::AUDIO &&
            target(*ei, graph) == targetVertex) return true;
    }
    return false;
}

bool DeviceGraphImplementation::isConnected(IndexType sourceId, IndexType targetId, IndexType parameterId) {
    typename Graph::out_edge_iterator ei, ei_end;
    auto sourceVertex = vertexFromDeviceId(sourceId);
    auto targetVertex = vertexFromDeviceId(targetId);
    for (tie(ei, ei_end) = out_edges(sourceVertex, graph); ei != ei_end; ei++) {
        if (graph[*ei].type == ConnectionType::CONTROL &&
            target(*ei, graph) == targetVertex &&
            graph[*ei].parameterId == parameterId) return true;
    }
    return false;
}

DeviceGraphInstance::Ptr DeviceGraphImplementation::createInstance() {
    return std::unique_ptr<DeviceGraphInstance>();
}

bool DeviceGraphImplementation::isInstanceUpToDate(DeviceGraphInstance &instance) {
    return false;
}

std::string DeviceWrapper::defaultName(IndexType deviceId) {
    std::stringstream stream;
    stream << "device " << deviceId;
    return stream.str();
}

DeviceGraphImplementation::Vertex DeviceGraphImplementation::vertexFromDeviceId(int deviceId) {
    return deviceIdVertexMap.at(deviceId);
}

DeviceGraphImplementation::~DeviceGraphImplementation() {}

DeviceGraph::DeviceDescriptions DeviceGraphImplementation::devices() {
    DeviceDescriptions devices;
    for (auto device : deviceTable) {
        auto description = describeDevice(device.second);
        devices.push_back(description);
    }
    return devices;
}

DeviceGraph::DeviceDescription DeviceGraphImplementation::device(IndexType deviceId) {
    auto device = deviceTable.at(deviceId);
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
    tie(ei_begin, ei_end) = edges(graph);
    for (auto edge = ei_begin; edge != ei_end; ++edge) {
        if (graph[*edge].type == ConnectionType::AUDIO) {
            AudioConnection connection;
            auto sourceVertex = source(*edge, graph);
            auto targetVertex = target(*edge, graph);
            connection.source = graph[sourceVertex].deviceId;
            connection.target = graph[targetVertex].deviceId;
            result.push_back(connection);
        }
    }
    return result;
}

DeviceGraph::ControlConnections DeviceGraphImplementation::controlConnections() {
    ControlConnections result;
    typename Graph::edge_iterator ei_begin, ei_end;
    tie(ei_begin, ei_end) = edges(graph);
    for (auto edge = ei_begin; edge != ei_end; ++edge) {
        if (graph[*edge].type == ConnectionType::CONTROL) {
            ControlConnection connection;
            auto sourceVertex = source(*edge, graph);
            auto targetVertex = target(*edge, graph);
            connection.source = graph[sourceVertex].deviceId;
            connection.target = graph[targetVertex].deviceId;
            connection.parameterId = graph[*edge].parameterId;
            result.push_back(connection);
        }
    }
    return result;
}

DataType DeviceGraphImplementation::parameterValue(IndexType deviceId, IndexType parameterId) {
    return parameterTable->parameterValue(deviceId, parameterId);
}

void DeviceGraphImplementation::parameterValue(IndexType deviceId, IndexType parameterId, DataType value) {
    parameterTable->parameterValue(deviceId, parameterId, value);
}

} // namespace itk
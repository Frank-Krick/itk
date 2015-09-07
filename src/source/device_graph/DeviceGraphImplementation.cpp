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

    void DeviceGraphImplementation::removeDevice(int deviceId) {
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

    void DeviceGraphImplementation::disconnect(IndexType sourceId, IndexType targetId) {

    }

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

    DeviceGraph::DeviceDescription DeviceGraphImplementation::describeDevice(IndexType deviceId) {
        auto vertex = deviceIdVertexMap.at(deviceId);
        auto device = deviceTable.at(deviceId);
        DeviceDescription description;
        description.name = device.name;
        description.deviceId = deviceId;
        return description;
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

}
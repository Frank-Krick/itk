//
// Created by Frank Krick on 9/2/15.
//

#include "DeviceGraphImplementation.h"

#include <exception/DeviceTypeMismatch.h>

namespace itk {

    using namespace std;
    using namespace boost;

    IndexType DeviceWrapper::nextDeviceId = 0;

    IndexType DeviceGraphImplementation::addDevice(Device::Ptr device) {
        auto vertex = add_vertex(graph);
        deviceTable[vertex] = DeviceWrapper(vertex, device);
        auto deviceId = deviceTable[vertex].deviceId;
        indexMap[deviceId] = vertex;
        parameterTable->registerDeviceParameters(deviceId, *device);
        return deviceId;
    }

    void DeviceGraphImplementation::removeDevice(int deviceId) {
        auto vertex = vertexFromDeviceId(deviceId);
        clear_vertex(vertex, graph);
        remove_vertex(vertex, graph);
        deviceTable.erase(vertex);
        indexMap.erase(deviceId);
    }

    void DeviceGraphImplementation::connect(IndexType sourceId, IndexType targetId) {
        auto sourceVertex = vertexFromDeviceId(sourceId);
        auto targetVertex = vertexFromDeviceId(targetId);
        auto sourceDevice = deviceTable[sourceVertex];
        auto targetDevice = deviceTable[targetVertex];
        if (sourceDevice.device->deviceType() != DeviceType::AUDIO ||
            targetDevice.device->deviceType() != DeviceType::AUDIO) {

            throw DeviceTypeMismatch(
                    sourceDevice.device->deviceType(),
                    targetDevice.device->deviceType(),
                    ConnectionType::CONTROL);

        } else {
            add_edge(sourceVertex, targetVertex, graph);
        }
    }

    void DeviceGraphImplementation::connect(IndexType sourceId,
                                            IndexType targetId,
                                            IndexType parameterId) {

    }

    void DeviceGraphImplementation::disconnect(IndexType sourceId, IndexType targetId) {

    }

    bool DeviceGraphImplementation::isConnected(IndexType sourceId, IndexType targetId) {
        typename Graph::out_edge_iterator ei, ei_end;
        auto sourceVertex = vertexFromDeviceId(sourceId);
        auto targetVertex = vertexFromDeviceId(targetId);
        for (tie(ei, ei_end) = out_edges(sourceVertex, graph); ei != ei_end; ei++) {
            if (target(*ei, graph) == targetVertex) return true;
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
        auto vertex = indexMap.at(deviceId);
        auto device = deviceTable.at(vertex);
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

    DeviceGraphImplementation::~DeviceGraphImplementation() {

    }


    DeviceGraphImplementation::Vertex DeviceGraphImplementation::vertexFromDeviceId(int deviceId) {
        return indexMap.at(deviceId);
    }

}
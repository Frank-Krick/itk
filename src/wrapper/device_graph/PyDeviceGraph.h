#ifndef INSTRUMENT_TOOL_KIT_PYDEVICEGRAPH_H
#define INSTRUMENT_TOOL_KIT_PYDEVICEGRAPH_H

#include <boost/python/object.hpp>
#include <boost/python/list.hpp>

#include <device_graph/DeviceGraph.h>
#include <device_graph/DeviceGraphFactory.h>

#include "../device/PyDevice.h"

namespace itk { namespace wrapper {

class PyDeviceGraph {
public:
    struct DeviceDescription {
        DeviceDescription(const DeviceGraph::DeviceDescription device)
                : name(device.name), deviceId(device.deviceId), device(device.device) {}

        std::string name;
        IndexType deviceId = 0;
        PyDevice device;
    };

    PyDeviceGraph()
            : _deviceGraph(DeviceGraphFactory::createDeviceGraph()) {}

    IndexType addDevice(PyDevice device);
    PyDeviceGraph::DeviceDescription device(IndexType deviceId);

    void connect(IndexType sourceId, IndexType targetId);
    void connect(IndexType sourceId, IndexType targetId, IndexType parameterId);
    bool isConnected(IndexType sourceId, IndexType targetId);
    bool isConnected(IndexType sourceId, IndexType targetTd, IndexType parameterId);

    DataType parameterValue(IndexType deviceId, IndexType parameterId);
    void parameterValue(IndexType deviceId, IndexType parameterId, DataType value);

    boost::python::list devices();
    boost::python::list audioConnections();
    boost::python::list controlConnections();

private:
    DeviceGraph::Ptr _deviceGraph;
};

}}

#endif //INSTRUMENT_TOOL_KIT_PYDEVICEGRAPH_H

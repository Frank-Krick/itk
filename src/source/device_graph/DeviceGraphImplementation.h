#ifndef INSTRUMENT_TOOL_KIT_DEVICEGRAPHIMPLEMENTATION_H
#define INSTRUMENT_TOOL_KIT_DEVICEGRAPHIMPLEMENTATION_H

#include <device_graph/DeviceGraph.h>
#include <parameter_table/ParameterTable.h>

#include "../parameter_table/ParameterTableImplementation.h"
#include "instance_builder/InstanceBuilder.h"

#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>


namespace itk {

struct VertexData {
    IndexType deviceId = 0;
};

struct EdgeData {
    IndexType parameterId = 0;
    ConnectionType type = ConnectionType::AUDIO;
};

class DeviceWrapper;

class DeviceGraphImplementation : public DeviceGraph {
public:
    virtual void connect(IndexType sourceId, IndexType targetId) override;
    virtual void connect(IndexType sourceId, IndexType targetId, IndexType parameterId) override;
    virtual void disconnect(IndexType sourceId, IndexType targetId) override;
    virtual bool isConnected(IndexType sourceId, IndexType targetId) override;
    virtual bool isConnected(IndexType sourceId, IndexType targetId, IndexType parameterId) override;
    virtual AudioConnections audioConnections();
    virtual ControlConnections controlConnections();
    virtual void outputDeviceId(IndexType deviceId) { _outputDeviceId = deviceId; }
    virtual IndexType outputDeviceId() { return _outputDeviceId; }

    virtual Instance::Ptr createInstance(unsigned int bufferSize) override;
    virtual bool isInstanceUpToDate(Instance &instance) override;

    virtual DeviceDescription device(IndexType deviceId) override;
    virtual DeviceDescriptions devices() override;
    virtual IndexType addDevice(Device::Ptr device) override;
    virtual void removeDevice(IndexType deviceId) override;

    virtual DataType parameterValue(IndexType deviceId, IndexType parameterId) override;
    virtual void parameterValue(IndexType deviceId, IndexType parameterId, DataType value) override;

    /*
     * Definition of the BGL graph type. We are using an adjacency list. The out edges
     * are stored in a std::vector, the vertices in a std::list. This configuration
     * doesn't invalidate the vertex_descriptors. We make use of that to provide an
     * easy link between the deviceId and the vertex.
     */
    typedef boost::adjacency_list<
            boost::vecS,
            boost::vecS,
            boost::bidirectionalS,
            VertexData, EdgeData> Graph;

    typedef Graph::vertex_descriptor Vertex;
    typedef Graph::edge_descriptor Edge;

    DeviceGraphImplementation();
    virtual ~DeviceGraphImplementation();

    Vertex vertexFromDeviceId(IndexType deviceId);

    InstanceBuilder<Graph, DeviceGraphImplementation, VertexData, EdgeData>::Ptr _instanceBuilder;

private:
    typedef std::unordered_map<IndexType, DeviceWrapper> DeviceTable;
    typedef std::unordered_map<IndexType, Graph::vertex_descriptor> DeviceIdVertexMap;

    IndexType _outputDeviceId = 0;
    DeviceTable _deviceTable;
    DeviceIdVertexMap _deviceIdVertexMap;
    Graph _graph;
    ParameterTable::Ptr _parameterTable = std::make_shared<ParameterTableImplementation>();

    DeviceDescription describeDevice(DeviceWrapper& device);
};

class DeviceWrapper {
public:
    typedef DeviceGraphImplementation::Graph::vertex_descriptor Vertex;

    Vertex vertex = 0;
    Device::Ptr device = Device::Ptr();
    IndexType deviceId = 0;
    std::string name;

    DeviceWrapper();
    DeviceWrapper(Vertex vertex, Device::Ptr device);
    DeviceWrapper(const DeviceWrapper &dw);

    DeviceWrapper &operator=(DeviceWrapper wrapper);

private:
    static IndexType nextDeviceId;

    std::string defaultName(IndexType deviceId);
};

} // namespace itk

#endif //INSTRUMENT_TOOL_KIT_DEVICEGRAPHIMPLEMENTATION_H

//
// Created by Frank Krick on 9/2/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICEGRAPHIMPLEMENTATION_H
#define INSTRUMENT_TOOL_KIT_DEVICEGRAPHIMPLEMENTATION_H

#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>

#include <device_graph/DeviceGraph.h>
#include <parameter_table/ParameterTable.h>
#include "../parameter_table/ParameterTableImplementation.h"

namespace itk {

    class DeviceWrapper;

    class DeviceGraphImplementation : DeviceGraph {
    public:

        virtual ~DeviceGraphImplementation();

        virtual IndexType addDevice(Device::Ptr device) override;
        virtual void removeDevice(int deviceId) override;

        virtual void connect(IndexType sourceId, IndexType targetId) override;
        virtual void connect(IndexType sourceId, IndexType targetId, IndexType parameterId);
        virtual void disconnect(IndexType sourceId, IndexType targetId) override;
        virtual bool isConnected(IndexType sourceId, IndexType targetId) override;

        virtual DeviceGraphInstance::Ptr createInstance() override;
        virtual bool isInstanceUpToDate(DeviceGraphInstance &instance) override;

        virtual DeviceDescription describeDevice(IndexType deviceId) override;

        /*
         * Definition of the BGL graph type. We are using an adjacency list. The out edges
         * are stored in a std::vector, the vertices in a std::list. This configuration
         * doesn't invalidate the vertex_descriptors. We make use of that to provide an
         * easy link between the deviceId and the vertex.
         */
        typedef boost::adjacency_list<
                boost::vecS,
                boost::listS,
                boost::bidirectionalS> Graph;

    private:

        typedef Graph::vertex_descriptor Vertex;
        typedef std::unordered_map<Vertex, DeviceWrapper> DeviceTable;
        typedef std::unordered_map<IndexType, Graph::vertex_descriptor> IndexMap;

        DeviceTable deviceTable;
        IndexMap indexMap;
        Graph graph;
        ParameterTable::Ptr parameterTable = ParameterTable::Ptr((ParameterTable *) new ParameterTableImplementation());

        Vertex vertexFromDeviceId(int deviceId);
    };

    class DeviceWrapper {
    public:
        typedef DeviceGraphImplementation::Graph::vertex_descriptor Vertex;

        Vertex vertex = 0;
        Device::Ptr device = Device::Ptr();
        IndexType deviceId = 0;
        std::string name;

        DeviceWrapper() {
            vertex = 0;
            device = Device::Ptr();
            deviceId = 0;
            name = defaultName(0);
        }

        DeviceWrapper(Vertex vertex, Device::Ptr device) {
            this->vertex = vertex;
            this->device = device;
            this->deviceId = nextDeviceId++;
            this->name = defaultName(this->deviceId);
        }

        DeviceWrapper(const DeviceWrapper &dw) {
            this->vertex = dw.vertex;
            this->device = dw.device;
            this->deviceId = dw.deviceId;
            this->name = dw.name;
        }

    private:
        static IndexType nextDeviceId;

        std::string defaultName(IndexType deviceId);
    };
}


#endif //INSTRUMENT_TOOL_KIT_DEVICEGRAPHIMPLEMENTATION_H

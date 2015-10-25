#ifndef INSTRUMENT_TOOL_KIT_INSTANCEBUILDER_H
#define INSTRUMENT_TOOL_KIT_INSTANCEBUILDER_H

#include <processing/instance/Instance.h>
#include <parameter_table/ParameterTable.h>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/transpose_graph.hpp>

#include "../../processing/instance/InstanceImplementation.h"


namespace itk {

template <class Graph, class DeviceGraph, class VertexData, class EdgeData>
class InstanceBuilder {
public:
    typedef std::shared_ptr<InstanceBuilder<Graph, DeviceGraph, VertexData, EdgeData>> Ptr;
    typedef FunctorList<AudioFunctor::Ptr> AudioFunctorList;
    typedef FunctorList<ControlFunctor::Ptr> ControlFunctorList;

    Instance::Ptr createInstance(unsigned int bufferSize) {
        auto instance = std::make_shared<InstanceImplementation>(_parameterTable);
        instance->bufferSize(bufferSize);
        auto devices = _deviceGraph.devices();
        if (devices.size() > 0) {
            auto functorList = std::make_shared<AudioFunctorList>();
            for (auto device : devices) {
                auto functor = device.device->functor(device.deviceId);
                functorList->push_back(std::dynamic_pointer_cast<AudioFunctor>(functor));
            }
            auto functorId = instance->addAudioFunctorList(functorList);
            instance->connectOutput(functorId);
        }
        instance->allocateMemory();
        return instance;
    }

    InstanceBuilder(DeviceGraph &deviceGraph, Graph &graph, ParameterTable &parameterTable)
            : _deviceGraph(deviceGraph), _graph(graph), _parameterTable(parameterTable) {}

    ~InstanceBuilder() {}

    DeviceGraph &_deviceGraph;
    Graph &_graph;
    ParameterTable &_parameterTable;

    /*
     * Collect leafs is a BFS visitor that collects the leafs in the device
     * graph in the connected component of the output device. The BFS has
     * to be executed on a transposed graph so that the direction of the
     * edges points from input device to output device. Leafs in the
     * transposed device have an out degree of zero.
     * Therefore, the visitor records all nodes with an out degree of zero.
     */
    class CollectLeafs : public boost::default_bfs_visitor {
    public:
        typedef std::vector<typename Graph::vertex_descriptor> Leafs;

        void examine_vertex(const typename Graph::vertex_descriptor vertex, const Graph &graph) const {
            if (boost::out_degree(vertex, graph) == 0) {
                _leafs.push_back(vertex);
            }
        }

        CollectLeafs(Leafs & leafs) : bfs_visitor(), _leafs(leafs) {}

    private:
        Leafs & _leafs;
    };

    std::vector<IndexType> findPartition(typename DeviceGraph::Vertex start) {
        std::vector<IndexType> result = { _graph[start].deviceId };
        auto vertex = start;
        while (boost::out_degree(start, _graph) == 1) {
            typename Graph::edge_iterator ei, ei_end;
            std::tie(ei, ei_end) = boost::out_edges(start, _graph);
            vertex = (*ei).second;
        }
        return result;
    }

    std::vector<typename Graph::vertex_descriptor> findLeafs() {
        typename CollectLeafs::Leafs leafs;
        CollectLeafs visitor(leafs);
        auto startVertex = _deviceGraph.vertexFromDeviceId(_deviceGraph.outputDeviceId());
        Graph transposed;
        boost::transpose_graph(_graph, transposed);
        boost::breadth_first_search(transposed, startVertex, boost::visitor(visitor));
        return leafs;
    }
};

}

#endif //INSTRUMENT_TOOL_KIT_INSTANCEBUILDER_H

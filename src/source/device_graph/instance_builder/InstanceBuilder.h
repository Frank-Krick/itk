#ifndef INSTRUMENT_TOOL_KIT_INSTANCEBUILDER_H
#define INSTRUMENT_TOOL_KIT_INSTANCEBUILDER_H

#include <processing/instance/Instance.h>
#include <parameter_table/ParameterTable.h>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/transpose_graph.hpp>
#include <limits>
#include "../../processing/instance/InstanceImplementation.h"


namespace itk {

template <class Graph, class DeviceGraph, class VertexData, class EdgeData>
class InstanceBuilder {
public:
    typedef std::shared_ptr<InstanceBuilder<Graph, DeviceGraph, VertexData, EdgeData>> Ptr;
    typedef FunctorList<AudioFunctor::Ptr> AudioFunctorList;
    typedef FunctorList<ControlFunctor::Ptr> ControlFunctorList;

    struct Partition {
    public:
        typedef std::shared_ptr<Partition> Ptr;
        std::vector<typename Graph::vertex_descriptor> vertices;
        bool isOutput = false;
        typename Graph::vertex_descriptor next;
        IndexType functorId = 0;
        IndexType connectTo = 0;
        IndexType index = 0;
        AudioFunctorList::Ptr audioFunctorList;
        ControlFunctorList::Ptr controlFunctorList;
        DeviceType deviceType = DeviceType::AUDIO;
        IndexType parameter = 0;
    };

    typedef std::vector<typename Partition::Ptr> Partitions;

    Instance::Ptr createInstance(unsigned int bufferSize) {
        auto instance = std::make_shared<InstanceImplementation>(_parameterTable);
        instance->bufferSize(bufferSize);
        if (_deviceGraph.isOutputDeviceValid()) {
            auto partitions = createPartitions();
            createAndAddFunctorLists(std::begin(partitions), std::end(partitions), *instance);
            createAndAddConnections(partitions, *instance);
        }
        instance->allocateMemory();
        return instance;
    }

    void createAndAddFunctorLists(
            typename Partitions::iterator begin,
            typename Partitions::iterator end,
            InstanceImplementation &instance) {

        for (auto it = begin; it != end; it++) {
            if ((*it)->deviceType == DeviceType::AUDIO) {
                (*it)->audioFunctorList = createAudioFunctorList(*(*it));
                (*it)->functorId = instance.addAudioFunctorList((*it)->audioFunctorList);
            } else {
                (*it)->controlFunctorList = createControlFunctorList(*(*it));
                (*it)->functorId = instance.addControlFunctorList((*it)->controlFunctorList);
            }
        }
    }

    void createAndAddConnections(
            Partitions &partitions,
            InstanceImplementation &instance) {

        for (auto it = std::begin(partitions); it != std::end(partitions); ++it) {
            if (not (*it)->isOutput) {
                auto target = partitions[(*it)->connectTo]->functorId;
                auto source = (*it)->functorId;
                if ((*it)->deviceType == DeviceType::AUDIO) {
                    instance.connect(source, target);
                } else {
                    instance.connect(source, target, (*it)->parameter);
                }
            } else {
                auto target = (*it)->functorId;
                instance.connectOutput(target);
            }
        }
    }

    ControlFunctorList::Ptr createControlFunctorList(const Partition &partition) {
        std::vector<IndexType> deviceIds(partition.vertices.size());
        std::transform(std::begin(partition.vertices), std::end(partition.vertices), std::begin(deviceIds),
                       [this](typename DeviceGraph::Vertex vertex) -> IndexType {
                           return _graph[vertex].deviceId;
                       });
        auto functorList = std::make_shared<ControlFunctorList>();
        for (auto deviceId : deviceIds) {
            auto functor = _deviceGraph.device(deviceId).device->functor(deviceId);
            functorList->push_back(std::dynamic_pointer_cast<ControlFunctor>(functor));
        }
        return functorList;
    }

    AudioFunctorList::Ptr createAudioFunctorList(const Partition &partition) {
        std::vector<IndexType> deviceIds(partition.vertices.size());
        std::transform(std::begin(partition.vertices), std::end(partition.vertices), std::begin(deviceIds),
                       [this](typename DeviceGraph::Vertex vertex) -> IndexType {
                           return _graph[vertex].deviceId;
                       });
        auto functorList = std::make_shared<AudioFunctorList>();
        for (auto deviceId : deviceIds) {
            auto functor = _deviceGraph.device(deviceId).device->functor(deviceId);
            functorList->push_back(std::dynamic_pointer_cast<AudioFunctor>(functor));
        }
        return functorList;
    }

    Partitions createPartitions() {
        std::vector<typename Partition::Ptr> partitions;
        auto leafs = findLeafs();
        for (auto leaf : leafs) {
            auto leafPartition = createPartition(leaf);
            leafPartition->index = partitions.size();
            partitions.push_back(leafPartition);
        }
        return expandLeafPartitions(partitions);
    }

    std::vector<typename Partition::Ptr> expandLeafPartitions(std::vector<typename Partition::Ptr> &leafPartitions) {
        std::unordered_map<IndexType, IndexType> prevVertexToIndexMap;
        std::vector<typename Partition::Ptr> partitions(leafPartitions);
        for (auto leafPartition : leafPartitions) {
            auto prevPartition = leafPartition;
            typename Partition::Ptr partition;
            while (not prevPartition->isOutput) {
                auto partitionIndexIterator = prevVertexToIndexMap.find(prevPartition->next);
                if (partitionIndexIterator == std::end(prevVertexToIndexMap)) {
                    partition = createPartition(prevPartition->next);
                    partition->index = partitions.size();
                    partitions.push_back(partition);
                    prevPartition->connectTo = partition->index;
                    if (prevPartition->deviceType == DeviceType::CONTROL) {
                        auto lastVertex = prevPartition->vertices.back();
                        typename Graph::out_edge_iterator ei, ei_end;
                        std::tie(ei, ei_end) = boost::out_edges(lastVertex, _graph);
                        prevPartition->parameter = _graph[*ei].parameterId;
                    }
                    prevVertexToIndexMap[prevPartition->next] = partition->index;
                } else {
                    partition = partitions[(*partitionIndexIterator).second];
                    prevPartition->connectTo = partition->index;
                }
                prevPartition = partition;
            }
        }
        return partitions;
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

    typename Partition::Ptr createPartition(typename DeviceGraph::Vertex start) {
        std::vector<typename Graph::vertex_descriptor> result { start };
        auto vertex = start;
        auto deviceId = std::numeric_limits<IndexType>::max();
        auto prevDeviceId = std::numeric_limits<IndexType>::max();
        auto deviceType = DeviceType::AUDIO;
        auto prevDeviceType = DeviceType::AUDIO;
        if (boost::out_degree(vertex, _graph) == 1) {
            typename Graph::out_edge_iterator ei, ei_end;
            std::tie(ei, ei_end) = boost::out_edges(start, _graph);
            prevDeviceId = _graph[vertex].deviceId;
            prevDeviceType = _deviceGraph.device(prevDeviceId).device->deviceType();
            vertex = boost::target(*ei, _graph);
            /* select all dependend nodes that can be computed afterwards without
             * dependencies unless the node is the root.
             */
            deviceId = _graph[vertex].deviceId;
            deviceType = _deviceGraph.device(deviceId).device->deviceType();
            while (boost::out_degree(vertex, _graph) == 1 and
                    boost::in_degree(vertex, _graph) == 1 and
                    prevDeviceType == deviceType) {
                result.push_back(vertex);
                prevDeviceId = _graph[vertex].deviceId;
                prevDeviceType = _deviceGraph.device(prevDeviceId).device->deviceType();
                std::tie(ei, ei_end) = boost::out_edges(vertex, _graph);
                vertex = boost::target(*ei, _graph);
                deviceId = _graph[vertex].deviceId;
                deviceType = _deviceGraph.device(deviceId).device->deviceType();
            }
        }

        /* decide what to do with "leftovers" */
        auto partition = std::make_shared<Partition>();
        if (boost::out_degree(vertex, _graph) == 0 and
                (boost::in_degree(vertex, _graph) == 0 or boost::in_degree(vertex, _graph) == 1) and
                prevDeviceType == deviceType and
                prevDeviceId != deviceId) {
            partition->next = vertex;
            result.push_back(vertex);
            partition->vertices = result;
        } else {
            partition->next = vertex;
            partition->vertices = result;
        }
        partition->deviceType = prevDeviceType;
        auto outputVertex = _deviceGraph.vertexFromDeviceId(_deviceGraph.outputDeviceId());
        if (outputVertex == partition->vertices.back()) {
            partition->isOutput = true;
        } else {
            partition->isOutput = false;
        }
        return partition;
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

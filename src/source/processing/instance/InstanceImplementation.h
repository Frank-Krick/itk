//
// Created by Frank Krick on 9/13/15.
//

#ifndef INSTRUMENT_TOOL_KIT_INSTANCEIMPLEMENTATION_H
#define INSTRUMENT_TOOL_KIT_INSTANCEIMPLEMENTATION_H

#include <processing/instance/Instance.h>
#include <processing/functor_list/FunctorList.h>
#include <parameter_table/ParameterTable.h>
#include <processing/generator/ControlFunctor.h>

#include <vector>
#include <unordered_set>

namespace itk {

    class InstanceImplementation : public Instance {
    public:
        typedef FunctorList<AudioFunctor::Ptr> AudioFunctorList;
        typedef FunctorList<ControlFunctor::Ptr> ControlFunctorList;

        virtual void operator () (OutputChannels beginOut, OutputChannels endOut) override;

        IndexType addAudioFunctorList(AudioFunctorList::Ptr functorList);
        IndexType addControlFunctorList(ControlFunctorList::Ptr functorList);
        void connect(IndexType sourceId, IndexType targetId);
        void connect(IndexType sourceId, IndexType targetId, IndexType parameterId);
        void connectOutput(IndexType sourceId);
        void allocateMemory();
        void bufferSize(unsigned int bufferSize);

        InstanceImplementation(ParameterTable &parameterTable)
            : parameterTable(parameterTable) {}

    private:
        struct ParameterWrapper {
            DataBuffer buffer;
            bool isModulated = false;
        };

        template <class T>
        struct FunctorWrapper {
            typename T::Ptr functor;
            DataBuffer left;
            DataBuffer right;
            DataBuffer control;
            typename std::vector<FunctorWrapper<T>>::size_type index;
        };

        struct ControlEdgeEnd {
            IndexType parameterId = 0;
            IndexType deviceId = 0;
        };

        typedef std::pair<IndexType, ControlEdgeEnd> ControlEdge;
        typedef std::list<ControlEdge> ControlEdgeList;
        typedef std::vector<ControlEdgeList> ControlAdjacencyList;
        typedef std::pair<IndexType, IndexType> AudioEdge;
        typedef std::list<AudioEdge> AudioEdgeList;
        typedef std::vector<AudioEdgeList> AudioAdjacencyList;
        typedef FunctorWrapper<AudioFunctorList> AudioFunctorWrapper;
        typedef FunctorWrapper<ControlFunctorList> ControlFunctorWrapper;
        typedef std::vector<AudioFunctorWrapper> AudioFunctorTable;
        typedef std::vector<ControlFunctorWrapper> ControlFunctorTable;
        typedef std::unordered_map<IndexType, std::unordered_map<IndexType, ParameterWrapper>> DeviceParameterMap;
        typedef std::pair<IndexType, IndexType> IndexPair;
        typedef std::vector<IndexPair> IndexPairs;

        AudioFunctorTable audioFunctorTable;
        ControlFunctorTable controlFunctorTable;
        DeviceParameterMap parameterMap;
        ParameterTable & parameterTable;

        AudioAdjacencyList audioDeviceEdges;
        ControlAdjacencyList controlDeviceEdges;

        unsigned int _bufferSize = 0;
        IndexType _audioOutput = 0;

        void allocateParameterMemory();
        void allocateDeviceMemory();
        void initializeParameterMap();
        void calculateControlParameter();
        void calculateAudioOutput();
        std::vector<IndexType> findControlDeviceLeafs();
        std::vector<IndexType> findAudioDeviceLeafs();

        template <typename T>
        std::unique_ptr<typename T::ParameterDeviceMap> createInputParameterMapForFunctorList(
                FunctorWrapper<FunctorList<typename T::Ptr>> & functorList) {

            using std::unique_ptr;
            auto inputParameterMap = new typename T::ParameterDeviceMap();
            auto parameterPairs = functorList.functor->allParameters();
            for (auto parameterPair : parameterPairs) {
                auto deviceId = parameterPair.first;
                for (auto parameter : parameterPair.second) {
                    auto parameterId = parameter.id;
                    auto sourceParameter = parameterMap[deviceId][parameterId];
                    Parameter p;
                    p.begin = begin(sourceParameter.buffer);
                    p.end = end(sourceParameter.buffer);
                    (*inputParameterMap)[deviceId][parameterId] = p;
                }
            }
            return unique_ptr<ControlFunctor::ParameterDeviceMap>(inputParameterMap);
        };

        void distributeParametersAlongEdges(IndexType vertexId);
        std::vector<IndexType> findChildren(ControlEdgeList edgeList);
        std::vector<IndexType> findChildren(AudioEdgeList edgeList);
        std::vector<IndexType> findControlDeviceChildren(
                std::vector<IndexType>::iterator begin, std::vector<IndexType>::iterator end);
        std::vector<IndexType> findAudioDeviceChildren(
                std::vector<IndexType>::iterator begin, std::vector<IndexType>::iterator end);
        std::vector<IndexType> findAudioDevicePredecessors(IndexType vertexId);
    };

}

#endif //INSTRUMENT_TOOL_KIT_INSTANCEIMPLEMENTATION_H

#ifndef INSTRUMENT_TOOL_KIT_INSTANCEBUILDER_H
#define INSTRUMENT_TOOL_KIT_INSTANCEBUILDER_H

#include <processing/instance/Instance.h>
#include <device_graph/DeviceGraph.h>
#include <parameter_table/ParameterTable.h>

#include "../../processing/instance/InstanceImplementation.h"


namespace itk {

template <class Graph>
class InstanceBuilder {
public:
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

private:
    DeviceGraph &_deviceGraph;
    Graph &_graph;
    ParameterTable &_parameterTable;
};

}

#endif //INSTRUMENT_TOOL_KIT_INSTANCEBUILDER_H

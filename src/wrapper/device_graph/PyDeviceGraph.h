#ifndef INSTRUMENT_TOOL_KIT_PYDEVICEGRAPH_H
#define INSTRUMENT_TOOL_KIT_PYDEVICEGRAPH_H

#include <boost/python/object.hpp>

#include <device_graph/DeviceGraph.h>

namespace itk { namespace wrapper {

    class PyDeviceGraph {
    public:
        PyDeviceGraph(const DeviceGraph::Ptr &_deviceGraph)
                : _deviceGraph(_deviceGraph) { }

        IndexType addDevice(boost::python::object pyDevice);
    private:
        DeviceGraph::Ptr _deviceGraph;
    };

}}

#endif //INSTRUMENT_TOOL_KIT_PYDEVICEGRAPH_H

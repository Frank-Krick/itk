//
// Created by Frank Krick on 9/3/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICEGRAPHFACTORY_H
#define INSTRUMENT_TOOL_KIT_DEVICEGRAPHFACTORY_H

#include <device_graph/DeviceGraph.h>

namespace itk {

    class DeviceGraphFactory {
    public:
        static DeviceGraph::Ptr createDeviceGraph();
    };

}

#endif //INSTRUMENT_TOOL_KIT_DEVICEGRAPHFACTORY_H

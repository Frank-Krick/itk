//
// Created by Frank Krick on 9/3/15.
//

#include <device_graph/DeviceGraphFactory.h>

#include "DeviceGraphImplementation.h"

namespace itk {

    DeviceGraph::Ptr DeviceGraphFactory::createDeviceGraph() {
        return DeviceGraph::Ptr((DeviceGraph *)new DeviceGraphImplementation());
    }

}

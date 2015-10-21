#include <device_graph/DeviceGraphFactory.h>

#include "DeviceGraphImplementation.h"

namespace itk {

DeviceGraph::Ptr DeviceGraphFactory::createDeviceGraph() {
    return std::make_shared<DeviceGraphImplementation>();
}

}

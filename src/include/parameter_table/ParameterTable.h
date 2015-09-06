//
// Created by Frank Krick on 9/4/15.
//

#ifndef INSTRUMENT_TOOL_KIT_PARAMETERTABLE_H
#define INSTRUMENT_TOOL_KIT_PARAMETERTABLE_H

#include <Typedef.h>
#include <device/Device.h>
#include <device_graph/DeviceGraph.h>

namespace itk {

    class ParameterTable {
    public:
        typedef std::shared_ptr<ParameterTable> Ptr;
        typedef IndexType IndexType;

        virtual void registerDeviceParameters(IndexType deviceId, Device &device) = 0;
        virtual void unregisterDeviceParameters(IndexType deviceId) = 0;
    };

}

#endif //INSTRUMENT_TOOL_KIT_PARAMETERTABLE_H

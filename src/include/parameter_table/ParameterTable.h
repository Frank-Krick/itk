#ifndef INSTRUMENT_TOOL_KIT_PARAMETERTABLE_H
#define INSTRUMENT_TOOL_KIT_PARAMETERTABLE_H

#include <Typedef.h>
#include <device/Device.h>
#include <device_graph/DeviceGraph.h>

namespace itk {

class ParameterTable {
public:
    typedef std::shared_ptr<ParameterTable> Ptr;

    virtual void registerDeviceParameters(IndexType deviceId, Device &device) = 0;
    virtual void unregisterDeviceParameters(IndexType deviceId) = 0;

    virtual bool hasParameter(IndexType deviceId, IndexType parameterId) = 0;
    virtual DataType parameterValue(IndexType deviceId, IndexType parameterId) = 0;
    virtual void parameterValue(IndexType deviceId, IndexType parameterId, DataType value) = 0;

    virtual ~ParameterTable();
};

}

#endif //INSTRUMENT_TOOL_KIT_PARAMETERTABLE_H

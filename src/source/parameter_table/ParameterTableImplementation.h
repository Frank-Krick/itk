//
// Created by Frank Krick on 9/4/15.
//

#ifndef INSTRUMENT_TOOL_KIT_PARAMETERTABLEIMPLEMENTATION_H
#define INSTRUMENT_TOOL_KIT_PARAMETERTABLEIMPLEMENTATION_H

#include <parameter_table/ParameterTable.h>
#include <parameter_description/ParameterDescription.h>
#include <unordered_map>
#include <device_graph/DeviceGraph.h>
#include <list>

namespace itk {

    class ParameterTableImplementation : public ParameterTable {
    public:
        virtual void registerDeviceParameters(IndexType deviceId, Device &device) override;
        virtual void unregisterDeviceParameters(IndexType deviceId) override;

        virtual bool hasParameter(IndexType deviceId, IndexType parameterId) override;
        virtual DataType getParameterValue(IndexType deviceId, IndexType parameterId) override;
        virtual void setParameterValue(IndexType deviceId, IndexType parameterId, DataType value) override;

    private:
        typedef std::list<ParameterDescription> ParameterList;
        typedef std::unordered_map<IndexType, ParameterList> ParameterTable;
        typedef std::unordered_map<IndexType, std::unordered_map<IndexType, DataType>> ValueMap;

        ParameterTable parameterTable;
        ValueMap valueMap;
    };

}

#endif //INSTRUMENT_TOOL_KIT_PARAMETERTABLEIMPLEMENTATION_H

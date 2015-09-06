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

    private:
        typedef std::list<ParameterDescription> ParameterList;
        typedef std::unordered_map<IndexType, ParameterList> ParameterTable;

        ParameterTable parameterTable;
    };

}

#endif //INSTRUMENT_TOOL_KIT_PARAMETERTABLEIMPLEMENTATION_H

//
// Created by Frank Krick on 9/2/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICEGRAPH_H
#define INSTRUMENT_TOOL_KIT_DEVICEGRAPH_H

#include <Typedef.h>
#include <device/Device.h>
#include <parameter_table/ParameterTable.h>
#include "DeviceGraphInstance.h"

#include <memory>
#include <string>

namespace itk {

    enum class ConnectionType { AUDIO, CONTROL };

    std::ostream & operator << (std::ostream & stream, const ConnectionType & type);

    class DeviceGraph {
    public:

        typedef std::shared_ptr<DeviceGraph> Ptr;

        struct DeviceDescription {
            std::string name;
            IndexType deviceId = 0;
        };

        virtual ~DeviceGraph() {};

        virtual IndexType addDevice(Device::Ptr device) = 0;
        virtual void removeDevice(int deviceId) = 0;

        /*
         * Methods to manage connections between the devices.
         */
        virtual void connect(IndexType sourceId, IndexType targetId) = 0;
        virtual void connect(IndexType sourceId, IndexType targetId, IndexType parameterId) = 0;
        virtual void disconnect(IndexType sourceId, IndexType targetId) = 0;
        virtual bool isConnected(IndexType sourceId, IndexType targetId) = 0;
        virtual bool isConnected(IndexType sourceId, IndexType targetTd, IndexType parameterId) = 0;

        virtual DeviceGraphInstance::Ptr createInstance() = 0;
        virtual bool isInstanceUpToDate(DeviceGraphInstance &instance) = 0;

        virtual DeviceDescription describeDevice(IndexType deviceId) = 0;
    };

}


#endif //INSTRUMENT_TOOL_KIT_DEVICEGRAPH_H

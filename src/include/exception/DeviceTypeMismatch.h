//
// Created by Frank Krick on 9/5/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICETYPEMISMATCH_H
#define INSTRUMENT_TOOL_KIT_DEVICETYPEMISMATCH_H

#include <device/Device.h>
#include <device_graph/DeviceGraph.h>

#include <exception>

namespace itk {

    class DeviceTypeMismatch : std::exception {
    public:

        DeviceTypeMismatch(
                DeviceType sourceType,
                DeviceType targetType,
                ConnectionType connectionType);

        DeviceType getSourceType();
        DeviceType getTargetType();
        ConnectionType getConnectionType();

        virtual const char *what() const noexcept override;

    private:
        DeviceType sourceType = DeviceType::AUDIO;
        DeviceType targetType = DeviceType::AUDIO;
        ConnectionType connectionType = ConnectionType::CONTROL;

    };

}

#endif //INSTRUMENT_TOOL_KIT_DEVICETYPEMISMATCH_H

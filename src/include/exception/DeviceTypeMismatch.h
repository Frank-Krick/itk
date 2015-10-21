#ifndef INSTRUMENT_TOOL_KIT_DEVICETYPEMISMATCH_H
#define INSTRUMENT_TOOL_KIT_DEVICETYPEMISMATCH_H

#include <device/Device.h>
#include <device_graph/DeviceGraph.h>

#include <exception>

namespace itk {

class DeviceTypeMismatch : std::exception {
public:
    DeviceTypeMismatch(DeviceType sourceType, DeviceType targetType, ConnectionType connectionType)
            : _sourceType(sourceType), _targetType(targetType), _connectionType(connectionType) {}

    DeviceType sourceType();
    DeviceType targetType();
    ConnectionType connectionType();

    virtual const char *what() const noexcept override;

private:
    DeviceType _sourceType;
    DeviceType _targetType;
    ConnectionType _connectionType;
};

}

#endif //INSTRUMENT_TOOL_KIT_DEVICETYPEMISMATCH_H

//
// Created by Frank Krick on 9/6/15.
//

#include <exception/DeviceParameterMissing.h>
#include <sstream>

namespace itk {

    DeviceParameterMissing::DeviceParameterMissing(IndexType deviceId, IndexType parameterId) {
        this->deviceId = deviceId;
        this->parameterId = parameterId;
    }

    IndexType DeviceParameterMissing::getParameterId() {
        return parameterId;
    }

    IndexType DeviceParameterMissing::getDeviceId() {
        return deviceId;
    }

    const char *DeviceParameterMissing::what() const noexcept {
        std::stringstream stream;
        stream << "Device with id " << deviceId << " doesn't have a parameter with id " << deviceId;
        return stream.str().c_str();
    }

}

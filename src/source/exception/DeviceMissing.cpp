//
// Created by Frank Krick on 9/6/15.
//

#include <exception/DeviceMissing.h>

#include <sstream>

namespace itk {

    DeviceMissing::DeviceMissing(IndexType deviceId) {
        this->deviceId = deviceId;
    }

    IndexType DeviceMissing::getDeviceId() {
        return deviceId;
    }

    const char *DeviceMissing::what() const noexcept {
        std::stringstream stream;
        stream << "Device with id " << deviceId << " doesn't exist.";
        return stream.str().c_str();
    }

}
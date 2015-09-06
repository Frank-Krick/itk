//
// Created by Frank Krick on 9/5/15.
//

#include <exception/DeviceTypeMismatch.h>

#include <sstream>

namespace itk {

    using namespace std;

    DeviceTypeMismatch::DeviceTypeMismatch(
            DeviceType sourceType,
            DeviceType targetType,
            ConnectionType connectionType) {

    }

    DeviceType DeviceTypeMismatch::getSourceType() {
        return sourceType;
    }

    DeviceType DeviceTypeMismatch::getTargetType() {
        return targetType;
    }

    ConnectionType DeviceTypeMismatch::getConnectionType() {
        return connectionType;
    }

    const char *DeviceTypeMismatch::what() const noexcept {
        stringstream stream;
        stream << "Cannot connect the source device of type " << sourceType << " to the target device of " <<
        targetType << " using a connection of type " << connectionType;
        return stream.str().c_str();
    }

}

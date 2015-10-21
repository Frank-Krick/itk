//
// Created by Frank Krick on 9/5/15.
//

#include <exception/DeviceTypeMismatch.h>

#include <sstream>

namespace itk {

using namespace std;

DeviceType DeviceTypeMismatch::sourceType() {
    return _sourceType;
}

DeviceType DeviceTypeMismatch::targetType() {
    return _targetType;
}

ConnectionType DeviceTypeMismatch::connectionType() {
    return _connectionType;
}

const char *DeviceTypeMismatch::what() const noexcept {
    stringstream stream;
    stream << "Cannot connect the source device of type " << _sourceType << " to the target device of "
        << _targetType << " using a connection of type " << _connectionType;
    return stream.str().c_str();
}

}

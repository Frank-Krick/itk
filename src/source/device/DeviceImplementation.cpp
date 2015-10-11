//
// Created by Frank Krick on 9/4/15.
//

#include "DeviceImplementation.h"

namespace itk {

    using namespace std;

    tuple<Device::Iter, Device::Iter> DeviceImplementation::availableParameters() {
        return tuple<Iter, Iter>(_parameters.begin(), _parameters.end());
    }

    void DeviceImplementation::deviceType(DeviceType type) {
        this->_deviceType = type;
    }

    void DeviceImplementation::functor(Functor::Ptr functor) {
        this->_functor = functor;
        _parameters = functor->parameterList();
    }

    Functor::Ptr DeviceImplementation::functor(IndexType deviceId) {
        return _functor->clone(deviceId);
    }


    void DeviceImplementation::name(std::string name) {
        _name = name;
    }

    void DeviceImplementation::description(std::string description) {
        _description = description;
    }
}
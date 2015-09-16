//
// Created by Frank Krick on 9/4/15.
//

#include "DeviceImplementation.h"

namespace itk {

    using namespace std;

    tuple<Device::Iter, Device::Iter> DeviceImplementation::availableParameters() {
        return tuple<Iter, Iter>(parameters.begin(), parameters.end());
    }

    DeviceType DeviceImplementation::deviceType() {
        return type;
    }

    void DeviceImplementation::deviceType(DeviceType type) {
        this->type = type;
    }

    void DeviceImplementation::setFunctor(Functor::Ptr functor) {
        this->functor = functor;
        parameters = functor->parameterList();
    }

}
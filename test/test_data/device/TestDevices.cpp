//
// Created by Frank Krick on 9/12/15.
//

#include "TestDevices.h"
#include "ParameterCopyAudioFunctor.h"
#include <device/DeviceFactory.h>
#include <sstream>
#include <processing/generator/Functor.h>
#include "../../src/source/device/DeviceImplementation.h"
#include "InputCopyAudioFunctor.h"
#include "InputAddAudioFunctor.h"
#include "ConstantAddAudioFunctor.h"

using std::stringstream;

Device::Ptr TestDevices::parameterCopyDevice(IndexType numParameters,
                                             IndexType sourceParameterId) {

    auto device = std::make_shared<DeviceImplementation>();
    auto functor = std::make_shared<ParameterCopyAudioFunctor>(0, numParameters, sourceParameterId, 44100);
    device->functor(functor);
    device->deviceType(DeviceType::AUDIO);
    return device;
}

Device::Ptr TestDevices::inputCopyDevice() {
    auto device = std::make_shared<DeviceImplementation>();
    auto functor = std::make_shared<InputCopyAudioFunctor>(0, 44100);
    device->functor(functor);
    device->deviceType(DeviceType::CONTROL);
    return device;
}

Device::Ptr TestDevices::inputAddDevice() {
    auto device = std::make_shared<DeviceImplementation>();
    auto functor = std::make_shared<InputAddAudioFunctor>(0, 44100);
    device->functor(functor);
    device->deviceType(DeviceType::AUDIO);
    return device;
}

Device::Ptr TestDevices::constantAddDevice(DataType constant) {
    auto device = std::make_shared<DeviceImplementation>();
    auto functor = std::make_shared<ConstantAddAudioFunctor>(0, 44100, constant);
    device->functor(functor);
    device->deviceType(DeviceType::AUDIO);
    return device;
}
//
// Created by Frank Krick on 9/12/15.
//

#include "TestDevices.h"
#include "ParameterCopyAudioFunctor.h"

#include <device/DeviceFactory.h>

#include <sstream>
#include <processing/generator/Functor.h>
#include "../../src/source/device/DeviceImplementation.h"

using std::stringstream;

Device::Ptr TestDevices::createParameterCopyDevice(IndexType numParameters,
                                                   IndexType sourceParameterId) {

    auto device = new DeviceImplementation();
    auto functor = new ParameterCopyAudioFunctor(0, 5, 2, 44100);
    auto deviceFunctor = Functor::Ptr((Functor *)functor);
    device->setFunctor(deviceFunctor);
    device->deviceType(DeviceType::AUDIO);
    return Device::Ptr((Device *)device);
}

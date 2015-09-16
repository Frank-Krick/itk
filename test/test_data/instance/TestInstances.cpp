//
// Created by Frank Krick on 9/13/15.
//

#include "TestInstances.h"

#include "../../src/source/parameter_table/ParameterTableImplementation.h"
#include "../device/TestFunctors.h"
#include "../device/TestFunctorLists.h"
#include "../device/TestDevices.h"

std::pair<ParameterTable::Ptr, Instance::Ptr> TestInstances::createSimpleTestInstance(unsigned int bufferSize) {
    auto parameterTable = new ParameterTableImplementation();
    auto parameterCopyDevice = TestDevices::createParameterCopyDevice(3, 1);
    parameterTable->registerDeviceParameters(0, *parameterCopyDevice);
    parameterTable->registerDeviceParameters(1, *parameterCopyDevice);
    parameterTable->registerDeviceParameters(2, *parameterCopyDevice);
    auto parameterCopyFunctorOne = TestFunctors::createParameterCopyFunctor(0, 3, 1, 44100);
    auto parameterCopyFunctorTwo = TestFunctors::createParameterCopyFunctor(1, 3, 1, 44100);
    auto parameterCopyFunctorThree = TestFunctors::createParameterCopyFunctor(2, 3, 2, 44100);
    auto testFunctor = TestFunctorLists::audioFunctorList(
            {parameterCopyFunctorOne, parameterCopyFunctorTwo, parameterCopyFunctorThree});

    auto instance = new InstanceImplementation(*parameterTable);
    auto functorId = instance->addAudioFunctorList(testFunctor);
    instance->connectOutput(functorId);
    instance->bufferSize(bufferSize);
    instance->allocateMemory();
    return std::pair<ParameterTable::Ptr, Instance::Ptr>(
            ParameterTable::Ptr((ParameterTable *)parameterTable),
            Instance::Ptr((Instance *)instance)
    );
}

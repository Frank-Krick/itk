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

    auto functorOne = parameterCopyDevice->functor(0);
    auto parameterCopyFunctorOne = std::dynamic_pointer_cast<AudioFunctor>(functorOne);

    auto functorTwo = parameterCopyDevice->functor(1);
    auto parameterCopyFunctorTwo = std::dynamic_pointer_cast<AudioFunctor>(functorTwo);

    auto functorThree = parameterCopyDevice->functor(2);
    auto parameterCopyFunctorThree = std::dynamic_pointer_cast<AudioFunctor>(functorThree);

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

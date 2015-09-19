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
    auto parameterCopyDevice = TestDevices::parameterCopyDevice(3, 1);
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

std::pair<ParameterTable::Ptr, Instance::Ptr> TestInstances::twoFunctorListInSeriesInstance(unsigned int bufferSize) {
    auto parameterTable = new ParameterTableImplementation();

    IndexType parameterCopyDeviceId = 0;
    auto parameterCopyDevice = TestDevices::parameterCopyDevice(1, 0);
    auto parameterCopyFunctor = parameterCopyDevice->functor(0);
    auto parameterCopyFunctorList = TestFunctorLists::audioFunctorList(
            {std::static_pointer_cast<AudioFunctor>(parameterCopyFunctor)});

    IndexType inputCopyDeviceId = 1;
    auto inputCopyDevice = TestDevices::inputCopyDevice();
    auto inputCopyFunctor = inputCopyDevice->functor(inputCopyDeviceId);
    auto inputCopyFunctorList = TestFunctorLists::audioFunctorList(
            {std::static_pointer_cast<AudioFunctor>(inputCopyFunctor)});

    parameterTable->registerDeviceParameters(
            parameterCopyDeviceId, *parameterCopyDevice);

    parameterTable->registerDeviceParameters(
            inputCopyDeviceId, *inputCopyDevice);

    auto instance = new InstanceImplementation(*parameterTable);
    auto parameterCopyFunctorListId = instance->addAudioFunctorList(parameterCopyFunctorList);
    auto inputCopyFunctorListId = instance->addAudioFunctorList(inputCopyFunctorList);
    instance->connectOutput(inputCopyFunctorListId);
    instance->connect(parameterCopyFunctorListId, inputCopyFunctorListId);
    instance->bufferSize(bufferSize);
    instance->allocateMemory();

    return std::pair<ParameterTable::Ptr, Instance::Ptr>(
            ParameterTable::Ptr((ParameterTable *)parameterTable),
            Instance::Ptr((Instance *)instance));
}

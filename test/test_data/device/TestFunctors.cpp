//
// Created by Frank Krick on 9/15/15.
//

#include "TestFunctors.h"

#include "../device/ParameterCopyAudioFunctor.h"

AudioFunctor::Ptr TestFunctors::createParameterCopyFunctor(IndexType deviceId, IndexType numParameters,
                                                           IndexType parameterId, unsigned int sampleRate) {

    auto functor = new ParameterCopyAudioFunctor(deviceId, numParameters, parameterId, sampleRate);
    return AudioFunctor::Ptr((AudioFunctor *)functor);
}

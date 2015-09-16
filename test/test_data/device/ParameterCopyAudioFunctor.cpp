//
// Created by Frank Krick on 9/12/15.
//

#include "ParameterCopyAudioFunctor.h"

#include "TestParameter.h"

#include <boost/assert.hpp>

void ParameterCopyAudioFunctor::operator()(InputChannels beginIn, InputChannels endIn,
                                           OutputChannels beginOut, OutputChannels endOut,
                                           ParameterMap &parameter) {

    auto pi = parameter[sourceParameterId].begin;
    auto li_end = endOut[0];
    auto li = beginOut[0];
    auto ri = beginOut[1];
    while (li != li_end) {
        *li = *pi;
        *ri = *pi;
        ++li;
        ++ri;
        ++pi;
    }
}

bool ParameterCopyAudioFunctor::operator==(const AudioFunctor &functor) {
    if (typeid(functor) == typeid(*this)) {
        auto f = (ParameterCopyAudioFunctor &) functor;
        return sampleRate == f.sampleRate &&
               numParameters == f.numParameters &&
               sourceParameterId == f.sourceParameterId;
    } else {
        return false;
    }
}

ParameterList ParameterCopyAudioFunctor::parameterList() {
    ParameterList parameterList;
    for (int i = 0; i < numParameters; ++i) {
        auto parameter = TestParameter::testParameter(i);
        parameterList.push_back(parameter);
    }
    return parameterList;
}

AudioFunctor::Ptr ParameterCopyAudioFunctor::clone() {
    auto clone = new ParameterCopyAudioFunctor(0, numParameters, sourceParameterId, sampleRate);
    return AudioFunctor::Ptr((AudioFunctor *)clone);
}

void ParameterCopyAudioFunctor::reset() {}

ParameterCopyAudioFunctor::ParameterCopyAudioFunctor(
        IndexType deviceId, IndexType numParameters,
        IndexType sourceParameterId, unsigned int sampleRate)
            : AudioFunctor(deviceId, sampleRate),
              sourceParameterId(sourceParameterId),
              numParameters(numParameters) {

    BOOST_ASSERT(sourceParameterId < numParameters);
}

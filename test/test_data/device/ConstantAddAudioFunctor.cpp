#include "ConstantAddAudioFunctor.h"

#include <algorithm>


void ConstantAddAudioFunctor::operator()(InputChannels beginIn, InputChannels endIn, OutputChannels beginOut,
                                         OutputChannels endOut, ParameterMap &parameter) {

    std::transform(beginIn[0], endIn[0], beginOut[0],
                   [this](DataType value) -> DataType { return value + _constant; });

    std::transform(beginIn[1], endIn[1], beginOut[1],
                   [this](DataType value) -> DataType { return value + _constant; });
}

bool ConstantAddAudioFunctor::operator==(const AudioFunctor &functor) {
    return false;
}

ParameterList ConstantAddAudioFunctor::parameterList() {
    return std::list<ParameterDescription>();
}

std::shared_ptr<AudioFunctor> ConstantAddAudioFunctor::clone() {
    return std::make_shared<ConstantAddAudioFunctor>(_deviceId, _sampleRate, _constant);
}

Functor::Ptr ConstantAddAudioFunctor::clone(IndexType deviceId) {
    return std::make_shared<ConstantAddAudioFunctor>(_deviceId, _sampleRate, _constant);
}

void ConstantAddAudioFunctor::reset() {}

ConstantAddAudioFunctor::ConstantAddAudioFunctor(IndexType deviceId, unsigned int sampleRate, DataType constant)
        : AudioFunctor(deviceId, sampleRate), _constant(constant) {}

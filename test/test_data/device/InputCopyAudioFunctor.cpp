//
// Created by Frank Krick on 9/16/15.
//

#include "InputCopyAudioFunctor.h"

void InputCopyAudioFunctor::operator()(InputChannels beginIn, InputChannels endIn, OutputChannels beginOut,
                                       OutputChannels endOut, ParameterMap &parameter) {

    using std::copy;
    using std::end;
    using std::begin;
    copy(beginIn[0], endIn[0], beginOut[0]);
    copy(beginIn[1], endIn[1], beginOut[1]);
}

bool InputCopyAudioFunctor::operator==(const AudioFunctor &functor) {
    return true;
}

ParameterList InputCopyAudioFunctor::parameterList() {
    return std::list<ParameterDescription>();
}

std::shared_ptr<AudioFunctor> InputCopyAudioFunctor::clone() {
    auto functor = new InputCopyAudioFunctor(_deviceId, _sampleRate);
    return std::shared_ptr<AudioFunctor>((AudioFunctor *)functor);
}

void InputCopyAudioFunctor::reset() {}

InputCopyAudioFunctor::InputCopyAudioFunctor(IndexType deviceId, unsigned int sampleRate)
        : AudioFunctor(deviceId, sampleRate) {}

Functor::Ptr InputCopyAudioFunctor::clone(IndexType deviceId) {
    auto functor = new InputCopyAudioFunctor(deviceId, _sampleRate);
    return std::shared_ptr<Functor>((Functor *)functor);
}

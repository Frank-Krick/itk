#include "InputAddAudioFunctor.h"

void InputAddAudioFunctor::operator()(InputChannels beginIn, InputChannels endIn, OutputChannels beginOut,
                                      OutputChannels endOut, ParameterMap &parameter) {

    auto outputLeft = beginOut[0];
    auto outputRight = beginOut[1];
    auto inputChannels = beginIn;
    while (outputLeft != endOut[0] && outputRight != endOut[1]) {
        DataType value(0.0);
        for (auto it = std::begin(inputChannels); it != std::end(inputChannels); ++it) {
            value += *(*it);
            ++(*it);
        }
        *outputLeft = value;
        *outputRight = value;
        ++outputLeft;
        ++outputRight;
    }
}

bool InputAddAudioFunctor::operator==(const AudioFunctor &functor) {
    return true;
}

ParameterList InputAddAudioFunctor::parameterList() {
    return std::list<ParameterDescription>();
}

std::shared_ptr<AudioFunctor> InputAddAudioFunctor::clone() {
    return std::make_shared<InputAddAudioFunctor>(_deviceId, _sampleRate);
}

Functor::Ptr InputAddAudioFunctor::clone(IndexType deviceId) {
    return std::make_shared<InputAddAudioFunctor>(deviceId, _sampleRate);
}

void InputAddAudioFunctor::reset() {}

InputAddAudioFunctor::InputAddAudioFunctor(IndexType deviceId, unsigned int sampleRate)
        : AudioFunctor(deviceId, sampleRate) {
}

#include "SineGenerator.h"

#include <cmath>

namespace itk {

const auto tau = 2 * M_PI;

void SineGenerator::operator () (
        InputChannels beginIn, InputChannels endIn,
        OutputChannels beginOut, OutputChannels endOut,
        ParameterMap & parameter) {

    auto left = beginOut[0];
    auto right = beginOut[1];
    auto end = endOut[0];

    auto frequencyIterator = parameter[0].begin;
    while (left != end) {
        auto f = *frequencyIterator;
        *left = sin(tau * f * phaseOffset);
        *right = *left;
        phaseOffset++;
        frequencyIterator++;
        left++;
        right++;
    }
}

ParameterList SineGenerator::parameterList() {
    auto frequency = ParameterDescription(0, "Frequency", "Frequency of the Generator", 1, 22000);

    ParameterList parameterList { frequency };
    return parameterList;
}

void SineGenerator::reset() {
    phaseOffset = 0;
}

AudioFunctor::Ptr SineGenerator::clone() {
    auto generator = std::make_shared<SineGenerator>(0, _sampleRate);
    generator->phaseOffset = phaseOffset;
    return generator;
}

SineGenerator::SineGenerator(IndexType deviceId, unsigned int sampleRate)
        : AudioFunctor(deviceId, sampleRate) {}

bool SineGenerator::operator == (const AudioFunctor & generator) {
    if (typeid(generator) == typeid(*this)) {
        auto sineGenerator = dynamic_cast<const SineGenerator &>(generator);
        return std::abs(phaseOffset - sineGenerator.phaseOffset) < 0.001 &&
               _sampleRate == sineGenerator._sampleRate;
    }
    return false;
}

Functor::Ptr SineGenerator::clone(IndexType deviceId) {
    auto generator = std::make_shared<SineGenerator>(deviceId, _sampleRate);
    generator->phaseOffset = phaseOffset;
    return generator;
}

}

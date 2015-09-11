/*
 * Frank Krick
 */
#include "SineGenerator.h"

namespace itk {

    auto tau = 2 * M_PI;

    void SineGenerator::operator () (
            InputChannels beginIn, InputChannels endIn,
            OutputChannels beginOut, OutputChannels endOut,
            ParameterMap & parameter) {

        auto left = beginOut[0];
        auto right = beginOut[1];
        auto end = endOut[0];

        auto frequencyIterator = parameter[0].start;
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

    AudioFunctor::ParameterList SineGenerator::parameterList() {
        auto frequency = ParameterDescription();
        frequency.description = "Frequency of the Generator";
        frequency.id = 0;
        frequency.max = 22000;
        frequency.min = 1;
        frequency.name = "Frequency";

        ParameterList parameterList { frequency };
        return parameterList;
    }

    void SineGenerator::reset() {
        phaseOffset = 0;
    }

    AudioFunctor::Ptr SineGenerator::clone() {
        auto generator = new SineGenerator(sampleRate);
        generator->phaseOffset = phaseOffset;
        return AudioFunctor::Ptr((AudioFunctor *)generator);
    }

    AudioFunctor::Ptr SineGenerator::create() {
        auto generator = new SineGenerator(sampleRate);
        return AudioFunctor::Ptr((AudioFunctor *)generator);
    }

    SineGenerator::SineGenerator(unsigned int sampleRate) : AudioFunctor(sampleRate) {}

    bool SineGenerator::operator == (const AudioFunctor & generator) {
        if (typeid(generator) == typeid(*this))
            return phaseOffset == ((SineGenerator &)generator).phaseOffset;
        return false;
    }

}
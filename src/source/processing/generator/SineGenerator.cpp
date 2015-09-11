/*
 * Frank Krick
 */
#include "SineGenerator.h"

namespace itk {

    auto tau = 2 * M_PI;

    void SineGenerator::operator () (Generator::Iterator begin, Generator::Iterator end, ParameterMap &parameter) {
        auto frequencyIterator = parameter[0].start;
        for (auto it = begin; it != end; ++it) {
            auto f = *frequencyIterator;
            *it = sin(tau * f * phaseOffset);
            phaseOffset++;
            frequencyIterator++;
        }
    }

    Generator::ParameterList SineGenerator::parameterList() {
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

    Generator::Ptr SineGenerator::clone() {
        auto generator = new SineGenerator(sampleRate);
        generator->phaseOffset = phaseOffset;
        return Generator::Ptr((Generator *)generator);
    }

    Generator::Ptr SineGenerator::create() {
        auto generator = new SineGenerator(sampleRate);
        return Generator::Ptr((Generator *)generator);
    }

    SineGenerator::SineGenerator(unsigned int sampleRate) : Generator(sampleRate) {}

    bool SineGenerator::operator == (const Generator & generator) {
        if (typeid(generator) == typeid(*this))
            return phaseOffset == ((SineGenerator &)generator).phaseOffset;
        return false;
    }

}
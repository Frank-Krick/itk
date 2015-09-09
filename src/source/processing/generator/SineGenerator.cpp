//
// Created by Frank Krick on 9/7/15.
//

#include "SineGenerator.h"

namespace itk {


    void SineGenerator::operator()(Generator::Iterator begin, Generator::Iterator end, ParameterMap &parameter) {

    }

    Generator::ParameterList SineGenerator::parameterList() {
        return ParameterList();
    }

    Generator::Ptr SineGenerator::clone() {
        return Generator::Ptr((Generator *)new SineGenerator(sampleRate));
    }

    Generator::Ptr SineGenerator::create() {
        return Generator::Ptr((Generator *)new SineGenerator(sampleRate));
    }

    SineGenerator::SineGenerator(unsigned int sampleRate) : Generator(sampleRate) {}

    bool SineGenerator::operator==(const Generator &generator) {
        return false;
    }

    void SineGenerator::reset() {

    }
}
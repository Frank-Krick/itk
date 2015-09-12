#include <boost/test/unit_test.hpp>

#include <Utility.h>

#include "../../src/source/processing/generator/SineGenerator.h"

using namespace itk;

using itk::clone;
using std::begin;
using std::end;
using std::equal;

BOOST_AUTO_TEST_SUITE(SineGeneratorTests)

    BOOST_AUTO_TEST_CASE( two_new_sine_generator_should_be_equal ) {
        auto one = SineGenerator(44100);
        auto two = one.create();
        BOOST_CHECK(one == *two);
    }

    BOOST_AUTO_TEST_CASE( two_copies_of_a_sine_generator_should_be_equal ) {
        auto one = SineGenerator(44100);
        DataBuffer buffer = DataBuffer();
        buffer.push_back(0.0);
        ParameterMap parameterMap;
        Parameter parameter;
        parameter.id = 0;
        DataBuffer parameterBuffer;
        parameterBuffer.push_back(0.0);
        parameter.start = begin(parameterBuffer);
        parameterMap[0] = parameter;
        std::array<Iterator, 2> tmpBegin { begin(buffer), begin(buffer) };
        std::array<Iterator, 2> tmpEnd { end(buffer), end(buffer) };
        std::array<ConstIterator, 2> cTmpBegin { begin(buffer), begin(buffer) };
        std::array<ConstIterator, 2> cTmpEnd { end(buffer), end(buffer) };
        one(cTmpEnd, cTmpEnd, tmpBegin, tmpEnd, parameterMap);
        auto copyOne = clone(one);
        auto copyTwo = clone(one);
        BOOST_CHECK(*copyOne == *copyTwo);
    }

    BOOST_AUTO_TEST_CASE( two_sine_generators_should_not_be_equal_if_state_differs ) {
        auto one = SineGenerator(44100);
        auto two = clone(one);
        DataBuffer buffer = DataBuffer();
        buffer.push_back(0.0);
        ParameterMap parameterMap;
        Parameter parameter;
        parameter.id = 0;
        DataBuffer parameterBuffer;
        parameterBuffer.push_back(0.0);
        parameter.start = begin(parameterBuffer);
        parameterMap[0] = parameter;
        std::array<Iterator, 2> tmpBegin { begin(buffer), begin(buffer) };
        std::array<Iterator, 2> tmpEnd { end(buffer), end(buffer) };
        std::array<ConstIterator, 2> cTmpBegin { begin(buffer), begin(buffer) };
        std::array<ConstIterator, 2> cTmpEnd { end(buffer), end(buffer) };
        one(cTmpEnd, cTmpEnd, tmpBegin, tmpEnd, parameterMap);
        BOOST_CHECK(one != *two);
    }

    BOOST_AUTO_TEST_CASE( sine_generator_should_generate_sine_wave ) {
        auto audioData = DataBuffer(5000);
        auto controlData = DataBuffer(5000);
        for (auto it = begin(controlData); it != end(controlData); ++it) {
            *it = 60.0;
        }
        ParameterMap parameterMap;
        Parameter parameter;
        parameter.id = 0;
        parameter.start = begin(controlData);
        parameterMap[0] = parameter;
        auto one = SineGenerator(44100);
        std::array<Iterator, 2> tmpBegin { begin(audioData), begin(audioData) };
        std::array<Iterator, 2> tmpEnd { end(audioData), end(audioData) };
        std::array<ConstIterator, 2> cTmpEnd { end(audioData), end(audioData) };
        one(cTmpEnd, cTmpEnd, tmpBegin, tmpEnd, parameterMap);
        auto checkData = DataBuffer(5000);
        int phaseOffset = 0;
        for (auto it = begin(checkData); it != end(checkData); ++it) {
            double tau = 2 * M_PI;
            double f = 60;
            *it = sin(tau * f * phaseOffset);
            ++phaseOffset;
        }
        BOOST_CHECK(equal(
                begin(audioData), end(audioData),
                begin(checkData), end(checkData)));
    }

BOOST_AUTO_TEST_SUITE_END()
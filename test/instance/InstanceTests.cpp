#include <boost/test/unit_test.hpp>
#include <iostream>

#include "../../src/source/processing/instance/InstanceImplementation.h"
#include "../test_data/instance/TestInstances.h"

using namespace itk;

using std::begin;
using std::end;
using std::cout;
using std::equal;

BOOST_AUTO_TEST_SUITE( InstanceTests )

    BOOST_AUTO_TEST_CASE( no_tests_implemented ) {
        int bufferSize = 35;
        auto temp = TestInstances::createSimpleTestInstance(bufferSize);
        auto parameters = temp.first;
        auto instance = temp.second;
        parameters->setParameterValue(0, 0, 1.0);
        parameters->setParameterValue(0, 1, 1.0);
        parameters->setParameterValue(0, 2, 1.0);
        parameters->setParameterValue(1, 0, 2.0);
        parameters->setParameterValue(1, 1, 2.0);
        parameters->setParameterValue(1, 2, 2.0);
        parameters->setParameterValue(2, 0, 3.0);
        parameters->setParameterValue(2, 1, 3.0);
        parameters->setParameterValue(2, 2, 3.0);
        auto left = DataBuffer(bufferSize);
        auto right = DataBuffer(bufferSize);
        OutputChannels beginOut { begin(left), begin(right) };
        OutputChannels endOut { end(left), end(right) };
        (*instance)(beginOut, endOut);
        auto expected = DataBuffer(bufferSize);
        for (auto ex : expected) {
            ex = 3.0;
        }
        BOOST_CHECK(equal(begin(left), end(left), begin(expected)));
        for (auto f : left) {
            cout << f << " ";
        }
    }

BOOST_AUTO_TEST_SUITE_END()
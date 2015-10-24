#include <boost/test/unit_test.hpp>

#include <device_graph/DeviceGraphFactory.h>
#include <Typedef.h>
#include "../../test_data/device/TestDevices.h"


BOOST_AUTO_TEST_SUITE( InstanceBuilderTests )

using namespace itk;
using std::begin;
using std::end;


BOOST_AUTO_TEST_CASE( the_instance_builder_of_an_emtpy_graph_should_return_an_instance_that_writes_zero ) {
    auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
    unsigned int bufferSize = 56;
    auto instance = deviceGraph->createInstance(bufferSize);
    auto expectedLeft = DataBuffer(bufferSize);
    auto expectedRight = DataBuffer(bufferSize);
    auto actualLeft = DataBuffer(bufferSize);
    auto actualRight = DataBuffer(bufferSize);
    std::fill(begin(actualLeft), end(actualLeft), 5.0);
    std::fill(begin(actualRight), end(actualRight), 5.0);
    OutputChannels beginIt = {begin(actualLeft), begin(actualRight)};
    OutputChannels endIt = {end(actualLeft), end(actualRight)};
    (*instance)(beginIt, endIt);
    BOOST_CHECK(equal(begin(expectedLeft), end(expectedLeft), begin(actualLeft)));
    BOOST_CHECK(equal(begin(expectedRight), end(expectedRight), begin(actualRight)));
}

BOOST_AUTO_TEST_CASE( instance_builder_with_one_device ) {
    auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
    auto parameterCopyDevice = TestDevices::parameterCopyDevice(3, 1);
    auto deviceId = deviceGraph->addDevice(parameterCopyDevice);
    deviceGraph->parameterValue(deviceId, 1, 4.0);
    unsigned int bufferSize = 56;
    auto instance = deviceGraph->createInstance(bufferSize);
    auto expectedLeft = DataBuffer(bufferSize);
    auto expectedRight = DataBuffer(bufferSize);
    auto actualLeft = DataBuffer(bufferSize);
    auto actualRight = DataBuffer(bufferSize);
    std::fill(begin(expectedLeft), end(expectedLeft), 4.0);
    std::fill(begin(expectedRight), end(expectedRight), 4.0);
    OutputChannels beginIt = {begin(actualLeft), begin(actualRight)};
    OutputChannels endIt = {end(actualLeft), end(actualRight)};
    (*instance)(beginIt, endIt);
    BOOST_CHECK(equal(begin(expectedLeft), end(expectedLeft), begin(actualLeft)));
    BOOST_CHECK(equal(begin(expectedRight), end(expectedRight), begin(actualRight)));
}

BOOST_AUTO_TEST_SUITE_END()

#include <boost/test/unit_test.hpp>

#include <device_graph/DeviceGraphFactory.h>
#include "../../test_data/device/TestDevices.h"
#include "../../../src/source/device_graph/DeviceGraphImplementation.h"


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
    deviceGraph->outputDeviceId(deviceId);
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

BOOST_AUTO_TEST_CASE( instance_builder_with_two_devices ) {
    auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
    auto parameterCopyDevice = TestDevices::parameterCopyDevice(3, 1);
    auto deviceIdOne = deviceGraph->addDevice(parameterCopyDevice);
    deviceGraph->parameterValue(deviceIdOne, 1, 4.0);
    auto deviceIdTwo = deviceGraph->addDevice(parameterCopyDevice);
    deviceGraph->parameterValue(deviceIdTwo, 1, 3.0);
    deviceGraph->connect(deviceIdOne, deviceIdTwo);
    deviceGraph->outputDeviceId(deviceIdTwo);
    unsigned int bufferSize = 56;
    auto instance = deviceGraph->createInstance(bufferSize);
    auto expectedLeft = DataBuffer(bufferSize);
    auto expectedRight = DataBuffer(bufferSize);
    auto actualLeft = DataBuffer(bufferSize);
    auto actualRight = DataBuffer(bufferSize);
    std::fill(begin(expectedLeft), end(expectedLeft), 3.0);
    std::fill(begin(expectedRight), end(expectedRight), 3.0);
    OutputChannels beginIt = {begin(actualLeft), begin(actualRight)};
    OutputChannels endIt = {end(actualLeft), end(actualRight)};
    (*instance)(beginIt, endIt);
    BOOST_CHECK(equal(begin(expectedLeft), end(expectedLeft), begin(actualLeft)));
    BOOST_CHECK(equal(begin(expectedRight), end(expectedRight), begin(actualRight)));
}

BOOST_AUTO_TEST_CASE( instance_builder_for_small_device_tree ) {
    auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
    auto parameterCopyDevice = TestDevices::parameterCopyDevice(3, 1);
    auto deviceIdOne = deviceGraph->addDevice(parameterCopyDevice);
    deviceGraph->parameterValue(deviceIdOne, 1, 4.0);
    auto deviceIdTwo = deviceGraph->addDevice(parameterCopyDevice);
    deviceGraph->parameterValue(deviceIdTwo, 1, 3.0);
    auto inputAddDevice = TestDevices::inputAddDevice();
    auto deviceIdThree = deviceGraph->addDevice(inputAddDevice);
    deviceGraph->outputDeviceId(deviceIdThree);
    deviceGraph->connect(deviceIdOne, deviceIdThree);
    deviceGraph->connect(deviceIdTwo, deviceIdThree);
    unsigned int bufferSize = 78;
    auto instance = deviceGraph->createInstance(bufferSize);
    auto expectedLeft = DataBuffer(bufferSize);
    auto expectedRight = DataBuffer(bufferSize);
    std::fill(begin(expectedLeft), end(expectedLeft), 14.0);
    std::fill(begin(expectedRight), end(expectedRight), 14.0);
    auto actualLeft = DataBuffer(bufferSize);
    auto actualRight = DataBuffer(bufferSize);
    OutputChannels beginIt = {begin(actualLeft), begin(actualRight)};
    OutputChannels endIt = {end(actualLeft), end(actualRight)};
    (*instance)(beginIt, endIt);
    BOOST_CHECK(equal(begin(expectedLeft), end(expectedLeft), begin(actualLeft)));
    BOOST_CHECK(equal(begin(expectedRight), end(expectedRight), begin(actualRight)));
}

BOOST_AUTO_TEST_CASE( tree_of_height_three ) {
    auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
    auto parameterCopyDevice = TestDevices::parameterCopyDevice(3, 1);
    auto inputAddDevice = TestDevices::inputAddDevice();
    auto constantAddDevice = TestDevices::constantAddDevice(1);
    std::vector<IndexType> deviceIds {
            deviceGraph->addDevice(parameterCopyDevice),
            deviceGraph->addDevice(parameterCopyDevice),
            deviceGraph->addDevice(parameterCopyDevice),
            deviceGraph->addDevice(inputAddDevice),
            deviceGraph->addDevice(constantAddDevice),
            deviceGraph->addDevice(inputAddDevice),
    };
    deviceGraph->parameterValue(deviceIds[0], 1, 3.0);
    deviceGraph->parameterValue(deviceIds[1], 1, 1.0);
    deviceGraph->parameterValue(deviceIds[2], 1, 4.0);
    deviceGraph->connect(deviceIds[0], deviceIds[3]);
    deviceGraph->connect(deviceIds[1], deviceIds[3]);
    deviceGraph->connect(deviceIds[2], deviceIds[4]);
    deviceGraph->connect(deviceIds[3], deviceIds[5]);
    deviceGraph->connect(deviceIds[4], deviceIds[5]);
    deviceGraph->outputDeviceId(deviceIds[5]);
    unsigned int bufferSize = 78;
    auto instance = deviceGraph->createInstance(bufferSize);
    auto expectedLeft = DataBuffer(bufferSize);
    auto expectedRight = DataBuffer(bufferSize);
    std::fill(begin(expectedLeft), end(expectedLeft), 27.0);
    std::fill(begin(expectedRight), end(expectedRight), 27.0);
    auto actualLeft = DataBuffer(bufferSize);
    auto actualRight = DataBuffer(bufferSize);
    OutputChannels beginIt = {begin(actualLeft), begin(actualRight)};
    OutputChannels endIt = {end(actualLeft), end(actualRight)};
    (*instance)(beginIt, endIt);
    BOOST_CHECK(equal(begin(expectedLeft), end(expectedLeft), begin(actualLeft)));
    BOOST_CHECK(equal(begin(expectedRight), end(expectedRight), begin(actualRight)));
}

BOOST_AUTO_TEST_CASE( find_leafs_should_find_leafs_connected_to_output_device ) {
    auto deviceGraph = std::dynamic_pointer_cast<DeviceGraphImplementation>(DeviceGraphFactory::createDeviceGraph());
    auto parameterCopyDevice = TestDevices::parameterCopyDevice(3, 1);
    std::vector<IndexType> deviceIds = {
            deviceGraph->addDevice(parameterCopyDevice),
            deviceGraph->addDevice(parameterCopyDevice),
            deviceGraph->addDevice(parameterCopyDevice),
            deviceGraph->addDevice(parameterCopyDevice),
            deviceGraph->addDevice(parameterCopyDevice),
    };
    deviceGraph->connect(deviceIds[0], deviceIds[1]);
    deviceGraph->connect(deviceIds[1], deviceIds[2]);
    deviceGraph->connect(deviceIds[3], deviceIds[1]);
    deviceGraph->connect(deviceIds[4], deviceIds[3]);
    deviceGraph->connect(deviceIds[0], deviceIds[2]);
    deviceGraph->outputDeviceId(deviceIds[2]);
    auto actualLeafs = deviceGraph->_instanceBuilder->findLeafs();
    std::vector<DeviceGraphImplementation::Vertex> expectedLeafs = {
            deviceGraph->vertexFromDeviceId(deviceIds[0]),
            deviceGraph->vertexFromDeviceId(deviceIds[4]),
    };
    BOOST_CHECK(equal(begin(expectedLeafs), end(expectedLeafs), begin(actualLeafs)));
}

BOOST_AUTO_TEST_CASE( device_graph_with_one_audio_and_one_control_device ) {
    auto deviceGraph = std::dynamic_pointer_cast<DeviceGraphImplementation>(DeviceGraphFactory::createDeviceGraph());
    auto parameterCopyDevice = TestDevices::parameterCopyDevice(3, 1);
    auto rampControlDevice = TestDevices::rampControlDevice(30);
    auto parameterCopyDeviceId = deviceGraph->addDevice(parameterCopyDevice);
    auto rampControlDeviceId = deviceGraph->addDevice(rampControlDevice);
    deviceGraph->connect(rampControlDeviceId, parameterCopyDeviceId, 1);
    deviceGraph->outputDeviceId(parameterCopyDeviceId);
    unsigned int bufferSize = 78;
    auto instance = deviceGraph->createInstance(bufferSize);
    auto expectedLeft = DataBuffer(bufferSize);
    auto expectedRight = DataBuffer(bufferSize);
    std::fill(begin(expectedLeft), end(expectedLeft), 27.0);
    std::fill(begin(expectedRight), end(expectedRight), 27.0);
    auto actualLeft = DataBuffer(bufferSize);
    auto actualRight = DataBuffer(bufferSize);
    OutputChannels beginIt = {begin(actualLeft), begin(actualRight)};
    OutputChannels endIt = {end(actualLeft), end(actualRight)};
    (*instance)(beginIt, endIt);
    BOOST_CHECK(equal(begin(expectedLeft), end(expectedLeft), begin(actualLeft)));
    BOOST_CHECK(equal(begin(expectedRight), end(expectedRight), begin(actualRight)));
}

BOOST_AUTO_TEST_SUITE_END()

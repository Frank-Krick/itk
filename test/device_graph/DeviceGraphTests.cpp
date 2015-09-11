//
// Created by Frank Krick on 9/3/15.
//

#include <boost/test/unit_test.hpp>

#include <device/DeviceFactory.h>
#include <device_graph/DeviceGraphFactory.h>
#include <exception/DeviceTypeMismatch.h>
#include <exception/DeviceParameterMissing.h>

using namespace itk;

BOOST_AUTO_TEST_SUITE(DeviceGraphTests)

    auto device = DeviceFactory::createDevice((DeviceType::AUDIO));

    BOOST_AUTO_TEST_CASE( add_to_device_graph ) {
        auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
        auto deviceId = deviceGraph->addDevice(device);
        auto description = deviceGraph->describeDevice(deviceId);
        BOOST_CHECK(description.name == "device 0");
        BOOST_CHECK(description.deviceId == deviceId);
    }

    BOOST_AUTO_TEST_CASE( remove_from_device_graph ) {
        auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
        auto deviceId = deviceGraph->addDevice(device);
        deviceGraph->removeDevice(deviceId);
        BOOST_CHECK_THROW(deviceGraph->describeDevice(deviceId), std::out_of_range);
    }

    BOOST_AUTO_TEST_CASE( connect_audio_devices ) {
        auto sourceDevice = DeviceFactory::createDevice((DeviceType::AUDIO));
        auto targetDevice = DeviceFactory::createDevice((DeviceType::AUDIO));
        auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
        auto sourceId = deviceGraph->addDevice(sourceDevice);
        auto targetId = deviceGraph->addDevice(targetDevice);
        deviceGraph->connect(sourceId, targetId);
        BOOST_CHECK(deviceGraph->isConnected(sourceId, targetId));
        BOOST_CHECK(!deviceGraph->isConnected(targetId, sourceId));
    }

    BOOST_AUTO_TEST_CASE( connect_audio_devices_should_only_connect_audio_devices ) {
        auto sourceDevice = DeviceFactory::createDevice((DeviceType::AUDIO));
        auto targetDevice = DeviceFactory::createDevice((DeviceType::CONTROL));
        auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
        auto sourceId = deviceGraph->addDevice(sourceDevice);
        auto targetId = deviceGraph->addDevice(targetDevice);
        BOOST_CHECK_THROW(deviceGraph->connect(sourceId, targetId), DeviceTypeMismatch);
        BOOST_CHECK(!deviceGraph->isConnected(sourceId, targetId));
        BOOST_CHECK(!deviceGraph->isConnected(targetId, sourceId));
    }

    BOOST_AUTO_TEST_CASE( connect_control_device ) {
        auto sourceDevice = DeviceFactory::createDevice(DeviceType::CONTROL);
        auto targetDevice = DeviceFactory::createDevice(DeviceType::AUDIO);
        auto parameter = ParameterDescription();
        parameter.description = "Test";
        parameter.id = 3;
        parameter.name = "Param 3";
        targetDevice->addParameter(parameter);
        auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
        auto sourceId = deviceGraph->addDevice(sourceDevice);
        auto targetId = deviceGraph->addDevice(targetDevice);
        deviceGraph->connect(sourceId, targetId, 3);
        BOOST_CHECK(!deviceGraph->isConnected(sourceId, targetId));
        BOOST_CHECK(!deviceGraph->isConnected(targetId, sourceId));
        BOOST_CHECK(deviceGraph->isConnected(sourceId, targetId, parameter.id));
        BOOST_CHECK(!deviceGraph->isConnected(targetId, sourceId, parameter.id));
    }

    BOOST_AUTO_TEST_CASE( connect_control_devices_should_only_connect_control_source_devices ) {
        auto sourceDevice = DeviceFactory::createDevice(DeviceType::AUDIO);
        auto targetDevice = DeviceFactory::createDevice(DeviceType::AUDIO);
        auto parameter = ParameterDescription();
        parameter.description = "Test";
        parameter.id = 3;
        parameter.name = "Param 3";
        targetDevice->addParameter(parameter);
        auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
        auto sourceId = deviceGraph->addDevice(sourceDevice);
        auto targetId = deviceGraph->addDevice(targetDevice);
        BOOST_CHECK_THROW(deviceGraph->connect(sourceId, targetId, parameter.id), DeviceTypeMismatch);
    }

    BOOST_AUTO_TEST_CASE( connect_control_devices_should_only_connect_to_existing_parameters ) {
        auto sourceDevice = DeviceFactory::createDevice(DeviceType::CONTROL);
        auto targetDevice = DeviceFactory::createDevice(DeviceType::AUDIO);
        auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
        auto sourceId = deviceGraph->addDevice(sourceDevice);
        auto targetId = deviceGraph->addDevice(targetDevice);
        BOOST_CHECK_THROW(deviceGraph->connect(sourceId, targetId, 6), DeviceParameterMissing);
    }

BOOST_AUTO_TEST_SUITE_END()
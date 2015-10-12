//
// Created by Frank Krick on 9/3/15.
//

#include <boost/test/unit_test.hpp>

#include <device/DeviceFactory.h>
#include <device_graph/DeviceGraphFactory.h>
#include <exception/DeviceTypeMismatch.h>
#include <exception/DeviceParameterMissing.h>

#include "../test_data/device/TestDevices.h"

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
        auto targetDevice = TestDevices::parameterCopyDevice(5, 2);
        auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
        auto sourceId = deviceGraph->addDevice(sourceDevice);
        auto targetId = deviceGraph->addDevice(targetDevice);
        IndexType parameterId = 3;
        deviceGraph->connect(sourceId, targetId, parameterId);
        BOOST_CHECK(!deviceGraph->isConnected(sourceId, targetId));
        BOOST_CHECK(!deviceGraph->isConnected(targetId, sourceId));
        BOOST_CHECK(deviceGraph->isConnected(sourceId, targetId, parameterId));
        BOOST_CHECK(!deviceGraph->isConnected(targetId, sourceId, parameterId));
    }

    BOOST_AUTO_TEST_CASE( connect_control_devices_should_only_connect_control_source_devices ) {
        auto sourceDevice = DeviceFactory::createDevice(DeviceType::AUDIO);
        auto targetDevice = DeviceFactory::createDevice(DeviceType::AUDIO);
        IndexType parameterId = 2;
        auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
        auto sourceId = deviceGraph->addDevice(sourceDevice);
        auto targetId = deviceGraph->addDevice(targetDevice);
        BOOST_CHECK_THROW(deviceGraph->connect(sourceId, targetId, parameterId), DeviceTypeMismatch);
    }

    BOOST_AUTO_TEST_CASE( connect_control_devices_should_only_connect_to_existing_parameters ) {
        auto sourceDevice = DeviceFactory::createDevice(DeviceType::CONTROL);
        auto targetDevice = DeviceFactory::createDevice(DeviceType::AUDIO);
        auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
        auto sourceId = deviceGraph->addDevice(sourceDevice);
        auto targetId = deviceGraph->addDevice(targetDevice);
        BOOST_CHECK_THROW(deviceGraph->connect(sourceId, targetId, 6), DeviceParameterMissing);
    }

    BOOST_AUTO_TEST_CASE( return_device_descriptions ) {
        using std::sort;
        using std::begin;
        using std::end;
        using std::equal;

        std::vector<Device::Ptr> devices {
            DeviceFactory::createDevice((DeviceType::AUDIO)),
            DeviceFactory::createDevice((DeviceType::AUDIO)),
            DeviceFactory::createDevice((DeviceType::AUDIO)),
            DeviceFactory::createDevice((DeviceType::AUDIO)),
            DeviceFactory::createDevice((DeviceType::AUDIO))
        };

        auto deviceGraph = DeviceGraphFactory::createDeviceGraph();

        std::vector<IndexType> deviceIds;
        for (auto d : devices) {
            auto deviceId = deviceGraph->addDevice(d);
            deviceIds.push_back(deviceId);
        }

        auto descriptions = deviceGraph->devices();

        sort(begin(deviceIds), end(deviceIds));
        sort(begin(devices), end(devices));

        std::vector<IndexType> keys;
        for(auto d : descriptions) {
            keys.push_back(d.deviceId);
        }
        sort(begin(keys), end(keys));
        BOOST_CHECK_EQUAL(deviceIds.size(), devices.size());
        BOOST_CHECK(equal(begin(deviceIds), end(deviceIds), begin(keys), end(keys)));
    }

BOOST_AUTO_TEST_SUITE_END()
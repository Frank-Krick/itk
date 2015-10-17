#include <tuple>
#include <random>
#include <algorithm>
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
    auto description = deviceGraph->device(deviceId);
    BOOST_CHECK(description.name == "device 0");
    BOOST_CHECK(description.deviceId == deviceId);
}

BOOST_AUTO_TEST_CASE( remove_from_device_graph ) {
    auto deviceGraph = DeviceGraphFactory::createDeviceGraph();
    auto deviceId = deviceGraph->addDevice(device);
    deviceGraph->removeDevice(deviceId);
    BOOST_CHECK_THROW(deviceGraph->device(deviceId), std::out_of_range);
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

std::mt19937 rng;

struct TestScenario {
public:
    typedef std::vector<IndexType> IndexList;
    typedef std::tuple<IndexType, IndexType> AudioConnection;
    typedef std::tuple<IndexType, IndexType, IndexType> ControlConnection;
    typedef std::vector<AudioConnection> AudioConnections;
    typedef std::vector<ControlConnection> ControlConnections;

    Device::Ptr audioDevice = TestDevices::parameterCopyDevice(1, 0);
    Device::Ptr controlDevice = TestDevices::inputCopyDevice();
    DeviceGraph::Ptr deviceGraph = DeviceGraphFactory::createDeviceGraph();
    IndexList audioDeviceIds;
    IndexList controlDeviceIds;
    AudioConnections audioConnections;
    ControlConnections controlConnections;
    std::uniform_int_distribution<int> deviceIdDistribution;
    std::uniform_int_distribution<int> parameterIdDistribution;

    TestScenario(int n) : deviceIdDistribution(0, n - 1) {
        audioDeviceIds = addNumDevicesToGraph(n, audioDevice);
        controlDeviceIds = addNumDevicesToGraph(n, controlDevice);
        auto parameters = audioDevice->availableParameters();
        typename Device::Iter pBegin, pEnd;
        std::tie(pBegin, pEnd) = parameters;
        int numParameters = 0;
        for (auto it = pBegin; it != pEnd; ++it) {
            ++numParameters;
        }
        parameterIdDistribution = std::uniform_int_distribution<int>(0, numParameters - 1);
    }

    void addRandomAudioConnection() {
        auto connection = randomAudioConnection();
        IndexType source, target;
        std::tie(source, target) = connection;
        deviceGraph->connect(source, target);
    }

    void addRandomControlConnection() {
        auto connection = randomControlConnection();
        IndexType source, target, parameter;
        std::tie(source, target, parameter) = connection;
        deviceGraph->connect(source, target, parameter);
    }

private:
    IndexList addNumDevicesToGraph(int n, Device::Ptr device) {
        IndexList deviceIds;
        for (int i = 0; i < n; i++) {
            auto deviceId = deviceGraph->addDevice(device);
            deviceIds.push_back(deviceId);
        }
        return deviceIds;
    }

    AudioConnection randomAudioConnection() {
        auto source = audioDeviceIds[deviceIdDistribution(rng)];
        auto target = audioDeviceIds[deviceIdDistribution(rng)];
        auto connection = AudioConnection(source, target);

        bool doesNotExist = std::find(
                std::begin(audioConnections),
                std::end(audioConnections),
                connection) == std::end(audioConnections);

        if (doesNotExist) {
            audioConnections.push_back(connection);
            return connection;
        } else {
            return randomAudioConnection();
        }
    }

    ControlConnection randomControlConnection() {
        auto source = controlDeviceIds[deviceIdDistribution(rng)];
        auto target = audioDeviceIds[deviceIdDistribution(rng)];
        auto parameterId = parameterIdDistribution(rng);
        auto connection = ControlConnection(source, target, parameterId);

        bool doesNotExist = std::find(
                std::begin(controlConnections),
                std::end(controlConnections),
                connection) == std::end(controlConnections);

        if (doesNotExist) {
            controlConnections.push_back(connection);
            return connection;
        } else {
            return randomControlConnection();
        }
    }
};

BOOST_AUTO_TEST_CASE( audio_connections_should_return_all_audio_connections ) {
    TestScenario scenario(5);
    scenario.addRandomAudioConnection();
    scenario.addRandomAudioConnection();
    scenario.addRandomAudioConnection();
    scenario.addRandomAudioConnection();
    scenario.addRandomAudioConnection();
    scenario.addRandomControlConnection();
    scenario.addRandomControlConnection();
    scenario.addRandomControlConnection();
    scenario.addRandomControlConnection();
    scenario.addRandomControlConnection();
    scenario.addRandomControlConnection();
    scenario.addRandomControlConnection();
    auto actual = scenario.deviceGraph->audioConnections();
    auto expected = scenario.audioConnections;
    BOOST_CHECK_EQUAL(actual.size(), expected.size());
    for (auto connection : expected) {
        bool hasBeenReturned = [connection, actual](void) -> bool {
            IndexType source, target;
            std::tie(source, target) = connection;
            for (auto actualConnection : actual) {
                if (actualConnection.source == source && actualConnection.target == target) {
                    return true;
                }
            }
            return false;
        }();
        BOOST_CHECK(hasBeenReturned);
    }
}

BOOST_AUTO_TEST_CASE( control_connections_should_return_all_control_connections ) {
    TestScenario scenario(5);
    scenario.addRandomAudioConnection();
    scenario.addRandomAudioConnection();
    scenario.addRandomAudioConnection();
    scenario.addRandomAudioConnection();
    scenario.addRandomAudioConnection();
    scenario.addRandomControlConnection();
    scenario.addRandomControlConnection();
    scenario.addRandomControlConnection();
    scenario.addRandomControlConnection();
    scenario.addRandomControlConnection();
    scenario.addRandomControlConnection();
    scenario.addRandomControlConnection();
    auto actual = scenario.deviceGraph->controlConnections();
    auto expected = scenario.controlConnections;
    BOOST_CHECK_EQUAL(actual.size(), expected.size());
    for (auto connection : expected) {
        bool hasBeenReturned = [connection, actual](void) -> bool {
            IndexType source, target, parameter;
            std::tie(source, target, parameter) = connection;
            for (auto actualConnection : actual) {
                if (actualConnection.source == source &&
                    actualConnection.target == target &&
                    actualConnection.parameterId == parameter) {
                    return true;
                }
            }
            return false;
        }();
        BOOST_CHECK(hasBeenReturned);
    }
}

BOOST_AUTO_TEST_SUITE_END()
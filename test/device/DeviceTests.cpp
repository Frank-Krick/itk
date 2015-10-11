#include <boost/test/unit_test.hpp>

#include <device/Device.h>
#include <device/DeviceFactory.h>

#include "../test_data/device/ParameterCopyAudioFunctor.h"
#include "../../src/source/device/DeviceImplementation.h"

using namespace itk;
using namespace std;

BOOST_AUTO_TEST_SUITE( DeviceTests )

    bool compare_test(const ParameterDescription & lhs, const ParameterDescription & rhs) {
        return lhs.id() == rhs.id() &&
               lhs.description() == rhs.description() &&
               lhs.name() == rhs.name();
    }

    BOOST_AUTO_TEST_CASE( new_device_has_empty_parameter_table ) {
        auto device = DeviceFactory::createDevice((DeviceType::AUDIO));
        typename Device::Iter di, di_end;
        tie(di, di_end) = device->availableParameters();
        BOOST_CHECK(di == di_end);
    }

    BOOST_AUTO_TEST_CASE( add_parameter_from_functor ) {
        auto device = DeviceImplementation();
        auto functor = new ParameterCopyAudioFunctor(0, 5, 2, 44100);
        auto deviceFunctor = Functor::Ptr((Functor *)functor);
        device.functor(deviceFunctor);
        auto parameters = functor->parameterList();

        typename Device::Iter pi, pi_end;
        tie(pi, pi_end) = device.availableParameters();
        BOOST_REQUIRE(pi != pi_end);
        BOOST_CHECK(equal(begin(parameters), end(parameters), pi, compare_test));
    }

BOOST_AUTO_TEST_SUITE_END()
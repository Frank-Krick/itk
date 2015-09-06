#include <boost/test/unit_test.hpp>

#include <device/Device.h>
#include <device/DeviceFactory.h>

#include <algorithm>

using namespace itk;
using namespace std;

BOOST_AUTO_TEST_SUITE( DeviceTests )

    bool compare_test(const ParameterDescription & lhs, const ParameterDescription & rhs) {
        return lhs.id == rhs.id &&
               lhs.description == rhs.description &&
               lhs.name == rhs.name;
    }

    ParameterDescription create_parameter_test(IndexType id) {
        std::stringstream stream;
        auto parameter = ParameterDescription();
        stream << "para " << id;
        parameter.name = stream.str();
        stream.clear();
        parameter.description = "Test parameter";
        parameter.id = id;
        return parameter;
    }

    BOOST_AUTO_TEST_CASE( new_device_has_empty_parameter_table ) {
        auto device = DeviceFactory::createDevice((DeviceType::AUDIO));
        typename Device::Iter di, di_end;
        tie(di, di_end) = device->availableParameters();
        BOOST_CHECK(di == di_end);
    }

    BOOST_AUTO_TEST_CASE( add_parameter ) {
        auto device = DeviceFactory::createDevice((DeviceType::AUDIO));
        auto parameter = create_parameter_test(1);
        device->addParameter(parameter);
        typename Device::Iter pi, pi_end;
        tie(pi, pi_end) = device->availableParameters();
        BOOST_REQUIRE(pi != pi_end);
        BOOST_CHECK(compare_test(*pi, parameter));
    }

    bool all(const ParameterDescription & p) { return true; };

    BOOST_AUTO_TEST_CASE( multiple_parameter ) {
        std::list<ParameterDescription> parameter {
                create_parameter_test(0),
                create_parameter_test(1),
                create_parameter_test(2),
                create_parameter_test(3),
                create_parameter_test(4)
        };

        auto device = DeviceFactory::createDevice((DeviceType::AUDIO));
        for ( auto p : parameter ) {
            device->addParameter(p);
        }

        typename Device::Iter pi, pi_end;
        tie(pi, pi_end) = device->availableParameters();
        auto ti = parameter.begin();
        auto ti_end = parameter.end();
        BOOST_REQUIRE(pi != pi_end);
        BOOST_REQUIRE(ti != ti_end);
        auto pCount = count_if(pi, pi_end, all);
        auto tCount = count_if(ti, ti_end, all);
        BOOST_REQUIRE(pCount == tCount);
        BOOST_CHECK(equal(pi, pi_end, ti, compare_test));
    }

BOOST_AUTO_TEST_SUITE_END()
#include <boost/test/unit_test.hpp>

#include <parameter_table/ParameterTable.h>

#include "../../src/source/parameter_table/ParameterTableImplementation.h"
#include "../test_data/device/TestDevices.h"

using namespace itk;

BOOST_AUTO_TEST_SUITE( ParameterTableTests )

    BOOST_AUTO_TEST_CASE( should_store_registered_parameters ) {
        auto parameterTable = ParameterTableImplementation();
        auto device = TestDevices::createParameterCopyDevice(4, 1);
        IndexType deviceId = 3;
        parameterTable.registerDeviceParameters(deviceId, *device);
        IndexType parameterId = 2;
        parameterTable.setParameterValue(deviceId, parameterId, 4.0);
        BOOST_CHECK(parameterTable.hasParameter(deviceId, parameterId));
        BOOST_CHECK(parameterTable.getParameterValue(deviceId, parameterId) == 4.0);
    }

BOOST_AUTO_TEST_SUITE_END()
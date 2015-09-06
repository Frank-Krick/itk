//
// Created by Frank Krick on 9/4/15.
//

#include "ParameterTableImplementation.h"

#include <tuple>
#include <device_graph/DeviceGraph.h>

namespace itk {

    using namespace std;

    void ParameterTableImplementation::registerDeviceParameters(IndexType deviceId, Device &device) {
        typename Device::Iter di, di_end;
        auto deviceParameters = parameterTable[deviceId];
        for (tie(di, di_end) = device.availableParameters(); di != di_end; ++di) {
            deviceParameters.push_back(*di);
        }
    }

    void ParameterTableImplementation::unregisterDeviceParameters(IndexType deviceId) {
        parameterTable.erase(deviceId);
    }
}
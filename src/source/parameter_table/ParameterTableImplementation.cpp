//
// Created by Frank Krick on 9/4/15.
//

#include "ParameterTableImplementation.h"

namespace itk {

    using namespace std;

    void ParameterTableImplementation::registerDeviceParameters(IndexType deviceId, Device &device) {
        typename Device::Iter di, di_end;
        auto deviceParameters = parameterTable[deviceId];
        for (tie(di, di_end) = device.availableParameters(); di != di_end; ++di) {
            deviceParameters.push_back(*di);
        }
        parameterTable[deviceId] = deviceParameters;
    }

    void ParameterTableImplementation::unregisterDeviceParameters(IndexType deviceId) {
        parameterTable.erase(deviceId);
        valueMap.erase(deviceId);
    }

    bool ParameterTableImplementation::hasParameter(IndexType deviceId, IndexType parameterId) {
        auto parameters = parameterTable.find(deviceId);
        if (parameters == parameterTable.end()) return false;
        auto start = parameters->second.cbegin();
        auto end = parameters->second.cend();
        for (auto p = start; p != end; ++p) {
            if (p->id == parameterId) return true;
        }
        return false;
    }

    DataType ParameterTableImplementation::getParameterValue(IndexType deviceId, IndexType parameterId) {
        return valueMap.at(deviceId).at(parameterId);
    }

    void ParameterTableImplementation::setParameterValue(IndexType deviceId, IndexType parameterId, DataType value) {
        valueMap[deviceId][parameterId] = value;
    }

}
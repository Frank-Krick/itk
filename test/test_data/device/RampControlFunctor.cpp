//
// Created by Frank Krick on 10/29/15.
//

#include "RampControlFunctor.h"

void RampControlFunctor::operator()(DataBuffer::iterator outputBegin, DataBuffer::iterator outputEnd,
                                    ControlFunctor::ParameterDeviceMap &parameter) {
    IndexType delta = 0;
    for (auto it = outputBegin; it != outputEnd; ++it) {
        if (delta < _rampSampleLength) {
            (*it) = 1.0;
        } else {
            (*it) = 0.0;
        }
        ++delta;
    }
}

ParameterList RampControlFunctor::parameterList() {
    return std::list<ParameterDescription>();
}

Functor::Ptr RampControlFunctor::clone(IndexType deviceId) {
    return std::make_shared<RampControlFunctor>(deviceId, _sampleRate, _rampSampleLength);
}

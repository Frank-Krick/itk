//
// Created by Frank Krick on 10/10/15.
//

#include "PyDevice.h"

namespace itk { namespace wrapper {

boost::python::list PyDevice::availableParameters() {
    boost::python::list result;
    auto param = _device->availableParameters();
    typename Device::Iter begin, end;
    tie(begin, end) = param;
    for (auto it = begin; it != end; ++it) {
        result.append(*it);
    }
    return result;
}

}}
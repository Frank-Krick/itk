//
// Created by Frank Krick on 9/2/15.
//

#ifndef INSTRUMENT_TOOL_KIT_DEVICE_H
#define INSTRUMENT_TOOL_KIT_DEVICE_H

#include <Typedef.h>
#include <parameter_description/ParameterDescription.h>

#include <ostream>
#include <memory>
#include <set>

namespace itk {

    enum class DeviceType { AUDIO, CONTROL };

    std::ostream & operator << (std::ostream & stream, const DeviceType & type);

    struct Device {
    public:
        typedef std::shared_ptr<Device> Ptr;
        typedef ParameterList::iterator Iter;

        virtual std::tuple<Iter, Iter> availableParameters() = 0;
        virtual DeviceType deviceType() = 0;

        virtual ~Device() {};
    };

}

#endif //INSTRUMENT_TOOL_KIT_DEVICE_H
